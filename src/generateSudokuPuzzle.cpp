#include "global.h"

bool solveSudoku(int a[9][9], int res[9][9], int debug) 
{       
    if(!checkLegal(a)) 
    {
        printf("illegal sudoku!\n");
        return false;
    }
    string sudokuCNFFile = "D:\\code\\C\\SATsolver\\tempSudoku.cnf";
    sudokuToCNFFile(a, sudokuCNFFile);
    CNFList *sudokuCNF = new CNFList();
    sudokuCNF->buildCNFList(sudokuCNFFile);
    // printf("built\n");
    int ans[MAXN] = {0};
    memset(ans, 0, sizeof(ans));
    // printf("Sudoku CNF built. varNum: %d, clauseNum: %d\n", sudokuCNF->varNum, sudokuCNF->clauseNum);

    int temp = 0;
    if(!DPLLLauncher(ans, sudokuCNF, temp, 1)) 
    {
        // printf("No solution!\n");
        return false;
    }
    readRes(ans, res);

    delete sudokuCNF;
    return true;
}

void generateSudokuPuzzle(int a[9][9], int finalAns[9][9], int n) 
{
    int nums[9];
    int finalMap[9][9] = {0};
    memset(finalMap, 0, sizeof(finalMap));
    for(int i = 0; i < 9; ++i) nums[i] = i + 1;
    shuffle(nums, 9);

    for(int i = 0; i < 9; ++i)
    {
        int row = 1 + i / 3, col = 1 + i % 3;
        finalMap[row][col] = nums[i];
    }
    shuffle(nums, 9);
    for(int i = 0; i < 9; ++i)
    {
        int row = 5 + i / 3, col = 5 + i % 3;
        finalMap[row][col] = nums[i];
    }
    if(finalMap[3][3] == finalMap[5][5]) Swap(finalMap[3][3], finalMap[1][1]);

    int temp[9][9];

    if(!solveSudoku(finalMap, finalMap))
    {
        generateSudokuPuzzle(a, temp, n);
        return ;
    }
    int ans[MAXN] = {0};

    for(int i = 0; i < 9; ++i)
        for(int j = 0; j < 9; ++j)
            finalAns[i][j] = finalMap[i][j];

    // printf("finalMap:\n");
    // printGrid(finalMap);
    if(n >= 20)LasvegasDFS(finalMap, n);
    else LasvegasW(finalMap, n);

    for(int i = 0; i < 9; ++i)
        for(int j = 0; j < 9; ++j)
            a[i][j] = finalMap[i][j];

    return ;
}
// bool vis[9][9];
void LasvegasDFS(int a[9][9], int remain) 
{
    int row, col, num;
    int cnt = 0;
    int temp[9][9];
    // memset(vis, 0, sizeof(vis));
    while(cnt < 81 - remain) 
    {
        row = rand() % 9;
        col = rand() % 9;
        if(!a[row][col]) continue;
        num = a[row][col];
        bool flag = true;
        for(int i = 1; i <= 9; ++i) 
        {
            if(i == num)continue;
            a[row][col] = i;
            if(!checkLegal(a)) 
            {
                continue;
            }
            // printf("solving\n");
            if(solveSudoku(a, temp)) 
            {
                flag = false;
                // printf("can't solve\n");
                break;
            }
            // printf("can solve\n");
        }
        a[row][col] = 0;
        cnt++;

        // printGrid(a);// debug
    }
    remain = 81 - cnt;
}
void LasvegasW(int a[9][9], int remain) 
{
    int row, col, num;
    int cnt = 0;
    // memset(vis, 0, sizeof(vis));
    while(cnt < 81 - remain) 
    {
        row = rand() % 9;
        col = rand() % 9;
        if(!a[row][col]) continue;
        a[row][col] = 0;
        cnt++;
    }
    remain = 81 - cnt;
    return ;
}

void printGrid(int a[9][9]) 
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
            printf("%d ", a[i][j]);
        }
        printf("|\n");
        if((i + 1) % 3 == 0)
            printf("  +-------+-------+-------+\n");
    }
    return ;
}
void printGame(int a[9][9], int base[9][9]) 
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
            if(base[i][j]) printf("%d'", a[i][j]);
            else printf("%d ", a[i][j]);
        }
        printf("|\n");
        if((i + 1) % 3 == 0)
            printf("  +-------+-------+-------+\n");
    }
    return ;
}

int transToVar(int row, int col, int num) // from 1.
{
    return ((row - 1) * 9 + col - 1) * 9 + num;
}
void transToPos(int var, int &row, int &col, int &num) 
{
    var--;
    num = var % 9 + 1;
    var /= 9;
    col = var % 9 + 1;
    row = var / 9 + 1;
    return ;
}

/*
id of each 3 * 3 box
0 1 2
3 4 5
6 7 8
*/

int checkLegal(int a[9][9]) 
{
    int rowCheck[9][10];
    int colCheck[9][10];
    int boxCheck[9][10];
    int lineCheck[10];
    memset(rowCheck, 0, sizeof(rowCheck));
    memset(colCheck, 0, sizeof(colCheck));
    memset(boxCheck, 0, sizeof(boxCheck));
    memset(lineCheck, 0, sizeof(lineCheck));
    for(int i = 0; i < 9; i++) 
    {
        for(int j = 0; j < 9; j++) 
        {
            int num = a[i][j];
            if(!num) continue;
            rowCheck[i][num]++;
            colCheck[j][num]++;
            boxCheck[(i / 3) * 3 + (j / 3)][num]++;
            if(rowCheck[i][num] > 1 || colCheck[j][num] > 1 || boxCheck[(i / 3) * 3 + (j / 3)][num] > 1)
            {
                // printf("row or line or box\n");
                return 0;
            }
        }
    }
    for(int i = 0; i < 9; ++i)
    {
        if(a[i][8 - i]) lineCheck[a[i][8 - i]]++;
        if(lineCheck[a[i][8 - i]] > 1)
        {
            // printf("main line\n");
            return 0;
        }
    }
    memset(lineCheck, 0, sizeof(lineCheck));// reset and use as box check
    for(int i = 1; i <= 3; ++i) // uper left box
    {
        for(int j = 1; j <= 3; ++j)
        {
            if(a[i][j]) lineCheck[a[i][j]]++;
            if(lineCheck[a[i][j]] > 1)
            {
                // printf("uper left box\n"); 
                return 0;
            }
        }
    }
    memset(lineCheck, 0, sizeof(lineCheck));
    for(int i = 5; i <= 7; ++i) // lower right box
    {
        for(int j = 5; j <= 7; ++j)
        {
            if(a[i][j]) lineCheck[a[i][j]]++;
            if(lineCheck[a[i][j]] > 1)
            {
                // printf("lower right box\n"); 
                return 0;
            }
        }
    }

    for(int i = 0; i < 9; ++i)
        for(int j = 0; j < 9; ++j)
            if(a[i][j] == 0) return 1;

    return 2;
}

void sudokuToCNFFile(int a[9][9], string fileName) 
{
    freopen(fileName.c_str(), "w", stdout);

    if(!checkLegal(a)) 
    {
        printf("error: the sudoku is illegal!\n");
        return ;
    }

    printf("p cnf 729 100000\n");
    
	for(int i = 0; i < 9; ++i)
        for(int j = 0; j < 9; ++j)
            if(a[i][j] != 0)
            {
                printf("%d 0\n", transToVar(i + 1, j + 1, a[i][j]));
                for(int k = 1; k <= 9; ++k)
                {
                    if(k != a[i][j])
                        printf("%d 0\n", -transToVar(i + 1, j + 1, k));
                }
            }

    for(int i = 1; i <= 9; ++i)
        for(int j = 1; j <= 9; ++j)
        {
            if(a[i - 1][j - 1] != 0) continue;
            for(int num = 1; num <= 9; ++num)
                printf("%d ", transToVar(i, j, num));
            printf("0\n");
        }

    for(int i = 1; i <= 9; ++i)
        for(int j = 1; j <= 9; ++j)
            for(int k1 = 1; k1 <= 9; ++k1)
                for(int k2 = k1 + 1; k2 <= 9; ++k2)
                {
                    printf("%d %d 0\n", -transToVar(i, j, k1), -transToVar(i, j, k2));
                    printf("%d %d 0\n", -transToVar(i, k1, j), -transToVar(i, k2, j));
                    printf("%d %d 0\n", -transToVar(k1, i, j), -transToVar(k2, i, j));
                }

    for(int num = 1; num <= 9; ++num)
        for(int i = 1; i <= 9; ++i)
            for(int j = i + 1; j <= 9; ++j)
                printf("%d %d 0\n", -transToVar(i, 9 - i + 1, num), -transToVar(j, 9 - j + 1, num));

    // freopen("CON", "w", stdout);
    // return ;

    int row[11] = {1, 1, 1, 4, 4, 4, 7, 7, 7, 2, 6};
    int col[11] = {1, 4, 7, 1, 4, 7, 1, 4, 7, 2, 6};

    for(int i = 0; i < 11; ++i)// i: box id
    {
        for(int num = 1; num <= 9; ++num)
            for(int k1 = 0; k1 < 9; ++k1)
                for(int k2 = k1 + 1; k2 < 9; ++k2)
                {
                    int r1 = row[i] + k1 / 3, r2 = row[i] + k2 / 3;
                    int c1 = col[i] + k1 % 3, c2 = col[i] + k2 % 3;
                    printf("%d %d 0\n", -transToVar(r1, c1, num), -transToVar(r2, c2, num));
                }
    }
    

    freopen("CON", "w", stdout);
}

void readRes(int ans[], int a[9][9])
{
    for(int i = 0; i < 9; ++i)
        for(int j = 0; j < 9; ++j)
            a[i][j] = 0;
    for(int var = 1; var <= 729; ++var)
    {
        if(ans[var] == 1)
        {
            int row, col, num;
            transToPos(var, row, col, num);
            a[row - 1][col - 1] = num;
        }
    }
    return ;
}
