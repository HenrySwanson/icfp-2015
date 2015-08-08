#ifndef _UNIT_H_
#define _UNIT_H_

#include "board.h"

typedef struct {
    int pivot_x, pivot_y;
    int num_cells;
    int *cells_x, *cells_y;
} unit;

/** Destructively shifts the unit in the given direction. */
void shift_unit(unit* u, dir_t dir);

/** Destructively rotates the unti in the given direction. */
void rotate_unit(unit* u, int ccw);

/** Return true if the unit can be placed on the board. */
int can_be_placed(const unit u, const board b);

/** Place the unit on the board. */
void place(const unit u, board* b);

#endif
