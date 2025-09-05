#pragma once
#include "tools.h"

struct LiteralNode;
struct ClauseNode;
class CNFList;

struct LiteralNode
{
    int varIndex;
    bool sign;// true for positive, false for negative
    LiteralNode *pre;
    LiteralNode *nextt;
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
    int clauseNum;
    int varNum;
    int unitClauseNum;
    void checkSAT(int ans[]);
    void clear();
    void printCNFList();
    void addClause(ClauseNode *clause);
    void buildCNFList(string fileName);
    void DeleteClause(ClauseNode *clause);
    void DeleteLiteral(LiteralNode *literal);
    void copyCNFList(const CNFList *other);// copy constructor
    void pullOut(ClauseNode *clause);
    void pullOut(LiteralNode *literal);
    void reinsert(ClauseNode *clause);
    void reinsert(LiteralNode *literal);
};

