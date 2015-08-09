#include "board.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

void shift_cell(int* x, int* y, dir_t dir)
{
    switch(dir) {
      case EAST:
        *x += 1;
        return;
      case WEST:
        *x -= 1;
        return;
      case SOUTHWEST:
        *x -= 1; // SW is the same as W then SE
      case SOUTHEAST:
        *x += (*y & 1);
        *y += 1;
        return;
      case NORTHWEST:
        *x -= 1; // NW is the same as W then NE
      case NORTHEAST:
        *x += (*y & 1);
        *y -= 1;
        return;
      default:
        assert(0);
    }
}

board* new_board(int width, int height)
{
    board* b = malloc(sizeof(board));
    b->width = width;
    b->height = height;
    b->cells = malloc(sizeof(int) * width * height);
    return b;
}

void free_board(board* b)
{
    free(b->cells);
    free(b);
}

board* copy_board(board* b)
{
    board* copy = new_board(b->width, b->height);
    for(int i = 0; i < b->width * b->height; i++)
        copy->cells[i] = b->cells[i];
    return copy;
}

int is_valid_cell(board* b, int x, int y)
{
    return (0 <= x && x < b->width) && (0 <= y && y < b->height);
}

int get_cell(board* b, int x, int y)
{
    assert(is_valid_cell(b, x, y));
    return b->cells[x + b->width * y];
}

void set_cell(board* b, int x, int y, int val)
{
    assert(is_valid_cell(b, x, y));
    b->cells[x + b->width * y] = val;
}

int clear_rows(board* b)
{
    int num_rows = 0;
    // Iterate from bottom to top because of the indexing.
    for (int j = b->height - 1; j >= 0; j--)
    {
        // Check whether this row is full.
        int is_full = 1;
        for (int i = 0; i < b->width; i++)
        {
            is_full &= get_cell(b, i, j);
        }
        // If it is full, keep track of that.
        if (is_full)
        {
            num_rows++;
        }
        // If it's not, but if there were full rows below it...
        else if (num_rows != 0)
        {
            for (int i = 0; i < b->width; i++)
            {
                // Move it down.
                set_cell(b, i, j + num_rows, get_cell(b, i, j));
            }
        }
        // If neither, just move on.
        else
        {
            continue;
        }
        // If we missed the else statement, then we have to clear the row.
        for (int i = 0; i < b->width; i++)
        {
            set_cell(b, i, j, 0);
        }
    }

    return num_rows;
}
