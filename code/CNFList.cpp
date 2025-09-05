/*
version: 1.0 (present)
method: basic linked list
*/
/*
version: 2.0 (Anticipation)
method: Two Literal Watch
*/
#include "CNFList.h"

LiteralNode::LiteralNode()
{
    belongClause = NULL;
    nextt = NULL;
    pre = NULL;
    sign = 0;
}
LiteralNode::LiteralNode(int _varIndex, LiteralNode *last)
{
    sign = _varIndex > 0 ? 1 : 0;
    varIndex = _varIndex > 0 ? _varIndex : -_varIndex;
    if(last) last -> nextt = this;
    nextt = NULL;
    pre = last;
}
void LiteralNode::Delete()
{
    if(pre) pre->nextt = nextt;
    else if(belongClause) belongClause->first = nextt;
    if(nextt) nextt->pre = pre;
    delete this;
}

ClauseNode::ClauseNode()
{
    num = 0;
    inCNFList = false;
    nextt = NULL;
    first = NULL;
    cnf = NULL;
}
void ClauseNode::initUnitClause(int var, bool sign)
{
    LiteralNode *newLiteral = new LiteralNode(var, NULL);
    newLiteral->sign = sign;
    newLiteral->belongClause = this;
    first = newLiteral;
    cnf = NULL;
    num = 1;
    return ;
}
void ClauseNode::Delete()
{

    // printf("deleting clause: ");
    // print();
    // printf("pre: ");
    // if(pre) pre->print();
    // else printf("NULL\n");
    // printf("next: ");
    // if(nextt) nextt->print();
    // else printf("NULL\n");

    if(first)
    {
        LiteralNode *pLiteral = first;
        LiteralNode *tempLiteral;
        while(pLiteral)
        {
            tempLiteral = pLiteral;
            pLiteral = pLiteral->nextt;
            delete tempLiteral;
        }
    }
    if(pre) pre->nextt = nextt;
    else if(cnf) cnf->clauseHead = nextt;

    // printf("cnf%p's ClauseHead after deletion: ", cnf);
    // if(cnf->clauseHead) cnf->clauseHead->print();
    // else printf("NULL\n");
    // putchar(10);

    if(nextt) nextt->pre = pre;
    delete this;
}
void ClauseNode::print()
{
    LiteralNode *pLiteral = first;
    while(pLiteral)
    {
        if(!pLiteral->sign) putchar('-');
        printf("%d ", pLiteral->varIndex);
        pLiteral = pLiteral->nextt;
    }
    putchar('0');
    putchar(10);
    return ;
}

CNFList::~CNFList()
{
    ClauseNode *pClause = clauseHead;
    ClauseNode *tempClause;
    while(pClause)
    {
        tempClause = pClause;
        pClause = pClause->nextt;
        tempClause->Delete();
    }
}
void CNFList::checkSAT(int ans[])
{
    int line = 0;
    ClauseNode *pClause = clauseHead;
    LiteralNode *pLiteral;
    while(pClause)
    {
        pLiteral = pClause->first;
        bool clauseSatisified = false;
        ++line;
        while(pLiteral)
        {
            if((ans[pLiteral->varIndex] == 1 && pLiteral->sign) || (ans[pLiteral->varIndex] == -1 && !pLiteral->sign))
            {
                clauseSatisified = true;
                break;
            }
            pLiteral = pLiteral->nextt;
        }
        if(!clauseSatisified) 
        {
            printf("WA! Clause not satisified in line %d: ", line);
            pClause->print();
            return ;
        }
        pClause = pClause->nextt;
    }
    printf("AC! Clauses satisified!\n");
    return ;
}
void CNFList::DeleteClause(ClauseNode *clause)
{
    // if(!clause) return ;
    // if(clause->num == 1) unitClauseNum--;
    // clauseNum--;
    
    // printf("deleting clause: ");
    // clause->print();
    // putchar(10);
    
    pullOut(clause);
    clause->Delete();
    
    // printf("cnf%p's ClauseHead after cnflist function deletion: ", this);
    // if(ClauseHead) ClauseHead->print();
    // else printf("NULL\n");
    // putchar(10);
    
    return ;
}
void CNFList::DeleteLiteral(LiteralNode *literal)
{
    // if(!literal) return ;
    // ClauseNode *clause = literal->belongClause;
    // literal->Delete();
    // if(!clause) return ;
    // clause->num--;
    // if(clause->num == 1) unitClauseNum++;
    // return ;
    pullOut(literal);
    literal->Delete();
    return ;
}
void CNFList::addClause(ClauseNode *clause)
{
    if(!clause) return ;
    if(!clause->num) {printf("the adding clause is fking empty\n");return ;}
    clause->cnf = this;
    clause->pre = NULL;
    clause->inCNFList = true;
    clause->nextt = clauseHead;
    if(clauseHead) clauseHead->pre = clause;
    clauseHead = clause;
    clauseNum++;
    if(clause->num == 1) unitClauseNum++;
    return ;
}
void CNFList::pullOut(ClauseNode *clause)
{
    if(clause->cnf != this){printf("this clause's mother isn't me.\n");return ;}
    if(!clause->inCNFList){printf("why doesn't it in cnf list when pulling?");clause->print();putchar(10);return ;}

    if(clause->pre) clause->pre->nextt = clause->nextt;
    else clauseHead = clause->nextt;
    if(clause->nextt) clause->nextt->pre = clause->pre;
    clause->inCNFList = false;

    if(clause->num == 1) unitClauseNum--;
    clauseNum--;
    return ;
}
void CNFList::pullOut(LiteralNode *liter)
{
    ClauseNode *clause = liter->belongClause;
    if(!clause){printf("this liter doesn't have belongClause?!\n"); return ;}
    if(clause->cnf != this){printf("this liter's mother isn't me.\n");return ;}

    if(!clause->inCNFList)return ;// pulling this literal doesn't matter
    //because when restoring the clause, the literal will be restored first

    if(liter->pre) liter->pre->nextt = liter->nextt;
    else if(clause) clause->first = liter->nextt;
    if(liter->nextt) liter->nextt->pre = liter->pre;
    clause->num--;
    if(clause->num == 1) unitClauseNum++;
    else if(clause->num == 0) unitClauseNum--;
    return ;
}
void CNFList::reinsert(ClauseNode *clause)
{
    if(clause->cnf != this){printf("this clause's mother isn't me.\n");return ;}
    clause->pre = NULL;
    clause->nextt = clauseHead;
    if(clauseHead) clauseHead->pre = clause;
    clause->inCNFList = true;

    clauseHead = clause;
    clauseNum++;
    if(clause->num == 1) unitClauseNum++;
    return ;
}
void CNFList::reinsert(LiteralNode *liter)
{
    if(liter->belongClause->cnf != this){printf("this liter's mother isn't me.\n");return ;}
    ClauseNode *clause = liter->belongClause;
    if(!clause){printf("this liter doesn't have belongClause?!\n"); return;}
    liter->pre = NULL;
    liter->nextt = clause->first;
    if(clause->first) clause->first->pre = liter;
    clause->first = liter;
    clause->num++;
    if(clause->inCNFList && clause->num == 1) unitClauseNum++;
    if(clause->inCNFList && clause->num == 2) unitClauseNum--;
    return ;
}


CNFList::CNFList()
{
    clauseHead = NULL;
    clauseNum = 0;
    varNum = 0;
    unitClauseNum = 0;
    return ;
}
void CNFList::clear()
{
    ClauseNode *pClause = clauseHead;
    ClauseNode *tempClause;
    while(pClause)
    {
        tempClause = pClause;
        pClause = pClause->nextt;
        tempClause->Delete();
    }
    clauseHead = NULL;
    clauseNum = 0;
    varNum = 0;
    unitClauseNum = 0;
    return ;
}
void CNFList::copyCNFList(const CNFList *other)// copy
{
    
    // printf("COPYING\n----------------------------\n");
    
    clauseNum = other->clauseNum;
    varNum = other->varNum;
    unitClauseNum = other->unitClauseNum;
    clauseHead = NULL;
    ClauseNode *pClause = other->clauseHead;
    ClauseNode *lastClause = NULL;
    while(pClause)
    {
        ClauseNode *newClause = new ClauseNode();
        newClause->num = pClause->num;
        newClause->cnf = this;

        // printf("copying clause: ");
        // pClause->print();
        // printf("cnf: %p\n", newClause->cnf);
        // putchar(10);

        newClause->pre = lastClause;
        if(lastClause) lastClause->nextt = newClause;
        else clauseHead = newClause;
        lastClause = newClause;

        LiteralNode *pLiteral = pClause->first;
        LiteralNode *lastLiteral = NULL;
        while(pLiteral)
        {
            LiteralNode *newLiteral = new LiteralNode();
            newLiteral->varIndex = pLiteral->varIndex;
            newLiteral->sign = pLiteral->sign;
            newLiteral->belongClause = newClause;
            newLiteral->pre = lastLiteral;
            if(lastLiteral) lastLiteral->nextt = newLiteral;
            else newClause->first = newLiteral;
            lastLiteral = newLiteral;

            pLiteral = pLiteral->nextt;
        }
        pClause = pClause->nextt;
    }
    return ;
}
void CNFList::buildCNFList(string fileName)
{
    freopen(fileName.c_str(), "r", stdin);

    clauseHead = NULL;
    ClauseNode *lastClause = NULL;
    
    string lines;
    while(getline(cin, lines))// read datas from the file
    {
        if(lines[0] == 'c') continue;
        int p = 0;
        if(lines[0] == 'p') 
        {
            varNum = readInt(lines, p);
            clauseNum = readInt(lines, p);
            continue;
        }
        
        LiteralNode *lastLiteral = NULL;
        ClauseNode *newClause = new ClauseNode();
        newClause->pre = lastClause;
        newClause->cnf = this;
        newClause->inCNFList = true;
        if(lastClause)lastClause->nextt = newClause;
        else clauseHead = newClause;
        lastClause = newClause;

        int len = lines.length();
        int tempVar;
        while(p < len)
        {
            tempVar = readInt(lines, p);
            if(tempVar == 0) break;
            LiteralNode *newLiteral = new LiteralNode(tempVar, lastLiteral);
            newLiteral->belongClause = newClause;
            newLiteral->pre = lastLiteral;
            lastLiteral = newLiteral;
            if(!newClause->first) newClause->first = newLiteral;                
            newClause->num++;
        }
        if(newClause->num == 1) unitClauseNum++;
        else if(newClause->num == 0) 
        {
            newClause->Delete();
            clauseNum--;
        }
    }
    
    return ;
}
void CNFList::printCNFList()
{
    ClauseNode *pClause = clauseHead;
    while(pClause)
    {
        pClause->print();
        pClause = pClause->nextt;
    }
    return ;
}

