#include "global.h"

void playSudoku(int a[9][9], const int finalAns[9][9], bool isM) 
{
    int base[9][9];
    memset(base, 0, sizeof(base));
    for(int i = 0; i < 9; ++i)
        for(int j = 0; j < 9; ++j)
            if(a[i][j]) base[i][j] = 1;
    printf("Welcome to play Sudoku!\n");
    printf("Puzzle:\n");
    printGame(a, base);
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
            printGame(finalAns, base);
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
        flag = checkLegal(a, isM) == 2 ? true : false;
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

int ChooseOptions(const string menu[]) 
{
    freopen("CON", "r", stdin);
    freopen("CON", "w", stdout);
    int choice, len = 0;
    while(menu[len] != "")++len;
    // printf("Menu:\n");
    // printf("1. Set sudoku puzzle\n");
    // printf("2. Play sudoku(auto generated)\n");
    // printf("3. Exit\n");
    // printf("Your choice: ");
    for(int i = 0; i < len; ++i) 
    {
        printf("%d. %s\n", i + 1, menu[i].c_str());
    }
    choice = chooseNum(1, len);
    return choice;
}

void printGame(const int a[9][9], const int base[9][9]) 
{
    printf(" ");
    for(int i = 1; i <= 9; ++i)
    {
        if((i - 1) % 3 == 0) printf("  ");
        printf(" %d", i);
    }
    putchar(10);
    printf("  +-------+-------+-------+\n");
    for(int i = 0; i < 9; i++) 
    {
        printf("%d ", i + 1);
        for(int j = 0; j < 9; j++) 
        {
            if(j % 3 == 0) printf("| ");
            if(base[i][j]) printf("%d ", a[i][j]);
            else 
            {   
                if(a[i][j] == 0) printColor(". ", "blue");
                else printColor(to_string(a[i][j]) + " ", "green");
            }
        }
        printf("|\n");
        if((i + 1) % 3 == 0)
            printf("  +-------+-------+-------+\n");
    }
    return ;
}