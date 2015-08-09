#include "unit.h"

#include <assert.h>
#include <stdlib.h>

// Rotating is complicated. We'll use a three-axis coordinate system.
// Constant a is \, constant b is /, and constant c is --.
// Positive is always up and to the right.
// Constraint: a + b + c == 0.
void to_triple(int x, int y, int* a, int* b, int* c)
{
    *a = x - (y - (y & 1)) / 2;
    *c = y;
    *b = -(*a + *c);
}

void from_triple(int a, int b, int c, int* x, int* y)
{
    assert(a + b + c == 0);
    *x = a + (c - (c & 1)) / 2;
    *y = c;
}

/** Destructively rotates the unit in the given direction. */
void rotate_unit(unit* u, int ccw)
{
     int pivot_a, pivot_b, pivot_c;
     to_triple(u->pivot_x, u->pivot_y, &pivot_a, &pivot_b, &pivot_c);

     for(int i = 0; i < u->num_cells; i++)
     {
        int cell_a, cell_b, cell_c;
        to_triple(u->cells_x[i], u->cells_y[i], &cell_a, &cell_b, &cell_c);

        int da = cell_a - pivot_a;
        int db = cell_b - pivot_b;
        int dc = cell_c - pivot_c;

        if(ccw)
        {
            cell_a = pivot_a - db;
            cell_b = pivot_b - dc;
            cell_c = pivot_c - da;
        }
        else
        {
            cell_a = pivot_a - dc;
            cell_b = pivot_b - da;
            cell_c = pivot_c - db;
        }

        from_triple(cell_a, cell_b, cell_c, &u->cells_x[i], &u->cells_y[i]);
     }
}

unit* new_unit(int num_cells)
{
    unit* u = malloc(sizeof(unit));
    u->num_cells = num_cells;
    u->cells_x = malloc(sizeof(int) * num_cells);
    u->cells_y = malloc(sizeof(int) * num_cells);
    return u;
}

void free_unit(unit* u)
{
    free(u->cells_x);
    free(u->cells_y);
    free(u);
}

unit* copy_unit(unit* u)
{
    unit* v = new_unit(u->num_cells);
    v->pivot_x = u->pivot_x;
    v->pivot_y = u->pivot_y;
    for(int i = 0; i < u->num_cells; i++)
    {
        v->cells_x[i] = u->cells_x[i];
        v->cells_y[i] = u->cells_y[i];
    }
    return v;
}

void move_unit(unit* u, dir_t dir)
{
    if(dir == CCWISE)
        rotate_unit(u, 1);
    else if(dir == CWISE)
        rotate_unit(u, 0);
    else
    {
        shift_cell(&u->pivot_x, &u->pivot_y, dir);
        for(int i = 0; i < u->num_cells; i++)
            shift_cell(&u->cells_x[i], &u->cells_y[i], dir);
    }
}

int can_be_placed(unit* u, board* b)
{
    for(int i = 0; i < u->num_cells; i++)
    {
        int xi = u->cells_x[i];
        int yi = u->cells_y[i];
        if(!is_valid_cell(b, xi, yi) || get_cell(b, xi, yi))
            return 0;
    }
    return 1;
}

void place(unit* u, board* b)
{
    assert(can_be_placed(u, b));

    for(int i = 0; i < u->num_cells; i++)
    {
        int xi = u->cells_x[i];
        int yi = u->cells_y[i];
        set_cell(b, xi, yi, 1);
    }
}

void spawn_unit(unit* u, board* b)
{
    assert(u->num_cells > 0);

    // Find the y-coordinate of the topmost cell and the x-coordinates of the
    // outermost cells.
    int top, left, right;
    top = u->cells_y[0];
    left = right = u->cells_x[0];

    for(int i = 1; i < u->num_cells; i++)
    {
       int xi = u->cells_x[i];
       int yi = u->cells_y[i];
       if(yi < top)
           top = yi;
       if(xi < left)
           left = xi;
       if(xi > right)
           right = xi;
    }

    // Shift north/south until we're vertically aligned
    // TODO can probably do this better by shifting straight north by 2
    if(top > 0)
        for(int i = 0; i < top; i++)
            move_unit(u, NORTHWEST);
    else if(top < 0)
        for(int i = 0; i < -top; i++)
            move_unit(u, SOUTHWEST);

    // We want this quantity to be as close to 0 as possible. But shifting
    // changes both left and right, so it changes by 2. So 1 is also okay.
    int margin_diff = (b->width - 1 - right) - (left - 0);

    // We'd divide by 2, but (-1) / 2 = 0, and we want it to be -1.
    // So we do a (signed) shift instead.
    int shift_dist = margin_diff >> 1;

    for(int i = 1; i < u->num_cells; i++)
        u->cells_x[i] += shift_dist;
    u->pivot_x += shift_dist;
}
