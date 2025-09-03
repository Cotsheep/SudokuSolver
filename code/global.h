#pragma once
#include "CNFList.h"

//generateSudokuPuzzle
void generateSudokuPuzzle(int a[9][9], int n);// generate a Sudoku puzzle which has n numbers remain and store it in a 2D array

//solveSAT
bool DPLL(int ans[], CNFList *cnf, int depth = 1);// ans is the status of variables. 1 for true, -1 for false, 0 for unsured
