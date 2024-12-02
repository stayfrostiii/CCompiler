
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
        string print();
    private:
        Node* root;
        Node* expr;
        vector<Token> expr_infix;
        vector<Token> expr_postfix;
        int line;
        string aCode;
        string cCode;
        map<string, int> regInUse;
        bool afterEq;
        bool isDeclaration;

        void constructPF();
        void addExpr();
        void printRecurse(Node*);
        void cToAssemExpr(Node*);

};

#endif