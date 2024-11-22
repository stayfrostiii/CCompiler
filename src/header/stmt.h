
#ifndef STMT_H
#define STMT_H

#include <string>
#include <iostream>
#include <vector>
#include "../header/token.h"

using namespace std;

class Node
{
    public:
        Node(Token);
        Token token;
        Node* left;
        Node* right;
};

class Stmt
{
    public:
        Stmt(int);
        void add(Token);
        void addExpr();
        void print();
    private:
        Node* root;
        vector<Token> expr_infix;
        int line;
        bool afterEq;
};

#endif