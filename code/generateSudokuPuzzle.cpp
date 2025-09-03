#include "global.h"

void generateSudokuPuzzle(int a[9][9], int n) 
{
    for(int i = 0; i < 9; i++)
        for(int j = 0; j < 9; j++)
            a[i][j] = i * 9 + j + 1;
    return ;
}
