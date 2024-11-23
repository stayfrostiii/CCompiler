
#ifndef STMT_H
#define STMT_H

#include <string>
#include <iostream>
#include <vector>
#include <map>
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
        void print();
    private:
        Node* root;
        vector<Token> expr_infix;
        vector<Token> expr_postfix;
        int line;
        string aCode;
        map<string, int> regInUse;
        bool afterEq;
        void constructPF();
        void addExpr();
        void printRecurse(Node*);
        void cToAssem(Node*);
};

#endif