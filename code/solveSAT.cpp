#include "global.h"

#define METHOD 2 // 1 for origin, 2 for SegTree

void Restore(CNFList *cnf, Queue <ClauseNode*> &clauseQ, Queue <LiteralNode*> &literalQ)
{
    while(!literalQ.empty())
    {
        LiteralNode *tempLiter = literalQ.front();
        cnf->reinsert(tempLiter);
        cnf->J->update(cnf->J->root, 
            tempLiter->varIndex, 
            tempLiter->belongClause->num, 2);
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

ClauseNode* clauseS[500000];int clauseTop = 0;
LiteralNode* literS[500000];int literTop = 0;

void Restore2(CNFList *cnf, int clauseBottom, int literBottom)
{
    while(clauseTop > clauseBottom)
    {
        cnf->reinsert(clauseS[clauseTop]);
        clauseTop--;
    }
    while(literTop > literBottom)
    {
        cnf->reinsert(literS[literTop]);
        literTop--;
    }
    return ;
}

int newCnt;

bool DPLLLauncher(int ans[], CNFList *cnf)
{
    clauseTop = 0;
    literTop = 0;
    newCnt = 0;
    bool flag = DPLL(ans, cnf, 1);
    printf("new times: %d\n", newCnt);
    return flag;
}

bool DPLL(int ans[], CNFList *cnf, int depth)
{
    // printf("\n\ndfs depth:%d\n", depth);
    // printf("DPLL called. clauseNum: %d, varNum: %d, unitClauseNum: %d\n",
    //  cnf->clauseNum, cnf->varNum, cnf->unitClauseNum);

    Queue <ClauseNode*> unitClauseQ;
    int clauseBottom = clauseTop;
    int literBottom = literTop;

    ClauseNode *pClause;
    LiteralNode *pLiteral;
    ClauseNode *tempClause;
    LiteralNode *tempLiteral;

    pClause = cnf->clauseHead;
    while(pClause)
    {
        if(pClause->num == 1) unitClauseQ.push(pClause);
        else if(depth > 1)break;
        pClause = pClause->nextt;
    }

    while(!unitClauseQ.empty())
    {
        pClause = unitClauseQ.front();
        unitClauseQ.pop();
        if(!pClause->inCNFList) continue;
        if(!pClause) // should not happen
        {
            printf("Error: the clause in unitQ is f**king error\n");
            return false;
        }
        int var = pClause->first->varIndex;
        bool sign = pClause->first->sign;
        ans[var] = sign ? 1 : -1;

        // printf("deleting variable %d\n", var);

        cnf->J->update(cnf->J->root, var, false, 3);

        pLiteral = cnf->literalList[var];

        while(pLiteral)
        {
            tempLiteral = pLiteral;
            pLiteral = pLiteral->nextPal;

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
                    // cnf->disable(tempClause);
                    clauseS[++clauseTop] = tempClause;
                }
            }
            else
            {
                if(tempClause->num == 1)
                {
                    Restore2(cnf, clauseBottom, literBottom);
                    // cnf->J->update(cnf->J->root, var, true, 3);
                    return false;
                }
                
                cnf->pullOut(tempLiteral);
                // cnf->disable(tempLiteral);
                literS[++literTop] = tempLiteral;

                if(tempClause->num == 1)
                {
                    unitClauseQ.push(tempClause);
                }
            }
        }
    }

    // cnf->J->printTree();


    if(!cnf->clauseHead)
        return true;

    // choose a literal to branch
    int var;
    bool sign;
    if(METHOD == 1)// origin method
    {
        pClause = cnf->clauseHead;
        pLiteral = pClause->first;
        var = pLiteral->varIndex;
        sign = pLiteral->sign;
    }
    else if(METHOD == 2)// SegTree method
    {
        var = cnf->J->query();
        if(var == -1)
        {
            printf("Error: could not find a variable to branch.\n");
            // putchar(10);
	        // cnf->J->printTree();
            return false;
        }
        pLiteral = cnf->literalList[var];
        while(pLiteral && !pLiteral->inCNFList)
            pLiteral = pLiteral->nextPal;
        
        if(!pLiteral)
        {
            printf("Error: could not find a literal to branch on.\n");
            return false;
        }
        sign = pLiteral->sign;
    }
    // end choose

    ClauseNode *newUnitClause = new ClauseNode();
    newCnt++;

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
            Restore2(cnf, clauseBottom, literBottom);
            cnf->DeleteClause(newUnitClause);

            cnf->J->update(cnf->J->root, var, true, 3);
            // printf("%d recoverd\n", var);
            return false;
        }
    }
    printf("Error: should f**king not reach here in DPLL bottom\n");
    return false;
}

