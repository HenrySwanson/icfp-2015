#include "board.h"

#include <assert.h>
#include <stdlib.h>

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

    for(int j = 0; j < b->height; j++)
    {
        int is_full = 1;
        for(int i = 0; i < b->width; i++)
            is_full &= get_cell(b, i, j);

        // If we have a full row, clear it
        if(is_full)
            num_rows++;
        // Else, move this row down
        else if(num_rows != 0)
            for(int i = 0; i < b->width; i++)
                set_cell(b, i, j - num_rows, get_cell(b, i, j));
        // If neither, look at the next row
        else
            continue;
        
        // If we didn't hit the else, clear the row
        for(int i = 0; i < b->width; i++)
            set_cell(b, i, j, 0);
    }

    return num_rows;
}
