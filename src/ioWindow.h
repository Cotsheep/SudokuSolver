#pragma once
#include "CNFList.h"
#include "generateSudokuPuzzle.h"

void printGame(const int a[9][9], const int base[9][9]);
void playSudoku(int a[9][9], const int ans[9][9], bool isM);
int ChooseOptions(const string menu[]);
int chooseNum(int start, int end);