
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
    INCDEC, // 5
    RET, // 6
    START, // 7
    END, // 8
    EQUAL, // 9
    LOGICAL, // 10
    BR_BEGIN, // 11
    BR_END, // 12
    PR_BEGIN, // 13
    PR_END, // 14
    JUMP, // 15
    NOTHING // 16
};

struct Token
{
    TokenType token;
    string value;
};

#endif