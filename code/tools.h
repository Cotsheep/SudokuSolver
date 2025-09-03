#pragma once
#include <string>
#include <stdio.h>
#include <iostream>
using namespace std;

int readInt();
int readInt(string s, int &p);
char readChar();

template<typename T>
class Queue// a simple queue by myself
{
    private:

    struct Node {
        T data;
        Node* next;
        Node();
        Node(T _data)
        {
            data = _data;
            next = nullptr;
        } 
            
    };
    Node *headNode;
    Node *tailNode;

    public:

    int _size;
    Queue()
    {
        headNode = NULL;
        tailNode = NULL;
        _size = 0;
    } 
    ~Queue() 
    {
        while (!empty())pop();
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
        if (empty()) return;
        Node *temp = headNode;
        headNode = headNode->next;
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
