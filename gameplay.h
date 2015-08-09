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
    int* source_seeds;
    int num_seeds;
} input;

typedef struct {
    int id;
    int seed;
    char* tag;
    dir_t* solution;
} result;

typedef struct {
    result* output;
} output;

output* play_game(input* input, int time_available, int mem_available,
          int cores_available, char* wops[], int num_wops);
