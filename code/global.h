// tools.h -> CNFList.h -> globle.h 

#pragma once
#include "CNFList.h"

//generateSudokuPuzzle
void generateSudokuPuzzle(int a[9][9], int finalAns[9][9], int n);// generate a Sudoku puzzle which has n numbers remain and store it in a 2D array
void printGrid(int a[9][9]);
void printGame(int a[9][9], int base[9][9]);
int transToVar(int row, int col, int num);
void transToPos(int var, int &row, int &col, int &num);
void sudokuToCNFFile(int a[9][9], string fileName);
int checkLegal(int a[9][9]);// 0 for illegal, 1 for legal, 2 for solved
void readRes(int ans[], int a[9][9]);
void LasvegasW(int a[9][9], int remain);
void LasvegasDFS(int a[9][9], int remain);
bool solveSudoku(int a[9][9], int res[9][9], int debug = 0);
//solveSAT
bool DPLLLauncher(int ans[], CNFList *cnf, int &branchTime, int method = 2);// ans is the status of variables. 1 for true, -1 for false, 0 for unsured
bool DPLL(int ans[], CNFList *cnf, int depth, int method);

//ioWindow
void playSudoku(int a[9][9], int ans[9][9]);
int chooseMainMenu();
int chooseNum(int start, int end);
