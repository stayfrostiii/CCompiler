
#include <iostream>
#include <conio.h>
#include <string>

#include "header/file.h"

int main()
{
    string filename = "..\\..\\testFiles\\main2.c";

    File file1(filename);

    file1.run();

    file1.print();

    cout << "Press any key to continue...";
    _getch();
    return 0;
}