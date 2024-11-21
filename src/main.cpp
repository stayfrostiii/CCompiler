#include <iostream>
#include <fstream>
#include <conio.h>
#include <vector>
#include <string>

using namespace std;

int main()
{
    const char* cf_keywords[] = {
        "if", "else", "switch", "case", "default", "while", "do", "for",
        "break", "continue", "return"
    };

    const char* access_modifiers[] = {
        "private", "protected", "public"
    };

    const char* co_keywords[] = {
        "class", "interface", "enum", "extends", "implements", "new",
        "this", "super"
    };

    const char* eh_keywords[] = {
        "try", "catch", "finally", "throw", "throws"
    };

    const char* pt_keywords[] = {
        "byte", "short", "int", "long", "float", "double", "char", "boolean"
    };

    const char* var_shit[] = {
        "final", "static", "void", "volatile", "transient"
    };

    const char* bools[] = {
        "true", "false"
    };

    string filename = "..\\..\\..\\testFiles\\Test.java";
    ifstream javaFile;
    javaFile.open(filename);
    vector<string> object;
    string fileString;
    string line;

    fileString = "";

    if (!javaFile.is_open()) {
        cerr << "Error: Could not open the file " << filename << endl;
        cout << "Press any key to continue...";
        _getch();
        return 1;
    }
    
    string singleComm = "//";

    // while (getline(javaFile, thing))
    // {

    //     if (line.substr(0,2) != singleComm)
    //     {
    //         fileString += line;
    //         lines.push_back(line); 
    //     }
    // } 

    char ch;

    while (javaFile.get(ch))
    {
        if (ch == '\n' || ch == ' ' || ch =='\t' || ch == '\r' || ch == '\f' || 
            ch == '(' || ch == ')' || ch == '{' || ch == '}' || ch == ';')
        {
            if (ch == '(' || ch == ')' || ch == '{' || ch == '}' || ch == ';')
            {
                object.push_back(line);
                object.push_back(string(1, ch));
                line = "";
            }
            else if (line != "")
            {
                object.push_back(line);
                line = "";
            }
        }
        else
            line += ch;
    }

    javaFile.close();

    for (int i = 0; i < object.size(); i++)
    {
        cout << i + 1 << ". " << object.at(i) << endl;
    }

    cout << "Press any key to continue...";
    _getch();
    return 0;
}
