#ifndef T_GRID
#define T_GRID

#include <cstdlib>
#include <ctime>

#include "Grid.cpp"
#include "TDancingLinks.hpp"

namespace GameSudoku {
    
bool check(const Grid &grid, int x, int y, int val) {
    if (val == 0) return true;

    for (int i = 0; i < 9; i++) if (i != y && grid.mat[x][i] == val) return false;
    for (int i = 0; i < 9; i++) if (i != x && grid.mat[i][y] == val) return false;

    for (int i = x / 3 * 3; i < (x / 3 + 1) * 3; i++)
        for (int j = y / 3 * 3; j < (y / 3 + 1) * 3; j++)
            if (i != x && j != y && grid.mat[i][j] == val) return false;

    return true;
}

const int T_GRID_INIT = 11;
void gen(Grid &grid, int remain) {
    srand(time(NULL));

    do {
        for (int i = 0; i < T_GRID_INIT; i++) {
            int x, y, val;

            do {
                x = rand() % 9;
                y = rand() % 9;
            } while (grid.mat[x][y]);

            do {
                val = rand() % 9 + 1;
            } while (!check(grid, x, y, val));

            grid.mat[x][y] = val;
        }
    } while (dlxSolve(grid, false) == -1);

    for (int i = 0; i < remain; i++) {
        int x, y, val;

        while (true) {
            do {
                x = rand() % 9;
                y = rand() % 9;
            } while (!grid.init[x][y]);

            val = grid.mat[x][y];
            grid.mat[x][y] = 0;
            grid.init[x][y] = false;

            if (dlxSolve(grid, true) == 1) break;

            grid.mat[x][y] = val;
            grid.init[x][y] = true;
        }
    }

    for (int i = 0; i < 9; i++) for (int j = 0; j < 9; j++) if (!grid.init[i][j]) grid.mat[i][j] = 0;
}

} // namespace GameSudoku

#endif // T_GRID
