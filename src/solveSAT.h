#pragma once
#include "CNFList.h"

bool DPLLLauncher(int ans[], CNFList *cnf, int &branchTime, int method = 2);// ans is the status of variables. 1 for true, -1 for false, 0 for unsured
bool DPLL(int ans[], CNFList *cnf, int depth, int method);