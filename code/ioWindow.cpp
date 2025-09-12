#include "global.h"

void playSudoku(int a[9][9], int finalAns[9][9]) 
{
    int base[9][9];
    memset(base, 0, sizeof(base));
    for(int i = 0; i < 9; ++i)
        for(int j = 0; j < 9; ++j)
            if(a[i][j]) base[i][j] = 1;
    printf("Welcome to play Sudoku!\n");
    printf("Puzzle:\n");
    printGrid(a);
    // printGrid(base);
    int row, col, num;
    int var;
    bool flag = false;
    while(!flag)
    {
        printf("Input your move(RowColNum), input 0 to check the answer: ");
        var = chooseNum(0, 999);
        if(var == 0)
        {
            printf("The answer:\n");
            printGrid(finalAns);
            return ;
        }
        row = var / 100;
        col = (var / 10) % 10;
        num = var % 10;
        if(row < 1 || row > 9 || col < 1 || col > 9 || num < 1 || num > 9) 
        {
            printf("Invalid input! Please input again!\n");
            continue;
        }

        if(base[row - 1][col - 1]) 
        {
            printf("This position is fixed, you can't change it!\n");
            continue;
        }
        a[row - 1][col - 1] = num;
        printGame(a, base);
        flag = checkLegal(a) == 2 ? true : false;
    }
    printf("Congratulations! You win!\n");
    return ;
}

int chooseNum(int start, int end) // [start, end]
{
    int num = readInt();
    while(num < start || num > end) 
    {
        printf("Invalid number! Please choose again!\n");
        num = readInt();
    }
    return num;
}

int chooseMainMenu() 
{
    freopen("CON", "r", stdin);
    freopen("CON", "w", stdout);
    int choice;
    printf("Menu:\n");
    printf("1. Solve SAT problem\n");
    printf("2. Play sudoku\n");
    printf("3. Exit\n");
    printf("Your choice: ");
    choice = chooseNum(1, 3);
    return choice;
}
