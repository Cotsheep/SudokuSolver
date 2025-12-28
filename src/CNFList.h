#pragma once
#include "tools.h"

struct LiteralNode;
struct ClauseNode;
class CNFList;

struct LiteralNode
{
    int varIndex;
    bool sign;// true for positive, false for negative
    bool inCNFList;
    LiteralNode *pre;
    LiteralNode *nextt;//in clauses
    LiteralNode *prePal;//in literalList
    LiteralNode *nextPal;
    ClauseNode *belongClause;
    LiteralNode();
    LiteralNode(int _varIndex, LiteralNode *last);
    void Delete();
};

struct ClauseNode
{
    int num;// number of literals
    bool inCNFList;
    ClauseNode *pre;
    ClauseNode *nextt;
    LiteralNode *first;// pointer to the first literal
    CNFList *cnf;
    ClauseNode();
    void print();
    void Delete();
    void initUnitClause(int var, bool sign);
};

class CNFList
{
public:
    CNFList();
    ~CNFList();// destroy itself when DPLL 
    ClauseNode *clauseHead;
    LiteralNode *literalList[MAXN];// literalList[i] is the head of the i'th variable
    SegTree *J;// J[i] is the sum of the clause's length that i'th variable appears in. 
    int clauseNum;
    int varNum;
    int unitClauseNum;
    void checkSAT(int ans[]);
    void clear();
    void printCNFList();
    void printLiteralList(int var = 0);
    void addClause(ClauseNode *clause);
    void buildCNFList(std::string fileName);
    void DeleteClause(ClauseNode *clause);
    void DeleteLiteral(LiteralNode *literal);
    void addToLiteralList(LiteralNode *literal, int copying = 0);
    void copyCNFList(const CNFList *other);// copy constructor
    void disable(ClauseNode *clause);
    void disable(LiteralNode *literal);
    void pullOut(ClauseNode *clause);
    void pullOut(LiteralNode *literal);
    void reinsert(ClauseNode *clause);
    void reinsert(LiteralNode *literal);
    void reable(ClauseNode *clause);
    void reable(LiteralNode *literal);
};

