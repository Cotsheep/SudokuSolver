#include "CNFList.h"
// #define DEBUG

LiteralNode::LiteralNode()
{
    inCNFList = false;
    belongClause = NULL;
    nextPal = NULL;
    prePal = NULL;
    nextt = NULL;
    pre = NULL;
    sign = 0;
}
LiteralNode::LiteralNode(int _varIndex, LiteralNode *last)
{
    sign = _varIndex > 0 ? 1 : 0;
    varIndex = _varIndex > 0 ? _varIndex : -_varIndex;
    if(last) last -> nextt = this;
    nextPal = NULL;
    prePal = NULL;
    nextt = NULL;
    pre = last;
}
void LiteralNode::Delete()
{
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
    if(first)
    {
        LiteralNode *pLiteral = first;
        LiteralNode *tempLiteral;
        while(pLiteral)
        {
            tempLiteral = pLiteral;
            pLiteral = pLiteral->nextt;
            cnf->DeleteLiteral(tempLiteral);
        }
    }
    if(pre) pre->nextt = nextt;
    else if(cnf) cnf->clauseHead = nextt;

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
    putchar(inCNFList ? '+':'-');
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

void standardCheck()
{
    
}

void CNFList::DeleteClause(ClauseNode *clause)
{
    pullOut(clause);
    clause->Delete();
    return ;
}
void CNFList::DeleteLiteral(LiteralNode *literal)
{
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

    LiteralNode *pLiteral = clause->first;
    while(pLiteral)
    {
        addToLiteralList(pLiteral);
        pLiteral->inCNFList = true;
        pLiteral = pLiteral->nextt;
    }
    return ;
}
void CNFList::addToLiteralList(LiteralNode *literal, int copying)
{
    if(!literal){printf("you are adding an fking empty literal to list!"); return ;}
    if(literal->prePal || literal->nextPal){printf("this literal is already in a literalList!"); return ;}
    
    int var = literal->varIndex;
    if(copying)printf("Adding literal %d%c to literalList[%d]\n", var, literal->sign ? '+' : '-', var);
    
    if(literalList[var])
    {
        if(copying)
        {
            printf("literalList[%d] already has elements when adding\n", var);
            printf("the existing pointer is: %p\n", literalList[var]);
            printf("the existing elements is: %d\n", literalList[var]->varIndex * (literalList[var]->sign ? 1 : -1));
        }
        literalList[var]->prePal = literal;
    }

    if(copying)printf("literalList added\n");

    literal->nextPal = literalList[var];
    literal->prePal = NULL;
    literalList[var] = literal;
    literal->inCNFList = true;

    return ;
}
void CNFList::pullOut(ClauseNode *clause)
{
    CNFList *cnf = clause->cnf;
    if(cnf != this){printf("this clause's mother isn't me.\n");return ;}
    // if(!clause->inCNFList){printf("why doesn't it in cnf list when pulling?");clause->print();putchar(10);return ;}

    if(clause->pre) clause->pre->nextt = clause->nextt;
    else clauseHead = clause->nextt;
    if(clause->nextt) clause->nextt->pre = clause->pre;
    
    if(!clause->inCNFList)return ;
    clause->inCNFList = false;

    if(clause->num == 1) unitClauseNum--;
    clauseNum--;

    LiteralNode *pLiteral = clause->first;
    while(pLiteral)
    {
        cnf->J->update(cnf->J->root, 
            pLiteral->varIndex, -clause->num, 2);
        pLiteral = pLiteral->nextt;
    }

    if(clause->num < 0) {printf("Error: clause num < 0 when pulling clause\n"); clause->print(); return ;}

    return ;
}
void CNFList::pullOut(LiteralNode *liter)
{
    ClauseNode *clause = liter->belongClause;
    CNFList *cnf = clause->cnf;
    if(!clause){printf("this liter doesn't have belongClause?!\n"); return ;}
    if(cnf != this){printf("this liter's mother isn't me.\n");return ;}
    
    // if(!liter->inCNFList){printf("why doesn't it in cnf list when pulling?%d", liter->varIndex * (liter->sign ? 1 : -1));putchar(10);return ;}
    // if(!clause->inCNFList){printf("warning: the clause isn't in CNFlist!");}
    // pulling this literal doesn't matter
    //because when restoring the clause, the literal will be restored first

    if(liter->pre) liter->pre->nextt = liter->nextt;
    else if(clause) clause->first = liter->nextt;
    if(liter->nextt) liter->nextt->pre = liter->pre;

    int var = liter->varIndex;

    if(liter->nextPal) liter->nextPal->prePal = liter->prePal;
    if(liter->prePal) liter->prePal->nextPal = liter->nextPal;
    else if(literalList[var] == liter) literalList[var] = liter->nextPal;
    else 
    {
        printf("Error in pulling literal from literalList\n");
        exit(0);
    }
    liter->prePal = NULL;
    liter->nextPal = NULL;
    
    if(!liter->inCNFList || !clause->inCNFList) return ;
    liter->inCNFList = false;

    if(clause->num == 1) unitClauseNum++;
    else if(clause->num == 0) unitClauseNum--;
    
    cnf->J->update(cnf->J->root, 
        liter->varIndex, -clause->num, 2);
        
    clause->num--;

    if(clause->num < 0) {printf("Error: clause num < 0 when pulling literal\n"); clause->print(); return ;}

    return ;
}
void CNFList::reinsert(ClauseNode *clause)
{
    CNFList *cnf = clause->cnf;
    if(cnf != this){printf("this clause's mother isn't me.\n");return ;}
    clause->pre = NULL;
    clause->nextt = clauseHead;
    if(clauseHead) clauseHead->pre = clause;
    clause->inCNFList = true;

    clauseHead = clause;
    clauseNum++;
    if(clause->num == 1) unitClauseNum++;

    LiteralNode *pLiteral = clause->first;
    while(pLiteral)
    {
        if(pLiteral->inCNFList) 
            cnf->J->update(cnf->J->root, pLiteral->varIndex, clause->num, 2);
        pLiteral = pLiteral->nextt;
    }

    return ;
}
void CNFList::reinsert(LiteralNode *liter)
{
    ClauseNode *clause = liter->belongClause;
    CNFList *cnf = clause->cnf;

    if(!clause){printf("this liter doesn't have belongClause?!\n"); return;}
    if(cnf != this){printf("this liter's mother isn't me.\n");return ;}
    
    liter->pre = NULL;
    liter->nextt = clause->first;
    if(clause->first) clause->first->pre = liter;
    clause->first = liter;
    liter->inCNFList = true;
    
    if(liter->nextPal || liter->prePal){printf("this literal is already in a literalList!"); return ;}
    
    int var = liter->varIndex;
    liter->prePal = NULL;
    liter->nextPal = literalList[var];
    if(literalList[var]) literalList[var]->prePal = liter;
    literalList[var] = liter;
    
    clause->num++;
    if(!clause->inCNFList) return ;
    if(clause->num == 1) unitClauseNum++;
    if(clause->num == 2) unitClauseNum--;
    
    cnf->J->update(cnf->J->root, 
        liter->varIndex, clause->num, 2);
    cnf->J->update(cnf->J->root, 
        liter->varIndex, true, 3);
    
    return ;
}


CNFList::CNFList()
{
    clauseHead = NULL;
    clauseNum = 0;
    varNum = 0;
    unitClauseNum = 0;
    J = new SegTree();
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
    for(int i = 1; i < MAXN; ++i) literalList[i] = NULL;
    clauseHead = NULL;
    clauseNum = 0;
    varNum = 0;
    unitClauseNum = 0;
    J->clear();
    return ;
}
void CNFList::copyCNFList(const CNFList *other)// copy
{
    
    // printf("COPYING\n----------------------------\n");
    clear();
    clauseNum = other->clauseNum;
    varNum = other->varNum;
    unitClauseNum = other->unitClauseNum;
    clauseHead = NULL;
    ClauseNode *pClause = other->clauseHead;
    ClauseNode *lastClause = NULL;
    
    int clauseCnt = 0;

    while(pClause)
    {
        ClauseNode *newClause = new ClauseNode();
        newClause->num = pClause->num;
        newClause->cnf = this;

        newClause->pre = lastClause;
        if(lastClause) lastClause->nextt = newClause;
        else clauseHead = newClause;
        lastClause = newClause;

        LiteralNode *pLiteral = pClause->first;
        LiteralNode *lastLiteral = NULL;

        int literalCnt = 0;

        while(pLiteral)
        {
            LiteralNode *newLiteral = new LiteralNode();
            newLiteral->varIndex = pLiteral->varIndex;
            newLiteral->sign = pLiteral->sign;
            newLiteral->belongClause = newClause;
            newLiteral->pre = lastLiteral;
            if(lastLiteral) lastLiteral->nextt = newLiteral;
            else newClause->first = newLiteral;
            // printf("Adding %d\n", ++literalCnt);
            addToLiteralList(newLiteral);

            // if(++literalCnt >= 30){printf("literal overwhelm!\n"); exit(0);}
            // printf("Copied literal %d in clause %d\n", ++literalCnt, clauseCnt + 1);

            lastLiteral = newLiteral;
            pLiteral = pLiteral->nextt;
        }
        pClause = pClause->nextt;

        // printf("Copied clause %d\n", ++clauseCnt);
    }

    // printf("Copied %d clauses and %d variables.\n", clauseNum, varNum);

    J->copyTree(other->J);
    return ;
}
void CNFList::buildCNFList(string fileName)
{
    freopen(fileName.c_str(), "r", stdin);

    #ifdef DEBUG
        // putchar(getchar());
        // putchar(10);
        printf("DEBUG: Building CNFList from %s\n", fileName.c_str());
    #endif

    clauseHead = NULL;
    ClauseNode *lastClause = NULL;
    
    string lines;
    int tempJ[MAXN];for(int i = 0; i < MAXN; ++i) tempJ[i] = 0;
    
    int lineCnt = 0;

    #ifdef DEBUG
    printf("Reading CNF file...\n");
    #endif

    while(getline(cin, lines))// read datas from the file
    {
        #ifdef DEBUG
        printf("got in");
        #endif

        if(lines[0] == 'c') continue;
        int p = 0;
        if(lines[0] == 'p') 
        {
            varNum = readInt(lines, p);
            clauseNum = readInt(lines, p) * 2;
            for(int i = 1; i <= varNum; ++i) literalList[i] = NULL;
            continue;
        }

        #ifdef DEBUG
        printf("Reading line %d\n", ++lineCnt);
        #endif
        
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

            addToLiteralList(newLiteral);
        }
        if(newClause->num == 1) unitClauseNum++;
        else if(newClause->num == 0) 
        {
            newClause->Delete();
            clauseNum--;
            continue;
        }

        LiteralNode *pLiteral = newClause->first;
        while(pLiteral)
        {
            int var = pLiteral->varIndex;
            tempJ[var] += newClause->num;
            pLiteral = pLiteral->nextt;
        }

    }

    #ifdef DEBUG
    printf("CNFList built: %d clauses, %d variables, %d unit clauses.\n", clauseNum, varNum, unitClauseNum);
    #endif

    J->build(tempJ, J->root, 1, varNum);

    freopen("CON", "r", stdin);
    cin.clear();
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
void CNFList::printLiteralList(int var)
{
    int cnt = 0;
    LiteralNode *pLiteral;
    for(int i = 1; i <= varNum; ++i)
    {
        if(var && i != var)continue;
        printf("variable %d:\n", i);
        pLiteral = literalList[i];
        while(pLiteral)
        {
            if(++cnt >= 10)break;
            printf("%d in ", pLiteral->varIndex * 
                (pLiteral->sign ? 1 : -1));
            pLiteral->belongClause->print();
            printf("pre: %p, next: %p, selfP: %p\n", 
                pLiteral->prePal, pLiteral->nextPal, pLiteral);
            pLiteral = pLiteral->nextPal;
        }
    }
    putchar(10);
    return ;
}
