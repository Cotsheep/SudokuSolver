#include "global.h"

void Restore(CNFList *cnf, Queue <ClauseNode*> &clauseQ, Queue <LiteralNode*> &literalQ)
{
    while(!literalQ.empty())
    {
        LiteralNode *tempLiter = literalQ.front();
        cnf->reinsert(tempLiter);
        literalQ.pop();
    }
    while(!clauseQ.empty())
    {
        ClauseNode *tempClause = clauseQ.front();
        cnf->reinsert(tempClause);
        clauseQ.pop();
    }
    return ;
}

bool DPLL(int ans[], CNFList *cnf, int depth)
{
    // printf("\n\ndfs depth:%d\n", depth);
    // printf("DPLL called. clauseNum: %d, varNum: %d, unitClauseNum: %d\n", cnf->clauseNum, cnf->varNum, cnf->unitClauseNum);
    // cnf->printCNFList();
    // printf("5:\n");
    // cnf->printLiteralList(5);
    // cnf->printLiteralList();
    // putchar(10);

    Queue <ClauseNode*> clauseQ;
    Queue <LiteralNode*> literalQ;
    Queue <ClauseNode*> unitClauseQ;
    // ClauseNode* clauseQ[5000];
    // LiteralNode* literalQ[5000];
    

    ClauseNode *pClause;
    LiteralNode *pLiteral;
    ClauseNode *tempClause;
    LiteralNode *tempLiteral;

    pClause = cnf->clauseHead;
    while(pClause)
    {
        if(pClause->num == 1) unitClauseQ.push(pClause);
        pClause = pClause->nextt;
    }

    while(!unitClauseQ.empty())
    {
        pClause = unitClauseQ.front();
        unitClauseQ.pop();
        if(!pClause->inCNFList) continue;
        // pClause = cnf->clauseHead;
        // pLiteral = NULL;
        // tempClause = NULL;
        // tempLiteral = NULL;
        // while(pClause)
        // {
        //     if(pClause->num == 1) 
        //     {
        //         // printf("unit clause found: %d\n", pClause->first->varIndex * (pClause->first->sign ? 1 : -1));
        //         break;
        //     }
        //     pClause = pClause->nextt;
        // }
        if(!pClause) // should not happen
        {
            // printf("Error: unitClauseNum is f**king wrong\n");
            printf("Error: the clause in unitQ is f**king error\n");
            return false;
        }
        int var = pClause->first->varIndex;
        bool sign = pClause->first->sign;
        ans[var] = sign ? 1 : -1;

        pLiteral = cnf->literalList[var];

        // if(var == 5)
        // {
        //     cnf->printLiteralList(5);
        // }

        while(pLiteral)
        {
            tempLiteral = pLiteral;
            pLiteral = pLiteral->nextPal;

            if(tempLiteral->nextPal == cnf->literalList[var])
            {
                printf("Error: Cycle!!! %d\n", tempLiteral->varIndex);
                // printf("%d %d", tempLiteral->varIndex, cnf->literalList[var]->varIndex);
                exit(0);
            }
            if(tempLiteral->nextPal == tempLiteral)
            {
                printf("Error: point to itself!\n");
                exit(0);
            }
            if(tempLiteral->varIndex != var)
            {
                printf("Error: WTF!!\n");
                exit(0);
            }

            if(!tempLiteral->inCNFList || !tempLiteral->belongClause->inCNFList) 
            {
                continue;
            }

            tempClause = tempLiteral->belongClause;
            if(tempLiteral->sign == sign)
            {
                if(tempClause->inCNFList)
                {
                    cnf->pullOut(tempClause);
                    clauseQ.push(tempClause);

                }
            }
            else
            {
                if(tempClause->num == 1)
                {
                    Restore(cnf, clauseQ, literalQ);
                    return false;
                }
                
                cnf->pullOut(tempLiteral);
                literalQ.push(tempLiteral);

                if(tempClause->num == 1)
                {
                    unitClauseQ.push(tempClause);
                }
            }
        }

        // printf("After unit propagation, clauseNum: %d, varNum: %d, unitClauseNum: %d\n", cnf->clauseNum, cnf->varNum, cnf->unitClauseNum);
        // cnf->printLiteralList(5);
        // putchar(10);

    }


    if(!cnf->clauseHead)
        return true;

    // choose a literal to branch
    pClause = cnf->clauseHead;
    pLiteral = pClause->first;
    int var = pLiteral->varIndex;
    bool sign = pLiteral->sign;

    ClauseNode *newUnitClause = new ClauseNode();
    newUnitClause->initUnitClause(var, sign);
    cnf->addClause(newUnitClause);

    ans[var] = sign ? 1 : -1;

    if(DPLL(ans, cnf, depth + 1))
    {
        return true;
    }
    else
    {
        newUnitClause->first->sign = !sign; // change the sign of the existing unit clause

        ans[var] = sign ? -1 : 1;

        // printf("the first try of %d faild, backtracking now.", var);

        if(DPLL(ans, cnf, depth + 1))
        {
            return true;
        }
        else
        {
            Restore(cnf, clauseQ, literalQ);


            // printf("ALL FAILED, new clause %d in %p deleted.\n",
            // newUnitClause->first->varIndex, newUnitClause);


            cnf->DeleteClause(newUnitClause);

            // printf("after deletion:\n");
            // cnf->printLiteralList(5);

            return false;
        }
    }
    printf("Error: should f**king not reach here in DPLL bottom\n");
    return false;
}
/*

used to using this sample to debug #2.4
c
p cnf 6 5
1 2 0
5 6 0
-5 6 0
-5 -6 0
5 -6 0
*/

