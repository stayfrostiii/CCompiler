
#ifndef TOKEN_H
#define TOKEN_H

#include <string>

using namespace std;

enum TokenType
{
    DATATYPE,
    IDEN,
    IMM,
    OPER,
    KEYWORD,
    RET,
    START,
    END,
    EQUAL
};

struct Token
{
    TokenType token;
    string value;
};

#endif