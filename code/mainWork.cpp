#include <time.h>
#include "global.h"

void QfuncTest(Queue <int > &q)// attention!! the pointers in q will be the same as the original queue, so add &
{
	for(int i = 1; i <= 7; ++i)
	{
		printf("%d ", q.front());
		q.pop();
	}
	return ;
}

int main() 
{
	srand(time(nullptr));
	//   freopen("D:\\code\\C\\SATsolver\\testSamples\\output.out", "w", stdout);
	string testIn[100];
	string fileHead = "D:\\code\\C\\SATsolver\\TEST\\";
	string SATsolution = "D:\\code\\C\\SATsolver\\TEST\\solution.txt";
	testIn[1] = fileHead + "1.cnf";
	testIn[2] = fileHead + "2.cnf";
	testIn[3] = fileHead + "3.cnf";
	testIn[4] = fileHead + "4-u.cnf";
	testIn[5] = fileHead + "5.cnf";	
	testIn[6] = fileHead + "6.cnf";// solved
	testIn[7] = fileHead + "7-u.cnf";
	testIn[8] = fileHead + "8-u.cnf";
	testIn[9] = fileHead + "9-u.cnf";
	testIn[10] = fileHead + "10.cnf";
	testIn[11] = fileHead + "11-u.cnf";// solved
	testIn[12] = fileHead + "12.cnf";

	int choose;

	while((choose = chooseMainMenu()) != 3)
	{
		if(choose == 1)
		{
			int testFileNum, method;
			printf("Choose a test file(1~12): ");
			testFileNum = chooseNum(1, 12);
			printf("Choose a method(1 for origin, 2 for SegTree): ");
			method = chooseNum(1, 2);
			CNFList *cnf = new CNFList(), *cnfCopy = new CNFList();
			cnf->buildCNFList(testIn[testFileNum]);
			cnfCopy->copyCNFList(cnf);
			int ans[MAXN] = {0};
			memset(ans, 0, sizeof(ans));
			int branchTime = 0;
			clock_t startTime = clock();
			printf("Solving...\n");
			freopen(SATsolution.c_str(), "w", stdout);
			bool flag = DPLLLauncher(ans, cnf, branchTime, method);
			if(flag)
			{
				printf("s 1\n");
				printf("v ");
				for(int i = 1; i <= cnf->varNum; ++i)
				{
					if(ans[i] == 1) printf("%d ", i);
					else if(ans[i] == -1) printf("%d ", -i);
					else printf("0 ");
				}
				putchar(10);
			}
			else
			{
				printf("s 0\n");
			}
			clock_t endTime = clock();
			int timeUsed = (int)(endTime - startTime);
			printf("t %d ms\n", timeUsed);
			printf("Branch times: %d\n", branchTime);
			if(flag)cnfCopy->checkSAT(ans);
			freopen("CON", "w", stdout);
			delete cnf;
			delete cnfCopy;
			printf("Solution written in %s\n", SATsolution.c_str());
		}
		else if(choose == 2)
		{
			int remain;
			int sudoku[9][9] = {0};
			int finalAns[9][9] = {0};
			printf("Choose the number of elements to remain\n(14~80, if less than 20, it may has multiple solutions)\nchoose: ");
			remain = chooseNum(14, 80);
			printf("You chose %d clues to remain.\n", remain);
			generateSudokuPuzzle(sudoku, finalAns, remain);
			playSudoku(sudoku, finalAns);
		}
	}

	return 0;
}
