#ifndef GRID
#define GRID

#include <cstring>

namespace GameSudoku {
    
struct Grid {
    int mat[9][9];
    bool init[9][9];

    int remain;

    void initialize() {
        memset(mat, 0, sizeof (mat));
        memset(init, true, sizeof (init));
        remain = 0;
    }
};

} // namespace GameSudoku

#endif // GRID
