
#include <conio.h>
#include <stack>
#include "../header/stmt.h"

// Nodes for Parse Tree
Node::Node(Token token)
{
    this->token = token;
    this->left = nullptr;
    this->right = nullptr;
}

// Stmt class represents statements within a function
Stmt::Stmt(int line)
{
    this->line = line;
    root = nullptr;
    this->afterEq = false;
    this->aCode = "";

    // Not in use yet
    this->regInUse = {
        {"eax", 0},
        {"ebx", 0},
        {"ecx", 0},
        {"edx", 0}
    };
}

// add each token after parsed
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
        if (token.token == END)
        {
            addExpr();
            cToAssem(root);
            // cout << root->token.value << " - " << root->left->token.value << " - " << root->right->token.value << endl;
        }
        else
            expr_infix.push_back(token);
    }
}

// Makes postfix from expression
void Stmt::constructPF()
{
    vector<Token> oper;

    // for (int i = 0; i < expr_infix.size(); i++)
    // {
    //     cout << expr_infix.at(i).value << " ";
    // }

    for (int i = 0; i < expr_infix.size(); i++)
    {

        // for (Token op : oper)
        // {
        //     cout << op.value << " ";
        // }

        // cout << endl;

        Token currToken = expr_infix.at(i);
        if (currToken.token == OPER)
        {

            if (oper.size() == 0)
                oper.push_back(currToken);

            else if (oper.front().value == "+")
                oper.insert(oper.begin(), currToken);

            else if (oper.front().value != currToken.value)
            {
                expr_postfix.insert(expr_postfix.end(), oper.begin(), oper.end());
                oper.clear();
                oper.push_back(currToken);
            }

            else
                oper.push_back(currToken);
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

    expr_postfix.insert(expr_postfix.end(), oper.begin(), oper.end());

    // cout << "\n";
    // for (int i = 0; i < expr_postfix.size(); i++)
    //     cout << expr_postfix.at(i).value << " ";
    // cout << "\n";

}

// Adds expression to tree using postfix
void Stmt::addExpr()
{
    constructPF();
    stack<Node*> st;

    for (Token currToken : expr_postfix) {

        if ((currToken.token == OPER)) {
            Node* rightNode = st.top(); 
            st.pop();

            Node* leftNode = st.top(); 
            st.pop();

            Node* newNode = new Node(currToken);
            newNode->left = leftNode;
            newNode->right = rightNode;

            // cout << newNode->token.value << " " << newNode->left->token.value << " " << newNode->right->token.value << endl;

            cToAssem(newNode);

            st.push(newNode);
        } 
        
        else 
        {
            st.push(new Node(currToken));
        }
    }
    // cout << st.top()->token.value << " ----" << endl;
    root->right = st.top(); 
}

// Converts subtrees (one parent and two children) into equivalent assembly code
void Stmt::cToAssem(Node* node)
{
    string value = node->token.value;
    string tempACode = "";

    if (node->token.token == EQUAL)
    {
        aCode += "mov " + node->left->token.value + ", eax;";
    }

    if (node->left->token.token != OPER && node->right->token.token != OPER)
    {
        tempACode += "mov eax, " + node->left->token.value + ";\n";

        if (value == "+" || value == "-")
        {
            if (value == "+")
                tempACode += "add ";
            else if (value == "-")
                tempACode += "sub "; 

            tempACode += "eax, " + node->right->token.value + ";";
        }
        else if (value == "*")
        {

        }
    }

    else if (node->left->token.token == OPER)
    {
        if (value == "+" || value == "-")
        {
            if (value == "+")
                tempACode += "add ";
            else if (value == "-")
                tempACode += "sub "; 

            tempACode += "eax, " + node->right->token.value + ";";
        }
    }

    else if (node->right->token.token == OPER)
    {
        if (value == "+" || value == "-")
        {
            if (value == "+")
                tempACode += "add ";
            else if (value == "-")
                tempACode += "sub "; 

            tempACode += "eax, " + node->left->token.value + ";";
        }
    }

    aCode += tempACode + "\n";
}

// Returns assembly code of statement
string Stmt::print()
{
    // printRecurse(root);
    return aCode;
}

// Prints parse tree of statement
void Stmt::printRecurse(Node* node)
{
    // cout << (node->left != nullptr) << " (" << node->token.value << ") " << (node->right != nullptr) << endl;
    if (node->left != nullptr)
    {
        printRecurse(node->left);
    }
    cout << node->token.value << " ";
    if (node->right != nullptr)
    {
        printRecurse(node->right);
    }
}