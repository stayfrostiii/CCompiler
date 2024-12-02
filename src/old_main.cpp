
/*

#include <iostream>
#include <conio.h>
#include <string>
#include <vector>
#include <fstream>
#include "header/token.h"
#include "header/stmt.h"
#include "header/func.h"
#include "header/file.h"

using namespace std;

// Checks to see if value is a valid identifier
bool isIden(string value)
{
    if (isdigit(value[0]))
        return false;
    int temp;
    for (int i = 0; i < value.length(); i++)
    {
        temp = value[i];
        if ((temp < 65 || temp > 90) && (temp < 48 || temp > 57) && (temp < 97 || temp > 122) && temp != 95)
            return false;

    }
    return true;
}

// Checks to see if value is valid integer
bool isInt(string value)
{
    for (int i = 0; i < value.length(); i++)
    {
        if (!isdigit(value[i]))
            return false;
    }
    return true;
}

// Assigns token to value
Token assignToken(string value)
{
    Token temp;
    map<string, TokenType> tokenMap = {
        {"if", DATATYPE}, {"char", DATATYPE}, {"short", DATATYPE}, {"long", DATATYPE}, {"float", DATATYPE}, {"double", DATATYPE}, 
        {"+", OPER}, {"-", OPER}, {"*", OPER}, {"/", OPER}, 
        {"return", RET},
        {"=", EQUAL},
        {";", END},
        {"{", BR_BEGIN},
        {"}", BR_END},
        {"(", PR_BEGIN},
        {")", PR_END}
    };

    temp.value = value;

    if (tokenMap.find(value) != tokenMap.end())
        temp.token = tokenMap[value];

    else if (isIden(value))
        temp.token = IDEN;

    else if (isInt(value))
        temp.token = IMM;

    return temp;
}

// Parses file and generates tokens for each value
vector<Token> generateTokens(string filename)
{
    ifstream cFile;
    cFile.open(filename);
    vector<Token> tokenList;

    if (!cFile.is_open()) {
        cerr << "Error: Could not open the file " << filename << endl;
        cout << "Press any key to continue...";
        _getch();
        exit(1);
    }

    string value;
    char ch;
    char prevCh;
    bool secSlash;
    bool singleCom; // Timeout parsing the file until comment is done
    bool multiCom; // Timeout parsing the file until comment is done
    bool checkCom;

    value = "";
    secSlash = false;
    singleCom = false;
    multiCom = false;
    checkCom = false;

    while(cFile.get(ch))
    {
        if (singleCom || multiCom)
        {
            if (ch == '\n' && singleCom)
                singleCom = false;
            if (prevCh = '*' && ch == '/' && multiCom)
                multiCom = false;
            continue;
        }
        else if (ch == '\n' || ch == ' ' || ch =='\t' || ch == '\r' || ch == '\f')
        {
            if (value != "")
            {
                Token temp = assignToken(value);
                temp.value = value;
                tokenList.push_back(temp);
                // cout << value << endl;
                value = "";
            }
        }
        else if (ch == '/' || ch =='*')
        {
            if (checkCom)
            {
                if (ch == '/')
                    singleCom = true;
                else if (ch == '*')
                    multiCom = true;
                else
                {
                    Token temp = assignToken(value);
                    temp.value = value;
                    tokenList.push_back(temp);
                    // cout << value << endl;
                }
                checkCom = false;
            }
            else if (ch == '*')
            {
                if (value != "")
                {
                    Token temp = assignToken(value);
                    temp.value = value;
                    tokenList.push_back(temp);
                    // cout << value << endl;
                }

                Token temp = assignToken(string(1, ch));
                temp.value = string(1, ch);
                tokenList.push_back(temp);
                // cout << ch << endl;
                value = "";
            }
            else
                checkCom = true;
        }
        else if (ch == ';' || ch == '{' || ch == '}' || ch == '(' || ch == ')')
        {
            if (value != "")
            {
                Token temp = assignToken(value);
                temp.value = value;
                tokenList.push_back(temp);
                // cout << value << endl;
            }

            Token temp = assignToken(string(1, ch));
            temp.value = string(1, ch);
            tokenList.push_back(temp);
            // cout << ch << endl;
            value = "";
        }
        else
            value += ch;
        prevCh = ch;
    }

    // cout << value << endl;

    cFile.close();

    return tokenList;
}

int main()
{
    string filename = "..\\..\\testFiles\\mainNoBrackets.txt";
    // vector<Token> tokenList;

    // // Pass 1: Lexical Analyzer
    // tokenList = generateTokens(filename);
    // vector<Stmt*> statements;
    // vector<Token> funcDetails;
    // vector<Func*> functions;
    // int lineCounter = 0;
    // bool inFunc = false;
    // string aCode = "";
    // Stmt* stmt = new Stmt(lineCounter);
    // Func* func = new Func();

    // for (Token token : tokenList)
    // {
    //     // cout << token.value << " " << token.token << endl;

    //     if (token.token == BR_BEGIN)
    //     {
    //         inFunc = true;
    //         func->addDetails(funcDetails);
    //     }
    //     else if (token.token == BR_END)
    //     {
    //         func->addContent(aCode);
    //         functions.push_back(func);
    //         inFunc = false;
    //         func = new Func();
    //     }

    //     else if (inFunc)
    //     {
    //         stmt->add(token);
    //         if (token.token == END)
    //         {
    //             statements.push_back(stmt);
    //             lineCounter++;
    //             aCode += stmt->print();
    //             stmt = new Stmt(lineCounter);
    //         }
    //     }
    //     else
    //     {
    //         funcDetails.push_back(token);
    //     }
    //     // cout << tokenList.at(i).token << " - " << tokenList.at(i).value << endl;
    // }

    // for (Func* function : functions)
    //     cout << function->print();

    File file1(filename);

    file1.print();

    cout << "Press any key to continue...";
    _getch();
    return 0;
}
*/