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


    string filename = "..\\..\\..\\testFiles\\add.txt";
    ifstream javaFile;
    javaFile.open(filename);
    vector<string> lines;
    string line;

    if (!javaFile.is_open()) {
        cerr << "Error: Could not open the file " << filename << endl;
        cout << "Press any key to continue...";
        _getch();
        return 1;
    }
    
    while (getline(javaFile, line))
    {
        lines.push_back(line);
    } 

    javaFile.close();

    for (int i = 0; i < lines.size(); i++)
    {
        cout << i + 1 << ". " << lines.at(i) << endl;
    }

    cout << "Press any key to continue...";
    _getch();
    return 0;
}
