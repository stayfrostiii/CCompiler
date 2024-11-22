
#include <conio.h>
#include "../header/stmt.h"

Node::Node(Token token)
{
    this->token = token;
}

Stmt::Stmt(int line)
{
    this->line = line;
    root = nullptr;
    this->afterEq = false;
}

void Stmt::add(Token token)
{
    if (root == nullptr)
    {
        root = new Node(token);
        root->left = nullptr;
        root->right = nullptr;
    }

    else if (!afterEq)
    {
        if (token.token == EQUAL)
            afterEq = true;

        Node* temp = root;
        root = new Node(token);
        root->left = temp;
    }

    else
    {
        if (token.token == END)
            addExpr();
        expr_infix.push_back(token);
    }
}

void Stmt::addExpr()
{
    vector<Token> expr_postfix;
    vector<Token> oper;

    for (int i = 0; i < expr_infix.size(); i++)
    {
        Token currToken = expr_infix.at(i);
        if (currToken.token == OPER)
        {
            if (currToken.value == "+" || currToken.value == "-")
            {
                if (oper.at(oper.size() - 1).value == "*" || oper.at(oper.size() - 1).value == "/")
                {
                    expr_postfix.push_back(oper.at(oper.size() - 1));
                    oper.pop_back();
                    oper.push_back(currToken);
                }

                else
                    oper.push_back(currToken);
            }

            else
            {
                if (oper.size() == 0)
                    oper.push_back(currToken);

                else
                {
                    expr_postfix.push_back(oper.at(oper.size() - 1));
                    oper.pop_back();
                    oper.push_back(currToken);
                }
            }
        }

        else if (currToken.token == IDEN || currToken.token == IMM)
            expr_postfix.push_back(expr_infix.at(i));

        else
        {
            cerr << "Error: Invalid token in expression at line " << line << endl;
            cout << "Press any key to continue...";
            _getch();
            exit(1);
        }
    }

    

}

void Stmt::print()
{

}