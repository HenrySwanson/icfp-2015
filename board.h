#ifndef _BOARD_H_
#define _BOARD_H_

extern const int FULL;
extern const int EMPTY;

/** Represents the board state. */
typedef struct {
    int width, height;
    int* cells; // TODO do we care enough to make this a bitset?
} board;

/** An enum for directions we are allowed to move in. */
typedef enum {EAST, WEST, SOUTHEAST, SOUTHWEST} dir_t;

/** Creates a new board. */
board* new_board(int width, int height);

/** Destroys a board. */
void free_board(board* b);

/** Copies a board. */
board* copy_board(board* b);

/** Returns 1 if this cell is on the board, 0 otherwise. */
int is_valid_cell(board* b, int x, int y);

/** Returns FULL or EMPTY. */
int get_cell(board* b, int x, int y);

/** Changes a cell to FULL or EMPTY. */
void set_cell(board* b, int x, int y, int val);

/** Translates the cell in the given direction. Note: destructive. */
void shift_cell(int* x, int* y, dir_t dir);

/**
 * If there's any full rows, clears them.
 * Returns the number of rows cleared.
 */
int clear_rows(board* b);

#endif
