#pragma once
#include "solveSAT.h"
#include "solveSudoku.h"
#include <string>
using namespace std;

void generateSudokuPuzzle(int a[9][9], int finalAns[9][9], int remain, bool isM);// generate a Sudoku puzzle which has n numbers remain and store it in a 2D array
void DigHoles(int a[9][9], int remain);
void Lasvegas(int a[9][9], int remain, bool isM);
