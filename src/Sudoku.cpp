#include <cstdio>

#include "Sudoku.hpp"

#include <string>
#include <iostream>

#include "TColor.hpp"
#include "TStringTools.hpp"

void GameSudoku::Sudoku::initialize() {
    f_quit = false;
}

void GameSudoku::Sudoku::tick() {
    int ret = game.initialize();

    if (ret == 0) {
        while (!game.isQuit()) {
            game.tick();
        }
    }

    while (getRestartCommand() != 0);
}

bool GameSudoku::Sudoku::isQuit() const {
    return f_quit;
}

int GameSudoku::Sudoku::getRestartCommand() {
    printf(C_L_YELLOW "Enter (yes/Y) for quit and (no/N) for restart.\n" C_NONE);

    std::getline(std::cin, c_line);

    if (toLower(c_line, 3) == "yes" || toLower(c_line, 1) == "y") {
        f_quit = true;
        return 0;
    } else if (toLower(c_line, 2) == "no" || toLower(c_line, 1) == "n") {
        f_quit = false;
        return 0;
    } else {
        printf(C_L_RED "Unknown command.\n" C_NONE);
        return -1;
    }
}
