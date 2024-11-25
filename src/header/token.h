
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
    EQUAL, // 8
    LOGICAL, // 9
    BR_BEGIN, // 10
    BR_END, // 11
    PR_BEGIN, // 12
    PR_END, // 13
    JUMP, // 14
    NOTHING // 15
};

struct Token
{
    TokenType token;
    string value;
};

#endif