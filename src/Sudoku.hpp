#ifndef GAME_SUDOKU
#define GAME_SUDOKU

#include <string>

#include "Game.hpp"

namespace GameSudoku {
    
class Sudoku {
  public:
    void initialize();

    void tick();

    bool isQuit() const;

  protected:
    std::string c_line;
    
    bool f_quit;
    Game game;

    int getRestartCommand();
};

} // namespace GameSudoku

#endif // GAME_SUDOKU
