#include "solveSudoku.h"

#define MAX_CLAUSE_NUM 100000

// return true if solved, the result is stored in res
bool solveSudoku(int a[9][9], int res[9][9], bool isM) 
{       
    if(!checkLegal(a, isM)) 
    {
        #ifdef DEBUG
        printf("illegal sudoku!\n");
        #endif
        return false;
    }
    string sudokuCNFFile = gRootPath + "/temp/tempSudoku.cnf";
    SudokuToCNFFile(a, sudokuCNFFile, isM);
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

bool checkComplete(const int a[9][9]) 
{
    for(int i = 0; i < 9; ++i)
        for(int j = 0; j < 9; ++j)
            if(a[i][j] == 0) return false;
    return true;
}
// return value: 0 for illegal, 1 for legal but incomplete, 2 for complete
// is_X indicates whether to check the "%" region constraints
int checkLegal(const int a[9][9], bool isM = false) 
{
    int rowCheck[9][10];
    int colCheck[9][10];
    int boxCheck[9][10];
    int lineCheck[10];
    memset(rowCheck, 0, sizeof(rowCheck));
    memset(colCheck, 0, sizeof(colCheck));
    memset(boxCheck, 0, sizeof(boxCheck));
    memset(lineCheck, 0, sizeof(lineCheck));
    for(int i = 0; i < 9; i++) // normal sudoku rules
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

    if(!isM) return checkComplete(a) ? 2 : 1;

    for(int i = 0; i < 9; ++i) // check diagonal (right top to left bottom)
    {
        if(a[i][8 - i]) lineCheck[a[i][8 - i]]++;
        if(lineCheck[a[i][8 - i]] > 1)
        {
            // printf("main line\n");
            return 0;
        }
    }

    memset(lineCheck, 0, sizeof(lineCheck));// reset and use as box check
    for(int i = 1; i <= 3; ++i) // % uper left box
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
    for(int i = 5; i <= 7; ++i) // % lower right box
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

    if(!checkComplete(a)) return 1;

    return 2;
}

// row and col start from 1.
// All possible situations are mapped to the numbers 1 to 729
int transToVar(int row, int col, int num)
{
    return ((row - 1) * 9 + col - 1) * 9 + num;
}

// var from 1 to 729, is mapped to all situations of (row, col, num)
void transToPos(int var, int &row, int &col, int &num) 
{
    var--;
    num = var % 9 + 1;
    var /= 9;
    col = var % 9 + 1;
    row = var / 9 + 1;
    return ;
}

// generate CNF file for sudoku a[9][9], store it in fileName
void SudokuToCNFFile(const int a[9][9], const string fileName, bool isM) 
{
    static const int row[11] = {1, 1, 1, 4, 4, 4, 7, 7, 7, 2, 6};
    static const int col[11] = {1, 4, 7, 1, 4, 7, 1, 4, 7, 2, 6};
    freopen(fileName.c_str(), "w", stdout);

    if(!checkLegal(a, isM)) 
    {
        printf("error: the sudoku is illegal!\n");
        return ;
    }

    printf("p cnf 729 %d\n", MAX_CLAUSE_NUM); // 9*9*9 = 729 is the number of variables
    
    // each given number
	for(int i = 0; i < 9; ++i)
        for(int j = 0; j < 9; ++j)
        {
            if(!a[i][j])continue;
            printf("%d 0\n", transToVar(i + 1, j + 1, a[i][j]));
            for(int k = 1; k <= 9; ++k)
                if(k != a[i][j])
                    printf("%d 0\n", -transToVar(i + 1, j + 1, k));
        }

    // each empty cell has at least one number
    for(int i = 1; i <= 9; ++i)
        for(int j = 1; j <= 9; ++j)
        {
            if(a[i - 1][j - 1] != 0) continue;
            for(int num = 1; num <= 9; ++num)
                printf("%d ", transToVar(i, j, num));
            printf("0\n");
        }

    // each cell has at most one number
    for(int i = 1; i <= 9; ++i)
        for(int j = 1; j <= 9; ++j)
            for(int k1 = 1; k1 <= 9; ++k1)
                for(int k2 = k1 + 1; k2 <= 9; ++k2)
                {
                    printf("%d %d 0\n", -transToVar(i, j, k1), -transToVar(i, j, k2));
                    printf("%d %d 0\n", -transToVar(i, k1, j), -transToVar(i, k2, j));
                    printf("%d %d 0\n", -transToVar(k1, i, j), -transToVar(k2, i, j));
                }

    // each number appears at most once in each 3 * 3 box
    for(int i = 0; i < 9; ++i)// i: box id
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

    if(!isM) 
    {
        freopen("CON", "w", stdout);
        return ;
    }
    // each number appears at most once in a diagonal (right top to left bottom)
    for(int num = 1; num <= 9; ++num)
        for(int i = 1; i <= 9; ++i)
            for(int j = i + 1; j <= 9; ++j)
                printf("%d %d 0\n", -transToVar(i, 9 - i + 1, num), -transToVar(j, 9 - j + 1, num));

    // each number appears at most once in % boxes
    for(int i = 9; i < 11; ++i)// i: box id
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
    return ;
}

// read the result from ans[] and store it in a[9][9]
void readRes(const int ans[], int a[9][9])
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