#include "tools.h"
#include <cstdlib>

int readInt() 
{
    int res = 0; char c = getchar(); int f = 1;
    while (c > '9' || c < '0') {
        if (c == '-') f = -1;
        c = getchar();
    } while (c <= '9' && c >= '0') {
        res = (res << 3) + (res << 1) + (c ^ 48);
        c = getchar();
    }
    return res * f;
}
int readInt(string s, int &p)
{
    int res = 0; char c = s[p]; int f = 1;
    while (c > '9' || c < '0') {
        if (c == '-') f = -1;
        c = s[++p];
    } while (c <= '9' && c >= '0') {
        res = (res << 3) + (res << 1) + (c ^ 48);
        c = s[++p];
    }
    return res * f;
}

char readChar() {
    char c = getchar();
    while (c == ' ' || c == '\n' || c == '\r' || c == '\t') c = getchar();
    return c;
}
void Swap(int &a, int &b) 
{
    int temp = a;
    a = b;
    b = temp;
    return ;
}
void shuffle(int a[], int n)// 0 ~ n-1
{
    for(int i = n - 1; i > 0; --i)
    {
        int j = rand() % (i + 1);
        Swap(a[i], a[j]);
    }
    return ;
}
// Print colored string s in console, color can be "red", "green", "blue", "yellow", or others for default color
void printColor(string s, string color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if(color == "red") SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
    else if(color == "green") SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    else if(color == "blue") SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    else if(color == "yellow") SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    else SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    std::cout << s;
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    return ;
}

SegTree::TreeNode::TreeNode()
{
    id = -1;
    minn = 1e9;
    abled = true;
    start = end = 0;
    leftChild = NULL;
    rightChild = NULL;
}
void SegTree::TreeNode::copyVal(TreeNode *other)
{
    if(!other) 
    {
        printf("Error: other is NULL in copyVal()\n");
        return ;
    }
    id = other->id;
    minn = other->minn;
    abled = other->abled;
    return ;
}
SegTree::SegTree()
{
    root = new TreeNode();
}
SegTree::~SegTree()
{
    clear();
}
void SegTree::clear()
{
    clearDFS(root);
    root = NULL;
}
void SegTree::clearDFS(TreeNode *node)
{
    if(!node) return ;
    clearDFS(node->leftChild);
    clearDFS(node->rightChild);
    delete node;
    return ;
}
void SegTree::pushUp(TreeNode *node)
{
    if(!node->leftChild && !node->rightChild) return ;
    if(!node->leftChild || !node->leftChild->abled) 
    {
        node->copyVal(node->rightChild);
        return ;
    }
    else if(!node->rightChild || !node->rightChild->abled)
    {
        node->copyVal(node->leftChild);
        return ;
    }
    if(node->leftChild->minn < node->rightChild->minn)
        node->copyVal(node->leftChild);
    else if(node->leftChild->minn > node->rightChild->minn)
        node->copyVal(node->rightChild);
    else {
        if (rand() % 2) node->copyVal(node->leftChild);
        else node->copyVal(node->rightChild);
    }
    return ;
}
void SegTree::build(int J[], TreeNode *node, int start, int end)
{
    node->start = start;
    node->end = end;
    if(start == end)
    {
        node->id = start;
        node->minn = J[start];
        node->abled = true;
        return ;
    }
    int mid = (start + end) >> 1;
    node->leftChild = new TreeNode();
    node->rightChild = new TreeNode();
    build(J, node->leftChild, start, mid);
    build(J, node->rightChild, mid + 1, end);
    pushUp(node);
    return ;
}
void SegTree::update(TreeNode *node, int id, int val, int mod)
{
    if(!node){printf("Error: node is NULL in update()\n"); return ;}
    if(node->start == id && node->end == id)
    {
        if(mod == 1)node->minn = val;
        else if(mod == 2) node->minn += val;
        else node->abled = val;
        return ;
    }
    if(node->leftChild->end >= id)
        update(node->leftChild, id, val, mod);
    else
        update(node->rightChild, id, val, mod);
    pushUp(node);
    return ;
}
void SegTree::printDFS(TreeNode *node)
{
    if(!node) return ;
    if(node->start == node->end)
        printf("id: %d, minn: %d, abled: %d, range: [%d, %d] pointer: %p\n", 
            node->id, node->minn, node->abled, node->start, node->end, node);
    printDFS(node->leftChild);
    printDFS(node->rightChild);
    return ;
}
void SegTree::printTree()
{
    printDFS(root);
    return ;
}
int SegTree::query()
{
    return root->abled ? root->id : -1;
}
int SegTree::queryDFS(TreeNode *node, int id)
{
    if(!node) return -1;
    if(!node->abled) return -1;
    if(node->end == id && node->start == id) return node->minn;
    if(node->leftChild->end >= id)
        return queryDFS(node->leftChild, id);
    else
        return queryDFS(node->rightChild, id);
}
int SegTree::query(int id)
{
    return queryDFS(root, id);
}
void SegTree::copyDFS(TreeNode *thisNode, TreeNode *otherNode)
{
    if(!thisNode || !otherNode) return ;
    thisNode->copyVal(otherNode);
    thisNode->start = otherNode->start;
    thisNode->end = otherNode->end;
    if(otherNode->leftChild)
    {
        thisNode->leftChild = new TreeNode();
        copyDFS(thisNode->leftChild, otherNode->leftChild);
    }
    if(otherNode->rightChild)
    {
        thisNode->rightChild = new TreeNode();
        copyDFS(thisNode->rightChild, otherNode->rightChild);
    }
    return ;
}
void SegTree::copyTree(SegTree *other)
{
    if(!other || !other->root) 
    {
        printf("Error: other is NULL in copyTree()\n");
        return ;
    }
    clear();
    root = new TreeNode();
    copyDFS(root, other->root);
    return ;
}
