#pragma once
#include "CNFList.h"
#include "solveSAT.h"
#include <string>
using namespace std;

void generateSudokuPuzzle(int a[9][9], int finalAns[9][9], int remain, bool isM);// generate a Sudoku puzzle which has n numbers remain and store it in a 2D array
int transToVar(int row, int col, int num);
void transToPos(int var, int &row, int &col, int &num);
void SudokuToCNFFile(const int a[9][9], string fileName, bool isM);
int checkLegal(const int a[9][9], bool isM);
void readRes(const int ans[], int a[9][9]);
void DigHoles(int a[9][9], int remain);
void Lasvegas(int a[9][9], int remain, bool isM);
bool solveSudoku(int a[9][9], int res[9][9], bool isM);