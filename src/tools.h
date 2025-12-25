#pragma once
#include <string>
#include <stdio.h>
#include <filesystem>
#include <windows.h>
#include <iostream>
#include <cstring>
#include <random>
#include <ctime>
using namespace std;
#define MAXN 10005

extern string gRootPath; // global root path of the project
int readInt();
int readInt(string s, int &p);
char readChar();
void Swap(int &a, int &b);
void shuffle(int a[], int n);// 0 ~ n-1
void printColor(string s, string color);
string getExePath();
string GetRootPath();

template<typename T>
class Queue// a simple queue by myself
{
    private:

    struct Node {
        T data;
        Node *next;
        Node();
        Node(T _data)
        {
            data = _data;
            next = nullptr;
        } 
            
    };
    Node *headNode;
    Node *tailNode;
    int _size;

    public:

    Queue()
    {
        headNode = NULL;
        tailNode = NULL;
        _size = 0;
    } 
    ~Queue() 
    {
        // printf("Queue destructing(%d)...\n", _size);

        while (!empty())
        {
            // printf("%d ", _size);
            // printf("popping:");
            pop();
            // printf(" done\n");
        }
        
        // printf("Queue destructed.\n");
    }
    void push(T value) 
    {
        Node *node = new Node(value);
        if (tailNode) tailNode->next = node;
        else headNode = node;
        tailNode = node;
        _size++;
    }
    void pop() 
    {

        // printf("p->");// 调试析构函数爆炸时用的
        
        if (empty()) return;
        Node *temp = headNode;
        headNode = headNode->next;
        
        // printf("3->");
        // if(!temp){printf("Error: temp is NULL in pop()\n"); return ;}

        delete temp;
        _size--;
        if (!headNode) tailNode = NULL;
    }
    T front() 
    {
        return headNode->data;
    }
    bool empty()
    {
        return _size == 0;
    }
    int size()
    {
        return _size;
    }

};

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
