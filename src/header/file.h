
#ifndef FILE_H
#define FILE_H

#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <map>

#include "token.h"

using namespace std;

enum Path
{
    FUNCTION,
    DECLARATION,
    WHILE,
    FOR,
    IF,
    EXPR,
    RETURN
};

class File
{
    public:
        File(string);
        void run();
        void print();
    private:

        // Filename
        string filename;

        // Counts lines using \n character
        int lineCounter;

        // .data part of assembly
        string aCodeData;

        // .text part of assembly
        string aCodeText;

        // Token vector for debugging
        vector<Token> tokenList;

        // Variables for finding correct path
        int path;
        bool pathChosen;

        // Variables for expression parsing
        map<string, bool> varsUsed;
        bool afterEq;
        bool secondVal;
        string currLine;
        string endOfLine;
        int exprIndex;
        int mdIndex;

        // Variables for loop parsing
        string jumpStmt;
        string compStmt;
        int loopCounter;

        // Variables for following grammars
        int stage;
        bool funcOn;
        stack<string> functionsInUse;

        bool hasError;

        void generateTokens();
        Token assignToken(string);
        void generateAssem(string);
        bool isInt(string);
        bool isIden(string);
};

#endif 