#include <cstdio>

#include "Sudoku.hpp"

int main() {
    GameSudoku::Sudoku sudoku;

    sudoku.initialize();

    while (!sudoku.isQuit()) {
        sudoku.tick();
    }
    
    return 0;
}
