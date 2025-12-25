#pragma once
#include "solveSAT.h"

bool solveSudoku(int a[9][9], int res[9][9], bool isM);
int transToVar(int row, int col, int num);
void transToPos(int var, int &row, int &col, int &num);
void SudokuToCNFFile(const int a[9][9], string fileName, bool isM);
int checkLegal(const int a[9][9], bool isM);
void readRes(const int ans[], int a[9][9]);