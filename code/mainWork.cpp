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
	//  freopen("D:\\code\\C\\SATsolver\\testSamples\\output.out", "w", stdout);
	string testIn[10];
	testIn[0] = "D:\\code\\C\\SATsolver\\testSamples\\input.in";
	testIn[1] = "D:\\code\\C\\SATsolver\\testSamples\\sat\\sat-20.cnf";
	testIn[2] = "D:\\code\\C\\SATsolver\\testSamples\\unsat\\unsat-5cnf-30.cnf";
	testIn[3] = "D:\\code\\C\\SATsolver\\testSamples\\sat\\ais10.cnf";
	testIn[4] = "D:\\code\\C\\SATsolver\\testSamples\\sat\\problem1-20.cnf";
	testIn[5] = "D:\\code\\C\\SATsolver\\testSamples\\sat\\problem2-50.cnf";
	testIn[6] = "D:\\code\\C\\SATsolver\\testSamples\\sat\\problem3-100.cnf";
	testIn[7] = "D:\\code\\C\\SATsolver\\testSamples\\sat\\problem6-50.cnf";
	testIn[8] = "D:\\code\\C\\SATsolver\\testSamples\\sat\\problem11-100.cnf";
	
	
	CNFList *testCNF = new CNFList();
	testCNF->buildCNFList(testIn[0]);
	CNFList *copyCNF = new CNFList();
	copyCNF->copyCNFList(testCNF);
	int ans[MAXN] = {0};
	printf("SAT\n");
	testCNF->printCNFList();
	 printf("could find answer: %s\n", DPLL(ans, testCNF, 1) ? "YES" : "NO");
	 printf("varNum: %d\n", testCNF->varNum);
	 for(int i = 1; i <= testCNF->varNum; i++)
	 	printf("%d ", ans[i]);
	 putchar(10);
	 copyCNF->checkSAT(ans);


//	Queue <int > test;
//	for(int i = 1; i <= 7; ++i)
//	{
//		test.push(i);
//	}
//	QfuncTest(test);
	// printf("after QfuncTest, size: %d\n", test.size());
	return 0;
}
