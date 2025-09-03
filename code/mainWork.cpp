#include "global.h"

#define MAXN 50005
int main() 
{
//	 freopen("D://code//C++//test.out", "w", stdout);
	string testIn[3];
	testIn[0] = "D:\\code\\C++\\test.in";
	testIn[1] = "D:\\code\\C\\CNFtests\\sat-20.cnf";
	testIn[2] = "D:\\code\\C\\CNFtests\\unsat-5cnf-30.cnf";
	CNFList *testCNF = new CNFList();
	testCNF->buildCNFList(testIn[0]);
	CNFList *copyCNF = new CNFList();
	copyCNF->copyCNFList(testCNF);
	int ans[MAXN] = {0};
	printf("SAT\n");
	// testCNF->printCNFList();
	printf("could find answer: %s\n", DPLL(ans, testCNF, 1) ? "YES" : "NO");
	printf("varNum: %d\n", testCNF->varNum);
	for(int i = 1; i <= testCNF->varNum; i++)
		printf("%d ", ans[i]);

	putchar(10);
	copyCNF->checkSAT(ans);

	return 0;
}
