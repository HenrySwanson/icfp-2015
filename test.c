#include <stdio.h>

#include "board.h"

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

int main()
{
    board b;

    int array [15] = { 1, 1, 1,
                       0, 1, 1,
                       1, 1, 1,
                       0, 0, 0,
                       0, 0, 1 };
    b.width = 3;
    b.height = 5;
    b.cells = array;

    print_board(b);
    clear_rows(&b);
    print_board(b);
}
