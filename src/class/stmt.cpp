
#include "../header/stmt.h"

Stmt::Stmt(int line)
{

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

        Node* temp = new Node(token);
        temp->left = root;
        root = temp;
    }
    else
    {
        if (expr)

    }

}