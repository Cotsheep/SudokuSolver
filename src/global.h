// tools.h -> CNFList.h -> globle.h 

#pragma once
#include "CNFList.h"

//generateSudokuPuzzle
void generateSudokuPuzzle(int a[9][9], int finalAns[9][9], int remain, bool isM);// generate a Sudoku puzzle which has n numbers remain and store it in a 2D array
int transToVar(int row, int col, int num);
void transToPos(int var, int &row, int &col, int &num);
void SudokuToCNFFile(const int a[9][9], string fileName, bool isM);
int checkLegal(const int a[9][9], bool isM);
void readRes(const int ans[], int a[9][9]);
void DigHoles(int a[9][9], int remain);
void Lasvegas(int a[9][9], int remain, bool isM);
bool solveSudoku(int a[9][9], int res[9][9], bool isM);
//solveSAT
bool DPLLLauncher(int ans[], CNFList *cnf, int &branchTime, int method = 2);// ans is the status of variables. 1 for true, -1 for false, 0 for unsured
bool DPLL(int ans[], CNFList *cnf, int depth, int method);

//ioWindow
void printGame(const int a[9][9], const int base[9][9]);
void playSudoku(int a[9][9], const int ans[9][9], bool isM);
int ChooseOptions(const string menu[]);
int chooseNum(int start, int end);
