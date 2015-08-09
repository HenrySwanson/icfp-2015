#include "board.h"
#include "unit.h"

typedef struct {
    int id;
    unit** units;
    int num_units;
    board* board;
    int width;
    int height;
    int source_length;
    int** source_seeds;
    int num_seeds;
} input;
