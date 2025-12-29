#pragma once
#include "segmentTree.h"
#include "myQueue.h"
#include <filesystem>
#include <windows.h>
#include <iostream>
#include <cstring>
#include <stdio.h>
#include <string>
#include <random>
#include <memory>
#include <ctime>
#define MAXN 10005

const std::string YES_NO_MENU[] = {
    "Yes",
    "No",
    ""
};

extern std::string gRootPath; // global root path of the project
int readInt();
int readInt(std::string s, int &p);
char readChar();
void Swap(int &a, int &b);
void shuffle(int a[], int n);// 0 ~ n-1
void printColor(std::string s, std::string color);
int ChooseOptions(const std::string menu[]);

int chooseNum(int start, int end); // [start, end]
std::string getExePath();
std::string GetRootPath();
