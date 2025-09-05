#include "global.h"

#define MAXN 50005

void Restore(CNFList *cnf, Queue <ClauseNode*> &clauseQ, Queue <LiteralNode*> &literalQ, int pos = 0)
{
    // printf("Restoring(%d)...\n", pos);
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
    // printf("Restoring done.\n");
    // cnf->printCNFList();
    // printf("restored print done.\n");
    // putchar(10);
    return ;
}

bool DPLL(int ans[], CNFList *cnf, int depth)
{
    // printf("\n\n\ndfs depth:%d\n", depth);
    // printf("DPLL called. clauseNum: %d, varNum: %d, unitClauseNum: %d\n", cnf->clauseNum, cnf->varNum, cnf->unitClauseNum);
    // cnf->printCNFList();
    // putchar(10);

    Queue <ClauseNode*> clauseQ;
    Queue <LiteralNode*> literalQ;

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
            printf("Error: unitClauseNum is f**king wrong\n");
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
            
            if(tempClause->num == 0)//empty clause
            {
                // cnf->DeleteClause(tempClause);
                Restore(cnf, clauseQ, literalQ, 1);
                return false;
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
                        
                        // cnf->DeleteClause(tempClause);
                        cnf->pullOut(tempClause);
                        clauseQ.push(tempClause);

                        // cnf->printCNFList();
                        // putchar(10);

                        break;
                    }
                    else
                    {
                        if(tempClause->num == 1)
                        {
                            Restore(cnf, clauseQ, literalQ, 2);
                            // printf("found empty clause after unit propagation\n");
                            return false;
                        }
                        
                        // cnf->DeleteLiteral(tempLiteral);
                        cnf->pullOut(tempLiteral);
                        literalQ.push(tempLiteral);

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


    // CNFList *cnfCopy = new CNFList();//remember to edit member's pointer in copys!!
    // cnfCopy->copyCNFList(cnf);
    ClauseNode *newUnitClause = new ClauseNode();
    newUnitClause->initUnitClause(var, sign);
    // cnfCopy->addClause(newUnitClause);
    cnf->addClause(newUnitClause);

    ans[var] = sign ? 1 : -1;

    // printf("starting dfs\n");

    if(DPLL(ans, cnf, depth + 1))
    {
        // printf("found satisfiable on %d\n", var);

        // printf("true\n");

        return true;
    }
    else
    {
        // delete cnfCopy;
        // printf("backtracking\n");
        // printf("backtracking on %d\n", var);
        // return false;
        // CNFList *cnfCopy2 = new CNFList();
        // cnfCopy2->copyCNFList(cnf);
        // ClauseNode *newUnitClause2 = new ClauseNode();
        // newUnitClause2->initUnitClause(var, !sign);
        // cnfCopy2->addClause(newUnitClause2);
        // printf("copy2 done\n");

        newUnitClause->first->sign = !sign; // change the sign of the existing unit clause

        ans[var] = sign ? -1 : 1;
        if(DPLL(ans, cnf, depth + 1))
        {
            return true;
        }
        else
        {
            // delete cnfCopy2;
            Restore(cnf, clauseQ, literalQ, 3);
            cnf->DeleteClause(newUnitClause);
            return false;
        }
    }
    
    // if DPLL reach here, something went f**king wrong
    printf("Error: should not reach here in DPLL bottom\n");
    return false;
}


