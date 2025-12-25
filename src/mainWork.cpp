#include <time.h>
#include "global.h"
#include <iostream>

// #define DEBUG

string gRootPath = GetRootPath();

void QfuncTest(Queue <int > &q)// attention!! the pointers in q will be the same as the original queue, so add &
{
	for(int i = 1; i <= 7; ++i)
	{
		printf("%d ", q.front());
		q.pop();
	}
	return ;
}

const string mainMenu[] = {
	"Set standard sudoku puzzle",
	"Play stored sudoku puzzle",
	"Play sudoku(auto generated)",
	"Exit",
	""
};

const string sudokuFormMenu[] = {
	"Standard Sudoku",
	"% Sudoku",
	""
};

int main() 
{
	
	srand(time(nullptr));
	// std::cout << fs::current_path() << std::endl;
	//   freopen("D:\\code\\C\\SATsolver\\testSamples\\output.out", "w", stdout);
	// string testIn[100];
	// string fileHead = "test/";
	// string SATsolution = "test/solution.txt";
	// testIn[1] = fileHead + "1.cnf";
	// testIn[2] = fileHead + "2.cnf";
	// testIn[3] = fileHead + "3.cnf";
	// testIn[4] = fileHead + "4-u.cnf";
	// testIn[5] = fileHead + "5.cnf";	
	// testIn[6] = fileHead + "6.cnf";// solved
	// testIn[7] = fileHead + "7-u.cnf";
	// testIn[8] = fileHead + "8-u.cnf";
	// testIn[9] = fileHead + "9-u.cnf";
	// testIn[10] = fileHead + "10.cnf";
	// testIn[11] = fileHead + "11-u.cnf";// solved
	// testIn[12] = fileHead + "12.cnf";

	int choose;


	while((choose = ChooseOptions(mainMenu)) != 4)
	{
		if(choose == 1)
		{
			
		}
		else if(choose == 2)
		{

		}
		else if(choose == 3)
		{
			int remain, isM;
			int sudoku[9][9] = {0};
			int finalAns[9][9] = {0};

			isM = ChooseOptions(sudokuFormMenu) == 2 ? true : false;
			if(isM) printf("You chose %% sudoku form.\n");
			else printf("You chose standard sudoku form.\n");

			printf("Choose the number of elements to remain\n(14~80, if less than 20, it may has multiple solutions)\nchoose: ");
			remain = chooseNum(14, 80);
			printf("You chose %d clues to remain.\n", remain);
			generateSudokuPuzzle(sudoku, finalAns, remain, isM);
			playSudoku(sudoku, finalAns, isM);
			
		}
	}

	return 0;
}
