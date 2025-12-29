#include <time.h>
#include "global.h"
using namespace std;

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
	"Set sudoku puzzle",
	"Play stored sudoku puzzle",
	"Play sudoku(auto generated)",
	"Exit",
	""
};

const string sudokuFormMenu[] = {
	"Standard Sudoku",
	"% Sudoku",
	"Back to main menu",
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
			unique_ptr<Sudoku> puzzle(new Sudoku());
			puzzle->SetPuzzle();
		}
		else if(choose == 2)
		{
			const string puzzleFile = gRootPath + "\\restore\\Puzzles.txt";
			vector<unique_ptr<Sudoku>> storedPuzzles = LoadPuzzles(puzzleFile);
			if(storedPuzzles.size() == 0)
			{
				printf("No stored puzzles found! Please set puzzles first!\n");
				continue ;
			}
			printf("Loaded %d puzzles from file.\n", (int)storedPuzzles.size());
			for(int i = 1; i <= (int)storedPuzzles.size(); ++i)
			{
				printColor(to_string(i) + ". ", "yellow");
				printf("%s sudoku puzzle\n", storedPuzzles[i - 1]->isM ? "%" : "Standard");
				storedPuzzles[i - 1]->PrintGrid();
				putchar(10);
			}
			printf("Choose a puzzle to play (1 ~ %d), (enter 0 to return to main menu): ", (int)storedPuzzles.size());
			int puzzleNum = chooseNum(0, (int)storedPuzzles.size());
			if(puzzleNum == 0) continue ;
			storedPuzzles[puzzleNum - 1]->Play();
		}
		else if(choose == 3)
		{
			int remain, isM;
			choose = ChooseOptions(sudokuFormMenu);
			if(choose == 3) continue;
			isM = choose == 2 ? true : false;
			if(isM) printf("You chose %% sudoku form.\n");
			else printf("You chose standard sudoku form.\n");

			printf("Choose the number of elements to remain\n(20~80)\nchoose: ");
			remain = chooseNum(20, 80);
			printf("You chose %d clues to remain.\n", remain);
			unique_ptr<Sudoku> puzzle(new Sudoku(remain, isM));
			puzzle->Play();
		}
	}

	return 0;
}
