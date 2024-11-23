
#ifndef TOKEN_H
#define TOKEN_H

#include <string>

using namespace std;

enum TokenType
{
    DATATYPE, // 0
    IDEN, // 1
    IMM, // 2
    OPER, // 3
    KEYWORD, // 4
    RET, // 5
    START, // 6
    END, // 7
    EQUAL // 8
};

struct Token
{
    TokenType token;
    string value;
};

#endif