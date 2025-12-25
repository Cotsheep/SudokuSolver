#pragma once
#include "generateSudokuPuzzle.h"
#include "solveSudoku.h"

// #define DEBUG

#define MAX_ATTEMPT 1000

// generate a sudoku puzzle with remain numbers remain
// the puzzle is stored in a[][], and the final answer is stored in finalAns[][]
void generateSudokuPuzzle(int a[9][9], int finalAns[9][9], int remain, bool isM) 
{
    int nums[9];
    for(int i = 0; i < 9; ++i) nums[i] = i + 1;

    int finalMap[9][9] = {0}; // as a temporary full sudoku map, then move numbers to a[][] from it
    bool flag = false; // if a legal sudoku final map is generated
    int cnt = 0; // count of attempts
    
    if(isM)// It firstly generate two boxes with "%" constraints, then complete the sudoku
    {
        printf("Generating %% sudoku puzzle with %d numbers remain...\n", remain);
        do
        {
            memset(finalMap, 0, sizeof(finalMap));
            shuffle(nums, 9);
        
            for(int i = 0; i < 9; ++i)
            {
                int row = 1 + i / 3, col = 1 + i % 3;
                finalMap[row][col] = nums[i];
            }
            shuffle(nums, 9);
            for(int i = 0; i < 9; ++i)// 
            {
                int row = 5 + i / 3, col = 5 + i % 3;
                finalMap[row][col] = nums[i];
            }
            if(finalMap[3][3] == finalMap[5][5]) Swap(finalMap[3][3], finalMap[1][1]);
        } while (!(flag = solveSudoku(finalMap, finalMap, isM)) && ++cnt <= MAX_ATTEMPT);
    }
    else // It firstly generate the middle and the left up box, then complete the sudoku
    {
        printf("Generating standard sudoku puzzle with %d numbers remain...\n", remain);
        do
        {
            memset(finalMap, 0, sizeof(finalMap));
            shuffle(nums, 9);
        
            for(int i = 0; i < 9; ++i)
            {
                int row = 1 + i / 3, col = 1 + i % 3;
                finalMap[row][col] = nums[i];
            }
            shuffle(nums, 9);
            for(int i = 0; i < 9; ++i)// 
            {
                int row = 4 + i / 3, col = 4 + i % 3;
                finalMap[row][col] = nums[i];
            }
        } while (!(flag = solveSudoku(finalMap, finalMap, isM)) && ++cnt <= MAX_ATTEMPT);
    }
    if (!flag)
    {
        printf("Failed to generate a legal sudoku puzzle after %d attempts.\n", MAX_ATTEMPT);
        printf("Please try again.\n");
        return ;
    }

    // int ans[MAXN] = {0};

    for(int i = 0; i < 9; ++i)
        for(int j = 0; j < 9; ++j)
            finalAns[i][j] = finalMap[i][j];

    // printf("finalMap:\n");
    // printGrid(finalMap);
    if(remain >= 20)Lasvegas(finalMap, remain, isM);
    else DigHoles(finalMap, remain); // if remain < 20, Lasvegas may be too slow

    for(int i = 0; i < 9; ++i)
        for(int j = 0; j < 9; ++j)
            a[i][j] = finalMap[i][j];

    return ;
}

// dig holes randomly in a[][] and ensure unique solution
void Lasvegas(int a[9][9], int remain, bool isM) 
{
    int row, col, num;
    int cnt = 0;
    int temp[9][9];
    while(cnt < 81 - remain) 
    {
        row = rand() % 9;
        col = rand() % 9;
        if(!a[row][col]) continue;
        num = a[row][col];
        for(int i = 1; i <= 9; ++i) 
        {
            if(i == num)continue;
            a[row][col] = i;
            if(!checkLegal(a, isM))continue;
            if(solveSudoku(a, temp, isM))break;
        }
        a[row][col] = 0;
        cnt++;
    }
    remain = 81 - cnt;
}

// dig holes randomly in a[][]
void DigHoles(int a[9][9], int remain) 
{
    int row, col;
    int cnt = 0;
    while(cnt < 81 - remain) 
    {
        row = rand() % 9;
        col = rand() % 9;
        if(!a[row][col]) continue;
        a[row][col] = 0;
        cnt++;
    }
    remain = 81 - cnt;
    return ;
}


/*
id of each 3 * 3 box
0 1 2
3 4 5
6 7 8
*/


