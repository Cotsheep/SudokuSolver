#pragma once
#include "solveSAT.h"
using namespace std;

class Sudoku
{
private:

    int grid[9][9];
    int finalAns[9][9];
    int base[9][9];
    int remainNum; // number of remaining WRONG cells
    
private:

    static void ToCNFFile(const int _a[9][9], const string fileName, bool _isM);
    static bool SolveSudoku(const int _a[9][9], int _finalAns[9][9], bool isM);
    static void Lasvegas(const int a[9][9], int target[9][9], int _remain, bool isM);
    
public:

    bool isM;
    bool canBePlayed;

public:

    Sudoku() = delete;
    Sudoku(int _remain, bool _isM);
    Sudoku(const Sudoku &other) = delete;
    static int transToVar(int row, int col, int num);
    static void transToPos(int var, int &row, int &col, int &num);
    int operator()(int row, int col) const;
    void SetCell(int row, int col, int val);
    static int CheckLegal(const int a[9][9], bool isM);
    static bool CheckComplete(const int a[9][9]);
    static void ReadRes(const int ans[], int _finalAns[9][9]);
    void Play();
    static void PrintGame(const int a[9][9], const int _base[9][9], int x = 0, int y = 0);
    void PrintAns() const;
    void GenerateSudokuPuzzle(int _remain, bool _isM);
};