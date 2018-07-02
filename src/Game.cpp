#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>

#include "Game.hpp"

#include "TColor.hpp"
#include "TStringTools.hpp"
#include "TGrid.hpp"

const int MAX_DIFFICULTY = 5;
const int d_remain[MAX_DIFFICULTY + 1] = {0, 15, 30, 45, 60, 64};

int GameSudoku::Game::initialize() {
    grid.initialize();
    f_quit = false;

    int difficulty = 1;
    while (getDifficultyCommand(difficulty) != 0) {
        if (difficulty == 1) {
            f_quit = true;
            return 1;
        }
    }

    gen(grid, d_remain[difficulty]);
    grid.remain = d_remain[difficulty];

    print();

    return 0;
}

void GameSudoku::Game::tick() {
    int x, y, val;
    int ret = getFillCommand(x, y, val);

    if (ret == 1) f_quit = true;
    if (ret != 0) return;

    grid.mat[x][y] = val;

    print();

    if (--grid.remain == 0) {
        printf(C_L_YELLOW "You win!" C_NONE);
        f_quit = true;
    }
}

bool GameSudoku::Game::isQuit() const {
    return f_quit;
}

void GameSudoku::Game::print() const {
    system("clear");

    for (int i = 0; i < 9; i++) for (int j = 0; j < 9; j++) {
        if (grid.init[i][j]) printf(C_L_CYAN "%d%c" C_NONE, grid.mat[i][j], " \n"[j == 8]);
        else printf(C_L_BLACK "%d%c" C_NONE, grid.mat[i][j], " \n"[j == 8]);
    }
}

int GameSudoku::Game::getFillCommand(int &x, int &y, int &val) {
    printf(C_L_YELLOW "Enter the command [x] [y] [val], or enter (quit/Q) for quit.\n" C_NONE);

    std::getline(std::cin, c_line);

    if (toLower(c_line, 4) == "quit" || toLower(c_line, 1) == "q") {
        return 1;
    }

    if (sscanf(c_line.c_str(), "%d %d %d", &x, &y, &val) == 3) {
        --x;
        --y;

        if (x < 0 || y < 0 || x >= 9 || y >= 9) {
            printf(C_L_RED "Coordinate out of bound.\n" C_NONE);
            return -1;
        } else if (val < 0 || val > 9) {
            printf(C_L_RED "Number out of bound.\n" C_NONE);
            return -1;
        } else if (grid.init[x][y]) {
            printf(C_L_RED "You can't change an initial number.\n" C_NONE);
            return -1;
        } else if (!check(grid, x, y, val)) {
            printf(C_L_RED "You can't change (%d, %d) to %d for there is another %d in the same row/column/box.\n" C_NONE,
                    x + 1, y + 1, val, val);
            return -1;
        } else {
            return 0;
        }
    } else {
        printf(C_L_RED "Unknown command.\n" C_NONE);
        return -1;
    }
}

int GameSudoku::Game::getDifficultyCommand(int &difficulty) {
    printf(C_L_YELLOW "Enter the difficulty, or enter (quit/Q) for quit.\n" C_NONE);
    for (int i = 1; i <= MAX_DIFFICULTY; i++)
        printf(C_L_YELLOW "  %d: %d empty blocks\n" C_NONE, i, d_remain[i]);

    std::getline(std::cin, c_line);

    if (toLower(c_line, 4) == "quit" || toLower(c_line, 1) == "q") {
        return 1;
    }

    if (sscanf(c_line.c_str(), "%d", &difficulty) == 1) {
        if (difficulty <= 0 || difficulty > MAX_DIFFICULTY) {
            printf(C_L_RED "Difficulty out of bound\n" C_NONE);
            return -1;
        } else {
            return 0;
        }
    } else {
        printf(C_L_RED "Unknown command.\n" C_NONE);
        return -1;
    }
}
