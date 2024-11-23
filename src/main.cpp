
#include <iostream>
#include <conio.h>
#include <string>
#include <vector>
#include <fstream>
#include "header/token.h"
#include "header/stmt.h"

using namespace std;

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

bool isInt(string value)
{
    for (int i = 0; i < value.length(); i++)
    {
        if (!isdigit(value[i]))
            return false;
    }
    return true;
}

Token assignToken(string value)
{
    Token temp;
    temp.value = value;

    if (value == "int" || value == "char" || value == "short" || value == "long" || value == "float" || value == "double")
        temp.token = DATATYPE;

    else if (value == "+" || value == "-" || value == "*" || value == "/")
        temp.token = OPER;
    
    else if (value == "return")
        temp.token = RET;

    else if (value == ";" || value == "}" || value == ")")
        temp.token = END;

    else if (value == "=")
        temp.token = EQUAL;

    else if (isIden(value))
        temp.token = IDEN;

    else if (isInt(value))
        temp.token = IMM;

    return temp;
}

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
        else if (ch == ';')
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
    vector<Token> tokenList;

    // Pass 1: Lexical Analyzer
    tokenList = generateTokens(filename);
    Stmt* test = new Stmt(0);

    for (int i = 0; i < tokenList.size(); i++)
    {
        test->add(tokenList.at(i));
        // cout << tokenList.at(i).token << " - " << tokenList.at(i).value << endl;
    }

    test->print();

    cout << "Press any key to continue...";
    _getch();
    return 0;
}