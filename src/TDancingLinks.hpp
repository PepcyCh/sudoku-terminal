#ifndef T_DANCINGLINKS
#define T_DANCINGLINKS

#include <cstdio>
#include <cstring>
#include <cassert>
#include <vector>
#include <algorithm>

#include "Grid.cpp"

namespace GameSudoku {

const int N = 81 * 4 + 1;
const int M = 81 * 9;
const int NM = N * M;

template <typename T, size_t SIZE>
struct MemoryPool {
    char mem[SIZE * sizeof (T)], *top;

    MemoryPool() {
        init();
    }

    void init() {
        top = mem;
    }

    void *alloc() {
        char *res = top;
        top += sizeof (T);
        assert(top < mem + SIZE * sizeof (T));
        return (void *) res;
    }
};

struct DancingLink {
    struct Node {
        Node *left, *right, *up, *down, *col;
        int size, id;

        static MemoryPool<Node, NM> pool;

        Node() : size(-1), id(0) {
            left = right = up = down = col = this;
        }

        void *operator new(size_t) {
            return pool.alloc();
        }
    } *head, *cols[N];
    int colNum;
    std::vector<int> ans, stack;

    DancingLink(int colNum) : colNum(colNum) {
        head = new Node();
        for (int i = 1; i < colNum; i++) {
            cols[i] = new Node();
            cols[i]->size = 0;
            cols[i]->id = i;

            cols[i]->left = head->left;
            head->left->right = cols[i];
            cols[i]->right = head;
            head->left = cols[i];
        }
    }

    Node *insert(int c) {
        Node *temp = new Node();
        temp->col = cols[c];
        ++temp->col->size;

        temp->up = cols[c]->up;
        cols[c]->up->down = temp;
        temp->down = cols[c];
        cols[c]->up = temp;

        return temp;
    }

    void addSubset(const std::vector<int> &s, int id) {
        Node *t1 = NULL, *t2 = NULL;
        for (auto i : s) {
            if (!t1) {
                t1 = insert(i);
                t1->id = id;
            } else {
                t2 = insert(i);
                t2->id = id;

                t2->left = t1->left;
                t1->left->right = t2;
                t2->right = t1;
                t1->left = t2;
            }
        }
    }

    Node *selectMinCol() {
        Node *u = head->right, *res = u;
        for (; u != head; u = u->right) if (u->size < res->size) res = u;
        return res;
    }

    void cover(Node *col) {
        col->left->right = col->right;
        col->right->left = col->left;

        for (Node *row = col->down; row != col; row = row->down) {
            for (Node *u = row->right; u != row; u = u->right) {
                u->up->down = u->down;
                u->down->up = u->up;
                --u->col->size;
            }
        }
    }

    void uncover(Node *col) {
        col->left->right = col;
        col->right->left = col;

        for (Node *row = col->down; row != col; row = row->down) {
            for (Node *u = row->right; u != row; u = u->right) {
                u->up->down = u;
                u->down->up = u;
                ++u->col->size;
            }
        }
    }

    int ansCnt;
    int _solve(bool multi) {
        if (head->right == head) {
            ++ansCnt;
            if (ansCnt == 1) ans = stack;
            else if (ansCnt > 1) return -2;
            return 1;
        }

        Node *col = selectMinCol();
        cover(col);

        int flag = -1;
        for (Node *u = col->down; u != col; u = u->down) {
            stack.push_back(u->id);
            for (Node *v = u->right; v != u; v = v->right) cover(v->col);

            int temp = _solve(multi);
            if (temp == 1) {
                flag = temp;
                if (!multi) return 1;
            } else if (temp == -2) {
                flag = temp;
                return -2;
            }

            stack.pop_back();
            for (Node *v = u->right; v != u; v = v->right) uncover(v->col);
        }

        uncover(col);
        return flag;
    }

    int solve(bool multi = false) {
        ansCnt = 0;
        ans.clear();
        stack.clear();

        int flag = _solve(multi);
        return flag;
    }
};
MemoryPool<DancingLink::Node, NM> DancingLink::Node::pool;

int getColID_coor(int x, int y) {
    return x * 9 + y + 1;
}

int getColID_col(int col, int val) {
    return 81 + col * 9 + val;
}

int getColID_row(int row, int val) {
    return 81 * 2 + row * 9 + val;
}

int getColID_box(int box, int val) {
    return 81 * 3 + box * 9 + val;
}

int getBoxID(int x, int y) {
    return x / 3 * 3 + y / 3;
}

int getRowID(int x, int y, int val) {
    return (getColID_coor(x, y) - 1) * 9 + val;
}

void getCoor(int rowID, int &x, int &y, int &val) {
    val = (rowID - 1) % 9 + 1;
    rowID = (rowID - val) / 9;
    y = rowID % 9;
    x = rowID / 9;
}

void getSubsetVec(int x, int y, int val, std::vector<int> &subset) {
    subset.clear();

    subset.push_back(getColID_coor(x, y));
    subset.push_back(getColID_col(x, val));
    subset.push_back(getColID_row(y, val));
    subset.push_back(getColID_box(getBoxID(x, y), val));
}

int dlxSolve(Grid &grid, bool multi) {
    DancingLink dlx(N);

    for (int i = 0; i < 9; i++) for (int j = 0; j < 9; j++) {
        static std::vector<int> subset;

        if (grid.mat[i][j]) {
            getSubsetVec(i, j, grid.mat[i][j], subset);
            dlx.addSubset(subset, getRowID(i, j, grid.mat[i][j]));
        } else for (int val = 1; val <= 9; val++) {
            getSubsetVec(i, j, val, subset);
            dlx.addSubset(subset, getRowID(i, j, val));
        }
    }

    int ret = dlx.solve(multi);

    for (auto ans : dlx.ans) {
        int x, y, val;
        getCoor(ans, x, y, val);
        grid.mat[x][y] = val;
    }

    return ret;
}

} // namespace GameSudoku

#endif // T_DANCINGLINKS
