#include <assert.h>
#include <stdio.h>

#include "board.h"
#include "unit.h"

void print_board(const board b)
{
    for(int i = 0; i < b.width; i++)
        printf(" ___");
    printf("\n");

    for(int j = 0; j < b.height; j++)
    {
        if(j & 1)
            printf("  ");

        for(int i = 0; i < b.width; i++)
        {
            if(get_cell(b, i, j))
                printf("|XXX");
            else
                printf("|   ");
        }
        printf("|\n");

        if(j & 1)
            printf(" _");

        for(int i = 0; i < b.width; i++)
            printf("|___");

        if(j & 1 || j + 1 == b.height)
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
}

void test_clear_rows()
{
    board b;
    alloc_board(&b, 3, 5);

    int start [3][5] = {{1, 0, 1, 0, 0},
                        {1, 1, 1, 0, 0},
                        {1, 1, 1, 0, 1}};

    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 5; j++)
            set_cell(&b, i, j, start[i][j]);

    clear_rows(&b);

    int end [3][5] = {{0, 0, 0, 0, 0},
                      {1, 0, 0, 0, 0},
                      {1, 0, 1, 0, 0}};

    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 5; j++)
            assert(get_cell(b, i, j) == end[i][j]);

    free_board(b);
}

int main()
{
    test_shifts();
    test_clear_rows();

    int xs [] = { 4 };
    int ys [] = { 2 };

    unit u;
    u.pivot_x = 2;
    u.pivot_y = 3;
    u.num_cells = 1;
    u.cells_x = xs;
    u.cells_y = ys;

    rotate_unit(&u, 0);
    printf("%d, %d\n", u.cells_x[0], u.cells_y[0]);
    rotate_unit(&u, 0);
    printf("%d, %d\n", u.cells_x[0], u.cells_y[0]);
    rotate_unit(&u, 0);
    printf("%d, %d\n", u.cells_x[0], u.cells_y[0]);
    rotate_unit(&u, 0);
    printf("%d, %d\n", u.cells_x[0], u.cells_y[0]);
    rotate_unit(&u, 0);
    printf("%d, %d\n", u.cells_x[0], u.cells_y[0]);
    rotate_unit(&u, 0);
    printf("%d, %d\n", u.cells_x[0], u.cells_y[0]);
}
