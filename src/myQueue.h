#pragma once
#include <stdio.h>
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

