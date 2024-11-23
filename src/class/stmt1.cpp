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
                while (!oper.empty() && (oper.back().value == "*" || oper.back().value == "/"))
                {
                    expr_postfix.push_back(oper.back());
                    oper.pop_back();
                }
                oper.push_back(currToken);
            }

            else
            {
                while (!oper.empty())
                {
                    expr_postfix.push_back(oper.back());
                    oper.pop_back();
                }
                oper.push_back(currToken);
            }
        }

        else if (currToken.token == IDEN || currToken.token == IMM)
        {
            expr_postfix.push_back(currToken);
        }

        else
        {
            cerr << "Error: Invalid token in expression at line " << line << endl;
            cout << "Press any key to continue...";
            _getch();
            exit(1);
        }
    }

    while (!oper.empty())
    {
        expr_postfix.push_back(oper.back());
        oper.pop_back();
    }

    this->expr_postfix = expr_postfix;
}

void Stmt::generateAssembly()
{
    printf("; Assembly for statement\n");

    for (auto token : expr_postfix)
    {
        if (token.token == IMM) {
            printf("PUSH %s\n", token.value.c_str());
        }
        else if (token.token == IDEN) {
            printf("PUSH [%s]\n", token.value.c_str());
        }
        else if (token.token == OPER) {
            if (token.value == "+") printf("ADD\n");
            else if (token.value == "-") printf("SUB\n");
            else if (token.value == "*") printf("MUL\n");
            else if (token.value == "/") printf("DIV\n");
        }
    }

    // Handle assignment if applicable
    if (root && root->token.token == EQUAL) {
        printf("POP [%s]\n", root->left->token.value.c_str());
    }
}

void Stmt::print()
{
    printf("Infix Expression: ");
    for (auto token : expr_infix) {
        printf("%s ", token.value.c_str());
    }
    printf("\nPostfix Expression: ");
    for (auto token : expr_postfix) {
        printf("%s ", token.value.c_str());
    }
    printf("\n");
}
