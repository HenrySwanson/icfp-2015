#include "unit.h"

#include <assert.h>

void shift_unit(unit* u, dir_t dir)
{
    shift_cell(&u->pivot_x, &u->pivot_y, dir);
    for(int i = 0; i < u->num_cells; i++)
        shift_cell(&u->cells_x[i], &u->cells_y[i], dir);
}

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

int can_be_placed(const unit u, const board b)
{
    for(int i = 0; i < u.num_cells; i++)
    {
        int xi = u.cells_x[i];
        int yi = u.cells_y[i];
        if(!is_valid_cell(b, xi, yi) || get_cell(b, xi, yi))
            return 0;
    }
    return 1;
}

void place(const unit u, board* b)
{
    assert(can_be_placed(u, *b));

    for(int i = 0; i < u.num_cells; i++)
    {
        int xi = u.cells_x[i];
        int yi = u.cells_y[i];
        set_cell(b, xi, yi, 1);
    }
}
