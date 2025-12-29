#include "sudoku.h"

#define DEBUG
#define MAX_ATTEMPT 1000
#define MAX_CLAUSE_NUM 100000

Sudoku::Sudoku()
{
    Clear();
}

Sudoku::Sudoku(int _remain, bool _isM)
{
    remainNum = _remain;
    isM = _isM;
    GenerateSudokuPuzzle(_remain, _isM);
    canBePlayed = true;
}

void Sudoku::Clear()
{
    memset(grid, 0, sizeof(grid));
    memset(finalAns, 0, sizeof(finalAns));
    memset(base, 0, sizeof(base));
    remainNum = 0;
    isM = false;
    canBePlayed = false;
}

// solve the sudoku puzzle stored in _a[][]
// return true if solved, the result is stored in _finalAns[][]
bool Sudoku::SolveSudoku(const int _a[9][9], int _finalAns[9][9], bool _isM) 
{       
    if(!CheckLegal(_a, _isM)) 
    {
        #ifdef DEBUG
        printf("illegal sudoku!\n");
        #endif
        return false;
    }
    const string sudokuCNFFile = gRootPath + "/temp/tempSudoku.cnf";
    ToCNFFile(_a, sudokuCNFFile, _isM);
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
    ReadRes(ans, _finalAns);

    delete sudokuCNF;
    return true;
}

bool Sudoku::CheckComplete(const int a[9][9]) 
{
    for(int i = 0; i < 9; ++i)
        for(int j = 0; j < 9; ++j)
            if(a[i][j] == 0) return false;
    return true;
}
// return value: 0 for illegal, 1 for legal but incomplete, 2 for complete
// isM indicates whether to check the "%" region constraints
int Sudoku::CheckLegal(const int a[9][9], bool _isM) 
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

    if(!_isM) return CheckComplete(a) ? 2 : 1;

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

    if(!CheckComplete(a)) return 1;

    return 2;
}

// row and col start from 1.
// All possible situations are mapped to the numbers 1 to 729
int Sudoku::TransToVar(int row, int col, int num)
{
    return ((row - 1) * 9 + col - 1) * 9 + num;
}

// var from 1 to 729, is mapped to all situations of (row, col, num)
void Sudoku::TransToPos(int var, int &row, int &col, int &num) 
{
    var--;
    num = var % 9 + 1;
    var /= 9;
    col = var % 9 + 1;
    row = var / 9 + 1;
    return ;
}

// generate CNF file for sudoku a[9][9], store it in fileName
void Sudoku::ToCNFFile(const int a[9][9], const string fileName, bool _isM)  
{
    static const int row[11] = {1, 1, 1, 4, 4, 4, 7, 7, 7, 2, 6};
    static const int col[11] = {1, 4, 7, 1, 4, 7, 1, 4, 7, 2, 6};
    freopen(fileName.c_str(), "w", stdout);

    if(!CheckLegal(a, _isM)) 
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
            printf("%d 0\n", TransToVar(i + 1, j + 1, a[i][j]));
            for(int k = 1; k <= 9; ++k)
                if(k != a[i][j])
                    printf("%d 0\n", -TransToVar(i + 1, j + 1, k));
        }

    // each empty cell has at least one number
    for(int i = 1; i <= 9; ++i)
        for(int j = 1; j <= 9; ++j)
        {
            if(a[i - 1][j - 1] != 0) continue;
            for(int num = 1; num <= 9; ++num)
                printf("%d ", TransToVar(i, j, num));
            printf("0\n");
        }

    // each cell has at most one number
    for(int i = 1; i <= 9; ++i)
        for(int j = 1; j <= 9; ++j)
            for(int k1 = 1; k1 <= 9; ++k1)
                for(int k2 = k1 + 1; k2 <= 9; ++k2)
                {
                    printf("%d %d 0\n", -TransToVar(i, j, k1), -TransToVar(i, j, k2));
                    printf("%d %d 0\n", -TransToVar(i, k1, j), -TransToVar(i, k2, j));
                    printf("%d %d 0\n", -TransToVar(k1, i, j), -TransToVar(k2, i, j));
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
                    printf("%d %d 0\n", -TransToVar(r1, c1, num), -TransToVar(r2, c2, num));
                }
    }

    if(!_isM) 
    {
        freopen("CON", "w", stdout);
        return ;
    }
    // each number appears at most once in a diagonal (right top to left bottom)
    for(int num = 1; num <= 9; ++num)
        for(int i = 1; i <= 9; ++i)
            for(int j = i + 1; j <= 9; ++j)
                printf("%d %d 0\n", -TransToVar(i, 9 - i + 1, num), -TransToVar(j, 9 - j + 1, num));

    // each number appears at most once in % boxes
    for(int i = 9; i < 11; ++i)// i: box id
    {
        for(int num = 1; num <= 9; ++num)
            for(int k1 = 0; k1 < 9; ++k1)
                for(int k2 = k1 + 1; k2 < 9; ++k2)
                {
                    int r1 = row[i] + k1 / 3, r2 = row[i] + k2 / 3;
                    int c1 = col[i] + k1 % 3, c2 = col[i] + k2 % 3;
                    printf("%d %d 0\n", -TransToVar(r1, c1, num), -TransToVar(r2, c2, num));
                }
    }

    freopen("CON", "w", stdout);
    return ;
}

// read the result from ans[] and store it in _target[9][9]
void Sudoku::ReadRes(const int ans[], int _target[9][9])
{
    for(int i = 0; i < 9; ++i)
        for(int j = 0; j < 9; ++j)
            _target[i][j] = 0;
    for(int var = 1; var <= 729; ++var)
    {
        if(ans[var] == 1)
        {
            int row, col, num;
            TransToPos(var, row, col, num);
            _target[row - 1][col - 1] = num;
        }
    }
    return ;
}

// read puzzle from a string of length 81 + 81 + 1
void Sudoku::ReadPuzzle(string puzzleStr)
{
    Clear();
    canBePlayed = true;
    int p = 0;
    isM = readInt(puzzleStr, p) == 1 ? true : false;
    for(int i = 0; i < 9; ++i)
        for(int j = 0; j < 9; ++j)
        {
            grid[i][j] = base[i][j] = readInt(puzzleStr, p);
            if(base[i][j] != 0) remainNum++;
        }
    for(int i = 0; i < 9; ++i)
        for(int j = 0; j < 9; ++j)
        {
            finalAns[i][j] = readInt(puzzleStr, p);
        }
    return ;
}

void Sudoku::Play()
{
    if(!canBePlayed)
    {
        printf("This Sudoku puzzle cannot be played!\n");
        return ;
    }
    printf("Welcome to play Sudoku!\n");
    printf("Puzzle:\n");
    PrintGame(grid, base);
    // printGrid(base);
    int row, col, num;
    int var;
    bool winFlag = false;
    printf("Input format: a three-digit number, RowColNum, do not enter Spaces.\n");
    printf("For example, to put number 5 in row 3 column 4, input 345.\n");
    printf("If Num = 0, it means to clear the cell.\n");
    printf("0 to check the answer\n");
    printf("1 to get a hint\n");
    while(!winFlag)
    {
        printf("Input your move: ");
        var = chooseNum(0, 999);
        if(var == 0 || (var == 1 && remainNum == 1))
        {
            printf("The answer:\n");
            PrintAns();
            return ;
        }
        else if(var == 1)
        {
            // provide a hint
            int hintID = rand() % remainNum + 1;
            int cnt = 0;
            for(int i = 0; i < 9; ++i)
                for(int j = 0; j < 9; ++j)
                {
                    if(!base[i][j] && grid[i][j] != finalAns[i][j]) cnt++;
                    if(cnt == hintID)
                    {
                        printf("Hint: position (%d, %d) should be %d\n", i + 1, j + 1, finalAns[i][j]);
                        base[i][j] = finalAns[i][j];
                        grid[i][j] = finalAns[i][j];
                        PrintGame(grid, base, i + 1, j + 1);
                        break;
                    }
                }
            continue;
        }
        row = var / 100;
        col = (var / 10) % 10;
        num = var % 10;

        
        if(row < 1 || row > 9 || col < 1 || col > 9 || num < 0 || num > 9) 
        {
            printf("Invalid input! Please input again!\n");
            continue;
        }
        if(base[row - 1][col - 1]) 
        {
            printf("This position is fixed, you can't change it!\n");
            continue;
        }

        bool isOriCorrect = (grid[row - 1][col - 1] == finalAns[row - 1][col - 1]);
        grid[row - 1][col - 1] = num;
        bool isNowCorrect = (grid[row - 1][col - 1] == finalAns[row - 1][col - 1]);

        if(isOriCorrect && !isNowCorrect) remainNum++;
        else if(!isOriCorrect && isNowCorrect) remainNum--;

        PrintGame(grid, base);
        winFlag = Sudoku::CheckLegal(grid, isM) == 2 ? true : false;
    }
    printf("Congratulations! You win!\n");
    return ;
}

void Sudoku::PrintAns() const
{
    PrintGame(finalAns, base);
    return ;
}

// print the game grid
// base[i][j] == true indicates that a[i][j] is a given number
// (x, y) will be highlighted if provided
void Sudoku::PrintGame(const int a[9][9], const int _base[9][9], int x, int y) 
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
            if(i + 1 == x && j + 1 == y)
            {
                printColor(to_string(a[i][j]) + " ", "yellow");
            }
            else if(_base[i][j]) printf("%d ", a[i][j]);
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

void Sudoku::PrintGrid() const
{
    PrintGame(grid, grid);
    return ;
}

// generate a sudoku puzzle with remain numbers remain
// it will cover existing puzzle
void Sudoku::GenerateSudokuPuzzle(int _remain, bool _isM)
{
    Clear();

    static int nums[9];
    for(int i = 0; i < 9; ++i) nums[i] = i + 1;

    isM = _isM;
    remainNum = _remain;
    bool flag = false; // if a legal sudoku final map is generated
    int cnt = 0; // count of attempts
    
    if(isM)// It firstly generate two boxes with "%" constraints, then complete the sudoku
    {
        printf("Generating %% sudoku puzzle with %d numbers remain...\n", _remain);
        do
        {
            memset(grid, 0, sizeof(grid));
            shuffle(nums, 9);
        
            for(int i = 0; i < 9; ++i)
            {
                int row = 1 + i / 3, col = 1 + i % 3;
                grid[row][col] = nums[i];
            }
            shuffle(nums, 9);
            for(int i = 0; i < 9; ++i)// 
            {
                int row = 5 + i / 3, col = 5 + i % 3;
                grid[row][col] = nums[i];
            }
            if(grid[3][3] == grid[5][5]) Swap(grid[3][3], grid[1][1]);
        } while (!(flag = SolveSudoku(grid, finalAns, _isM)) && ++cnt <= MAX_ATTEMPT);
    }
    else // It firstly generate the middle and the left up box, then complete the sudoku
    {
        printf("Generating standard sudoku puzzle with %d numbers remain...\n", _remain);
        do
        {
            memset(grid, 0, sizeof(grid));
            shuffle(nums, 9);
        
            for(int i = 0; i < 9; ++i)
            {
                int row = 1 + i / 3, col = 1 + i % 3;
                grid[row][col] = nums[i];
            }
            shuffle(nums, 9);
            for(int i = 0; i < 9; ++i)// 
            {
                int row = 4 + i / 3, col = 4 + i % 3;
                grid[row][col] = nums[i];
            }
        } while (!(flag = SolveSudoku(grid, finalAns, _isM)) && ++cnt <= MAX_ATTEMPT);
    }
    if (!flag)
    {
        printf("Failed to generate a legal sudoku puzzle after %d attempts.\n", MAX_ATTEMPT);
        printf("Please try again.\n");
        return ;
    }

    Lasvegas(finalAns, grid, _remain, _isM);

    for(int i = 0; i < 9; ++i)
        for(int j = 0; j < 9; ++j)
            base[i][j] = grid[i][j];

    return ;
}

// dig holes randomly in finalMap[][] and ensure unique solution
// the result will be stored in target[][]
void Sudoku::Lasvegas(const int finalMap[9][9], int target[9][9], int _remain, bool isM) 
{
    // printf("In Lasvegas, remain: %d \n", _remain);
    int row, col, num;
    int cnt = 0;
    int temp[9][9];
    for(int i = 0; i < 9; ++i)
        for(int j = 0; j < 9; ++j)
            target[i][j] = finalMap[i][j];
    while(cnt < 81 - _remain) 
    {
        row = rand() % 9;
        col = rand() % 9;
        if(!target[row][col]) continue;
        num = target[row][col];
        for(int i = 1; i <= 9; ++i) 
        {
            if(i == num)continue;
            target[row][col] = i;
            if(!CheckLegal(target, isM))continue;
            if(SolveSudoku(target, temp, isM))break;
        }
        target[row][col] = 0;
        cnt++;
    }

    return ;
}

void Sudoku::SetPuzzle()
{
    const string puzzleFile = gRootPath + "/restore/Puzzles.txt";
    const string puzzleTypeMenu[] = 
    {
        "Create standard sodoku puzzle", 
        "Create % sudoku puzzle",
        "Back to main menu",
        ""
    };
    const string completeEditingMenu[] = 
    {
        "Finish editing and save the puzzle",
        "Continue editing",
        "Play it now",
        ""
    };

    Clear();

    int choose = ChooseOptions(puzzleTypeMenu);
    if(choose == 3) return ;
    isM = (choose == 2 ? true : false);
    if(isM) printf("Creating %% sudoku form.\n");
    else printf("Creating standard sudoku form.\n");

    for(int i = 0; i < 9; ++i)
    {
        bool breakFlag = false;
        for(int j = 0; j < 9; ++j)
        {
            PrintGame(grid, grid, i + 1, j + 1);
            printf("Input the number at position (%d, %d), -1 to break: ", i + 1, j + 1);
            int num = chooseNum(-1, 9);
            if(num == -1) {breakFlag = true; break;}
            grid[i][j] = num;
        }
        if(breakFlag) break;
    }

    printf("Your initial input puzzle is:\n");
    PrintGame(grid, grid);
    printf("Now you can make modifications to specific cells.\n");
    printf("Input format: a three-digit number, RowColNum, do not enter Spaces.\n");
    printf("For example, to put number 5 in row 3 column 4, input 345.\n");
    printf("If Num = 0, it means to clear the cell.\n");
    printf("0 to check the current puzzle.\n");
    printf("-1 to give up and return to main menu.\n");
    int row, col, num;
    int var = chooseNum(-1, 999);
    bool completeFlag = false;
    while(!completeFlag)// editing loop
    {
        while(var != 0)
        {
            if(var == -1) return ;

            row = var / 100;
            col = (var / 10) % 10;
            num = var % 10;
            
            if(row < 1 || row > 9 || col < 1 || col > 9 || num < 0 || num > 9) 
            {
                printf("Invalid input! Please input again!\n");
                var = chooseNum(-1, 999);
                continue;
            }
    
            grid[row - 1][col - 1] = num;
            PrintGame(grid, grid, row, col);
            printf("Input your move: ");
            var = chooseNum(-1, 999);
        }
        printf("Checking the current puzzle...\n");
        
        remainNum = 0;
        for(int i = 0; i < 9; ++i)
            for(int j = 0; j < 9; ++j)
            {
                base[i][j] = grid[i][j];
                if(grid[i][j] == 0) remainNum++;
            }
        if(remainNum == 0)
        {
            printf("It's already a complete puzzle!\n");
            printf("Please dig at least one hole!\n");
            printf("Input your move: ");
            var = chooseNum(-1, 999);
            continue;
        }
        else if(remainNum > 61)
        {
            printf("Too many holes! Please ensure at least 20 numbers are given.\n");
            printf("Input your move: ");
            var = chooseNum(-1, 999);
            continue;
        }

        int legalStatus = SolveSudoku(grid, finalAns, isM) ? CheckLegal(grid, isM) : 0;
        if(legalStatus == 0)
        {
            printf("The current puzzle is illegal or has no solution. Please continue modifying.\n");
        }
        else if(legalStatus == 1)
        {
            printf("Input is valid. Save it?: \n");
            int choose = ChooseOptions(completeEditingMenu);
            if(choose != 2)
            {
                canBePlayed = true;

                if(choose == 1)
                {
                    printf("Saving the puzzle...\n");
                    SavePuzzle(puzzleFile);
                    printf("Puzzle saved successfully!\n");
                    completeFlag = true;
                }
                else if(choose == 3)
                {
                    canBePlayed = true;
                    printf("You can play the puzzle now!\n");
                    Play();
                }
                return ;
            }
        }
        else
        {
            printf("It's already a complete puzzle!\n");
            printf("Please dig at least one hole!\n");
        }
        printf("Input your move: ");
        var = chooseNum(-1, 999);
    }
    return ;
}
// for each there are one line to describe a puzzle in the saved puzzle file:
// the first number of the first line is isM (1 for %, 0 for standard)
// the following 81 numbers are the puzzle grid
// and other following 81 numbers are the final answer in the same format
void Sudoku::SavePuzzle(const string fileName) const
{
    freopen(fileName.c_str(), "a", stdout);
    printf("%d ", isM ? 1 : 0);
    for(int i = 0; i < 9; ++i)
        for(int j = 0; j < 9; ++j)
            printf("%d ", grid[i][j]);

    for(int i = 0; i < 9; ++i)
        for(int j = 0; j < 9; ++j)
            printf("%d ", finalAns[i][j]);

    printf("\n");

    freopen("CON", "w", stdout);
    return ;
}

vector<unique_ptr<Sudoku>> LoadPuzzles(const string fileName)
{
    vector<unique_ptr<Sudoku>> puzzles;
    freopen(fileName.c_str(), "r", stdin);
    string line;

    #ifdef DEBUG
    printf("Loading puzzles from file: %s\n", fileName.c_str());
    #endif

    while(getline(cin, line))
    {
        #ifdef DEBUG
        printf("Loading puzzle: %s\n", line.c_str());
        #endif
        
        puzzles.push_back(make_unique<Sudoku>());
        puzzles.back()->ReadPuzzle(line);
    }
    cin.clear(); // clear eof flag
    freopen("CON", "r", stdin);
    return puzzles;
}



