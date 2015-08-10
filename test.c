#include <assert.h>
#include <stdio.h>

#include "board.h"
#include "unit.h"

#define VERBOSE 0 // change to 1 for detailed output

void ugly_print_board(board* b)
{
    for (int j = 0; j < b->height; j++)
    {
        for (int i = 0; i < b->width; i++)
        {
            if (get_cell(b, i, j))
            {
                printf("1");
            }
            else
            {
                printf("0");
            }
        }
        printf("\n");
    }
}

void print_board(board* b)
{
    for(int i = 0; i < b->width; i++)
        printf(" ___");
    printf("\n");

    for(int j = 0; j < b->height; j++)
    {
        if(j & 1)
            printf("  ");

        for(int i = 0; i < b->width; i++)
        {
            if(get_cell(b, i, j))
                printf("|XXX");
            else
                printf("|   ");
        }
        printf("|\n");

        if(j & 1)
            printf(" _");

        for(int i = 0; i < b->width; i++)
            printf("|___");

        if(j & 1 || j + 1 == b->height)
            printf("|\n");
        else
            printf("|_\n");
    }
}

void test_shifts()
{
    int x = 2;
    int y = 1;

    shift_cell(&x, &y, EAST);
    assert(x == 3 && y == 1);

    shift_cell(&x, &y, SOUTHEAST);
    assert(x == 4 && y == 2);

    shift_cell(&x, &y, SOUTHEAST);
    assert(x == 4 && y == 3);

    shift_cell(&x, &y, WEST);
    assert(x == 3 && y == 3);

    shift_cell(&x, &y, SOUTHWEST);
    assert(x == 3 && y == 4);

    shift_cell(&x, &y, SOUTHWEST);
    assert(x == 2 && y == 5);

    shift_cell(&x, &y, NORTHWEST);
    assert(x == 2 && y == 4);

    shift_cell(&x, &y, NORTHWEST);
    assert(x == 1 && y == 3);

    shift_cell(&x, &y, NORTHEAST);
    assert(x == 2 && y == 2);

    shift_cell(&x, &y, NORTHEAST);
    assert(x == 2 && y == 1);
}

void test_clear_rows()
{
    board* b = new_board(3, 5);

    int start [3][5] = {{1, 0, 1, 0, 0},
                        {1, 1, 1, 0, 0},
                        {1, 1, 1, 0, 1}};

    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 5; j++)
            set_cell(b, i, j, start[i][j]);
#if VERBOSE
    printf("This is the initial board state:\n", stdout);
    print_board(b);
    printf("\n\n\n", stdout);
#endif

    clear_rows(b);

#if VERBOSE
    printf("This is what happened when full rows were cleared:\n", stdout);
    print_board(b);
#endif

    int end [3][5] = {{0, 0, 0, 0, 0},
                      {0, 0, 1, 0, 0},
                      {0, 0, 1, 0, 1}};

    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 5; j++)
            assert(get_cell(b, i, j) == end[i][j]);

    free_board(b);
}

void test_rotation()
{
    // We'll rotate some cells around 2, 3
    int xs [] = { 3, 4, 5 };
    int ys [] = { 3, 2, 4 };
    int new_xs [] = { 3, 2, 4 };
    int new_ys [] = { 2, 1, 1 };

    unit u;
    u.pivot_x = 2;
    u.pivot_y = 3;
    u.num_cells = 3;
    u.cells_x = xs;
    u.cells_y = ys;

    move_unit(&u, CWISE);
    for(int i = 0; i < u.num_cells; i++)
    {
        assert(u.cells_x[i] == new_xs[i]);
        assert(u.cells_y[i] == new_ys[i]);
    }
}

void test_spawning_helper(board* b, int* xs, int* ys, int num_cells)
{
    // Create a new unit from the given arrays
    unit* u = new_unit(num_cells);
    for(int i = 0; i < num_cells; i++)
    {
        u->cells_x[i] = xs[i];
        u->cells_y[i] = ys[i];
    }

    // These are the test offsets we'll use
    int  e_offsets [6] = { 0, 2, 6, 0, 7, 1 };
    int nw_offsets [6] = { 0, 4, 5, 1, 3, 9 };
    int sw_offsets [6] = { 0, 2, 3, 5, 8, 2 };

    // We'll only move the clone around
    unit* clone = copy_unit(u);

    for(int j = 0; j < 6; j++)
    {
        for(int i = 0; i < e_offsets[j]; i++)
            move_unit(clone, EAST);
        for(int i = 0; i < nw_offsets[j]; i++)
            move_unit(clone, NORTHWEST);
        for(int i = 0; i < sw_offsets[j]; i++)
            move_unit(clone, SOUTHWEST);

        spawn_unit(clone, b);

        for(int i = 0; i < u->num_cells; i++)
        {
            assert(u->cells_x[i] == clone->cells_x[i]);
            assert(u->cells_y[i] == clone->cells_y[i]);
        }
        assert(u->pivot_x == clone->pivot_x);
        assert(u->pivot_y == clone->pivot_y);
    }

    free_unit(clone);
    free_unit(u);
}

void test_spawning()
{
    board* b = new_board(5, 5);

    int xs_1 [1] = { 2 };
    int ys_1 [1] = { 0 };
    test_spawning_helper(b, xs_1, ys_1, 1);

    int xs_2 [6] = { 2, 3, 1, 3, 2, 3 };
    int ys_2 [6] = { 0, 0, 1, 1, 2, 2 };
    test_spawning_helper(b, xs_2, ys_2, 6);

    free_board(b);
}

int main()
{
    test_shifts();
    test_clear_rows();
    test_spawning();

    printf("All tests passed!\n");
}
