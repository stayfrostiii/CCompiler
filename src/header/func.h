
#ifndef FUNC_H
#define FUNC_H

#include <iostream>
#include <vector>
#include "stmt.h"
#include "token.h"

using namespace std;

class Func
{
    public:
        Func();
        void addDetails(vector<Token>);
        void addContent(string);
        string print();
    private:
        string content;
        string returnType;
        string name;
};

#endif