#ifndef GAME
#define GAME

#include <string>

#include "Grid.cpp"

namespace GameSudoku {
    
class Game {
  public:
    // 1:quit, 0:success
    int initialize();

    void tick();

    bool isQuit() const;

    void print() const;
  protected:
    bool f_quit;

    std::string c_line;

    Grid grid;

    // 1:quit, 0:success, -1:invalid
    int getFillCommand(int &x, int &y, int &val);

    // 1:quit, 0:success, -1:invalid
    int getDifficultyCommand(int &difficulty);
};

}

#endif // GAME
