#include <stdlib.h>
#include <assert.h>

#include "gameplay.h"

// Note, this expects the units to already be centered.
int score_seq(board* b, unit* units, int num_units, dir_t* moves, int num_moves)
{
    int score = 0;

    int active_unit = 0;

    for(int i = 0; i < num_moves; i++)
    {
        unit* u = units + active_unit;

        // At the beginning of the loop, we should always be in a valid place
        assert(can_be_placed(u, b));

        // Move, possibly to an invalid place
        move_unit(u, moves[i]);

        // Did we lock our piece?
        if(!can_be_placed(u, b))
        {
            // Undo, then place onto the board
            move_unit(u, opposite_dir(moves[i]));
            place(u, b);

            // TODO row scoring
            int rows_cleared = clear_rows(b);

            // Move to the next piece
            active_unit++;
            if(active_unit == num_units)
                // That should have been our final move
                assert(i == num_moves - 1);
                break;
        }
    }

    // TODO power scoring

    return score;
}

output* play_game(input* input, int time_available, int mem_available,
          int cores_available, char* wops[], int num_wops) {
    // TODO nop for now
    return NULL;
}
