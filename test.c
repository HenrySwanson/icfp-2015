#include <assert.h>
#include <stdio.h>

#include "board.h"
#include "unit.h"

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

    clear_rows(b);

    int end [3][5] = {{0, 0, 0, 0, 0},
                      {1, 0, 0, 0, 0},
                      {1, 0, 1, 0, 0}};

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

// TODO test spawning, because it's probably wrong

int main()
{
    test_shifts();
    test_clear_rows();

    printf("All tests passed!\n");
}
