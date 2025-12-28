#pragma once
#include <stdio.h>

class SegTree
{
    private:

    struct TreeNode
    {
        int id;
        int minn;
        int start, end;
        bool abled;
        TreeNode *leftChild, *rightChild;
        TreeNode();
        void copyVal(TreeNode *other);
    };
    void pushUp(TreeNode *node);
    void clearDFS(TreeNode *node);
    void printDFS(TreeNode *node);
    void copyDFS(TreeNode *thisNode, TreeNode *otherNode);
    int queryDFS(TreeNode *node, int id);

    public:

    TreeNode *root;
    SegTree();
    ~SegTree();
    void build(int J[], TreeNode *node, int start, int end);
    void update(TreeNode *node, int id, int val, int mod);// mod = 1 for overwrite, 2 for add, 3 for modify ableness
    void printTree();
    void clear();
    void copyTree(SegTree *other);
    int query();// return the max value
    int query(int id);
};
