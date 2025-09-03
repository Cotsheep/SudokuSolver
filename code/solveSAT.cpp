#include "global.h"

#define MAXN 50005

bool DPLL(int ans[], CNFList *cnf, int depth)
{
//    printf("dfs depth:%d\n", depth);
    // printf("DPLL called. clauseNum: %d, varNum: %d, unitClauseNum: %d\n", cnf->clauseNum, cnf->varNum, cnf->unitClauseNum);
    // cnf->printCNFList();
    // putchar(10);

    ClauseNode *pClause;
    LiteralNode *pLiteral;
    ClauseNode *tempClause;
    LiteralNode *tempLiteral;
    while(cnf->unitClauseNum)
    {
        pClause = cnf->clauseHead;
        pLiteral = NULL;
        tempClause = NULL;
        tempLiteral = NULL;
        while(pClause)
        {
            if(pClause->num == 1) 
            {
                // printf("unit clause found: %d\n", pClause->first->varIndex);
                break;
            }
            pClause = pClause->nextt;
        }
        if(!pClause) // should not happen
        {
            printf("Error: unitClauseNum is fucking wrong\n");
            return false;
        }
        int var = pClause->first->varIndex;
        bool sign = pClause->first->sign;
        ans[var] = sign ? 1 : -1;

        // delete clauses containing this literal
        pClause = cnf->clauseHead;
        pLiteral = NULL;
        while(pClause)//Clause cycle
        {
            tempClause = pClause;
            pClause = pClause->nextt;
            
            if(tempClause->num == 0)// delete empty clause
            {
                cnf->DeleteClause(tempClause);
                continue;
            }
            
            pLiteral = tempClause->first;
            while(pLiteral)//Literal cycle
            {
                tempLiteral = pLiteral;
                pLiteral = pLiteral->nextt;
                if(tempLiteral->varIndex == var)
                {
                    if(tempLiteral->sign == sign)
                    {
                        // printf("clause will be deleted: ");
                        
                        cnf->DeleteClause(tempClause);

                        // cnf->printCNFList();
                        // putchar(10);

                        break;
                    }
                    else
                    {
                        if(tempClause->num == 1)
                            return false;
                        
                        cnf->DeleteLiteral(tempLiteral);
                        continue;
                    }
                }
            }
        }
    }

    // #ifdef DEBUG
    // return true;
    // #endif
    // printf("after unit propagation, clauseNum: %d, varNum: %d, unitClauseNum: %d\n", cnf->clauseNum, cnf->varNum, cnf->unitClauseNum);
    // cnf->printCNFList();

    if(!cnf->clauseHead)
        return true;

    // choose a literal to branch
    pClause = cnf->clauseHead;
    pLiteral = pClause->first;
    int var = pLiteral->varIndex;
    bool sign = pLiteral->sign;
    // printf("branching on %d\n", var);


    CNFList *cnfCopy = new CNFList();//remember to edit member's pointer in copys!!
    cnfCopy->copyCNFList(cnf);
    ClauseNode *newUnitClause = new ClauseNode();
    newUnitClause->initUnitClause(var, sign);
    cnfCopy->addClause(newUnitClause);


    ans[var] = sign ? 1 : -1;
    if(DPLL(ans, cnfCopy, depth + 1))
    {
        return true;
    }
    else
    {
        delete cnfCopy;

        // printf("backtracking on %d\n", var);

        CNFList *cnfCopy2 = new CNFList();
        cnfCopy2->copyCNFList(cnf);
        ClauseNode *newUnitClause2 = new ClauseNode();
        newUnitClause2->initUnitClause(var, !sign);
        cnfCopy2->addClause(newUnitClause2);
        // printf("copy2 done\n");
        ans[var] = sign ? -1 : 1;
        if(DPLL(ans, cnfCopy2, depth + 1))
        {
            return true;
        }
        else
        {
            delete cnfCopy2;
            return false;
        }
    }

}
