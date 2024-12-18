
#include <fstream>
#include <conio.h>

#include "../header/file.h"

File::File(string filename)
{
    this->filename = filename;
    lineCounter = 1;
    aCodeText = ".text\n\n";
    aCodeData = ""
                ".386\n"
                ".model flat, stdcall\n"
                ".stack 4096\n"
                "ExitProcess PROTO, dwExitCode: DWORD\n\n"
                ".data\n\n";
    path = 0;
    pathChosen = false;  
    currLine = "";
    endOfLine = "";
    exprIndex = 0;
    mdIndex = 0;
    loopCounter = 1;
    jumpStmt = "";
    compStmt = "";
    stage = 0;
    funcOn = false;
    secondVal = false;
    hasError = false;
    isDeclare = false;
    endOfFor = "";
    noOper = true;
    immUsed = false;
    beforePRBegin = false;
    needOper = false;
}

void File::run()
{
    // Load File
    ifstream cFile;
    cFile.open(filename);

    // Check if file is found
    if (!cFile.is_open()) {
        cerr << "Error: Could not open the file " << filename << endl;
        cout << "Press any key to continue...";
        _getch();
        exit(1);
    }

    // Variables for parsing
    string value;
    char ch;
    char prevCh;
    bool secSlash;
    bool singleCom; // Timeout parsing the file until comment is done
    bool multiCom; // Timeout parsing the file until comment is done
    bool checkCom;
    bool checkInc;

    // Initializing parsing values
    value = "";
    secSlash = false;
    singleCom = false;
    multiCom = false;
    checkCom = false;
    checkInc = false;

    // Loop
    while(cFile.get(ch))
    {
        // True when currently parsing through comment
        if (singleCom || multiCom)
        {
            if (ch == '\n')
                lineCounter++;
            if (ch == '\n' && singleCom)
                singleCom = false;
            if (prevCh = '*' && ch == '/' && multiCom)
                multiCom = false;
            continue;
        }

        
        else if (checkInc)
        {  
            if (ch == '+')
            {
                value += ch;
                generateAssem(value);
                value = "";
            }
            else
            {
                generateAssem(value);
                value = "";
            }
            checkInc = false;
        }

        // True when end of "value"
        else if (ch == '\n' || ch == ' ' || ch =='\t' || ch == '\r' || ch == '\f')
        {
            if (ch == '\n')
                lineCounter++;
            generateAssem(value);
            value = "";
        }

        // Used for checking for comment
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
                    generateAssem(value);
                    value = "";
                }
                checkCom = false;
            }
            else if (ch == '*')
            {
                if (value != "")
                {
                    generateAssem(value);
                    value = "";
                }

                generateAssem(string(1, ch));
            }
            else
                checkCom = true;
        }

        else if (ch == '+')
        {
            if (!checkInc)
            {
                if (value != "")
                {
                    generateAssem(value);
                    value = "";
                    checkInc = true;
                    value += ch;
                }
                else
                {
                    checkInc = true;
                    value += ch;
                }
            }
        }

        else if (ch == ';' || ch == '{' || ch == '}' || ch == '(' || ch == ')')
        {
            generateAssem(value);
            generateAssem(string(1, ch));
            value = "";
        }

        // Add to value if neither above
        else
            value += ch;
        prevCh = ch;
    }

    cFile.close();

    if (functionsInUse.size() != 0)
        hasError = true;

    // for (Token toke : tokenList)
    // {
    //     cout << toke.value << " ~ " << toke.token << "\n";
    // }
    // cout << "\n\n";
}

void File::generateAssem(string value)
{
    if (value != "")
    {
        // Assign token
        Token token = assignToken(value);
        token.value = value;
        tokenList.push_back(token);

        if (token.token == BR_END)
        {
            if (functionsInUse.size() == 0)
            {
                
                hasError = true;
                cout << "Error at line " << lineCounter << "\n";
            }
            else
            {
                aCodeText += functionsInUse.top();
                functionsInUse.pop();
                pathChosen = false;
            }
        }

        // cout << token.value << " " << pathChosen << endl;

        else if (!pathChosen)
        {
            switch(token.token)
            {
                case DATATYPE:
                    if (!funcOn)
                    {
                        path = FUNCTION;
                    }
                    else
                        path = DECLARATION;
                    break;
                case JUMP:
                    if (token.value == "while")
                        path = WHILE;
                    if (token.value == "for")
                        path = FOR;
                    if (token.value == "if")
                        path = IF;
                    break;
                case IDEN:
                    path = EXPR;
                    break;
                case RET:
                    path = RETURN;
            }
            pathChosen = true;
        }

        if (pathChosen)
        {
            // cout << token.value << " " << path << endl;
            switch (path)
            {
                case FUNCTION:
                    switch (stage)
                    {
                        case 0:
                            if (token.token == DATATYPE)
                                stage++;
                            else
                                stage = -1;
                            break;
                        case 1:
                            if (token.token == IDEN)
                            {
                                aCodeText += token.value + " proc\n\n";
                                functionsInUse.push(token.value + " endp\n");
                                stage++;
                            }
                            else
                                stage = -1;
                            break;
                        case 2:
                            if (token.token == PR_BEGIN)
                                stage++;
                            else
                                stage = -1;
                            break;
                        case 3:
                            if (token.token == PR_END)
                                stage++;
                            else
                                stage = -1;
                            break;
                        case 4:
                            if (token.token == BR_BEGIN)
                            {
                                funcOn = true;
                                stage = 0;
                                pathChosen = false;
                            }
                            else
                                stage = -1;
                            break;
                        default:
                            hasError = true;
                            cout << "Error at line " << lineCounter << "\n";
                            stage = 0;
                            pathChosen = false;
                            break;
                    }
                    break;

                case DECLARATION:
                    switch (stage)
                    {
                        case 0:
                            if (token.token == DATATYPE)
                                stage++;
                            else
                                stage = -1;
                            break;
                        case 1:
                            if (token.token == IDEN)
                            {
                                aCodeData += token.value + " dword ";
                                varsUsed[token.value] = true;
                                stage++;
                            }
                            else
                                stage = -1;
                            break;
                        case 2:
                            if (token.token == EQUAL)
                                stage++;
                            else if (token.token == END)
                            {
                                aCodeData += "?\n";
                                stage = 0;
                                pathChosen = false;
                            }
                            else
                                stage = -1;
                            break;
                        case 3:
                            if (token.token == IDEN || token.token == IMM)
                            {
                                aCodeData += token.value + "\n";
                                stage++;
                            }
                            else
                                stage = -1;
                            break;
                        case 4:
                            if (token.token == END)
                            {
                                stage = 0;
                                pathChosen = false;
                            }
                            else
                                stage = -1;
                            break;
                        default:
                            hasError = true;
                            cout << "Error at line " << lineCounter << "\n";
                            stage = 0;
                            pathChosen = false;
                            break;
                    }
                    break;

                case WHILE:
                    switch (stage)
                    {
                        case 0:
                            if (token.token == JUMP)
                                stage++;
                            else
                                stage = -1;
                            break;
                        case 1:
                            if (token.token == PR_BEGIN)
                            {
                                compStmt += "loop" + to_string(loopCounter) + ":\n";
                                stage++;
                            }
                            else
                                stage = -1;
                            break;
                        case 2:
                            if (token.token == IDEN)
                            {
                                compStmt = "mov ecx, [" + token.value + "]\n" + compStmt;
                                stage++;
                            }
                            else if (token.token == IMM)
                            {
                                compStmt = "mov ecx, " + token.value + "\n" + compStmt;
                                immUsed = true;
                                stage++;
                            }
                            else
                                stage = -1;
                            break;
                        case 3:
                            if (token.token == LOGICAL)
                            {
                                compStmt += "cmp ecx, ";
                                if (token.value == "<")
                                    jumpStmt = "jge end" + to_string(loopCounter) + "\n";
                                if (token.value == ">")
                                    jumpStmt = "jle end" + to_string(loopCounter) + "\n";
                                if (token.value == "<=")
                                    jumpStmt = "jg end" + to_string(loopCounter) + "\n";
                                if (token.value == ">=")
                                    jumpStmt = "jl end" + to_string(loopCounter) + "\n";
                                stage++;
                            }
                            else
                                stage = -1;
                            break;
                        case 4:
                            if (token.token == IDEN || token.token == IMM)
                            {
                                if (!immUsed && token.token == IMM)
                                    stage = -1;
                                else
                                {
                                    if (token.token == IDEN)
                                        compStmt += "[" + token.value + "]\n" + jumpStmt;
                                    else if (token.token == IMM)
                                        compStmt += token.value + "\n" + jumpStmt;
                                    beforePRBegin = true;
                                    functionsInUse.push("jmp loop" + to_string(loopCounter) + "\n\n"
                                                        "end" + to_string(loopCounter) + ":\n");
                                    stage++;
                                }
                            }
                            else
                                stage = -1;
                            break;
                        case 5:
                            if (token.token == PR_END)
                                stage++;
                            else
                                stage = -1;
                            break;
                        case 6:
                            if (token.token == BR_BEGIN)
                            {
                                aCodeText += compStmt;
                                compStmt = "";
                                jumpStmt = "";
                                beforePRBegin = false;
                                loopCounter++;
                                stage = 0;
                                pathChosen = false;
                            }
                            else
                                stage = -1;
                            break;
                        default:
                            if (beforePRBegin)
                                functionsInUse.push("Filler");
                            compStmt = "";
                            jumpStmt = "";
                            beforePRBegin = false;
                            stage = 0;
                            pathChosen = false;

                            hasError = true;
                            cout << "Error at line " << lineCounter << "\n";
                            break;
                    }
                    break;

                case FOR:
                    switch (stage)
                    {
                        case 0:
                            if (token.token == JUMP)
                                stage++;
                            else
                                stage = -1;
                            break;
                        case 1:
                            if (token.token == PR_BEGIN)
                            {
                                compStmt += "loop" + to_string(loopCounter) + ":\n";
                                stage++;
                            }
                            else
                                stage = -1;
                            break;
                        case 2:
                            if (token.token == IDEN || token.token == DATATYPE)
                            {
                                if (token.token == IDEN)
                                {
                                    endOfLine = "mov ecx, [" + token.value + "]\n";
                                    stage += 2;
                                }
                                else if (token.token == DATATYPE)
                                    stage++;
                            }
                            else
                                stage = -1;
                            break;
                        case 3:
                            if (token.token == IDEN)
                            {
                                aCodeData += token.value + " dword ";
                                endOfLine = "mov ecx, [" + token.value + "]\n";
                                isDeclare = true;
                                stage++;
                            }
                            else
                                stage = -1;
                            break;
                        case 4:
                            if (token.token == EQUAL)
                                stage++;
                            else
                                stage = -1;
                            break;
                        case 5:
                            if (token.token == IDEN || token.token == IMM)
                            {
                                if (isDeclare)
                                {
                                    aCodeData += token.value + "\n";
                                    isDeclare = false;
                                }
                                if (token.token == IDEN)
                                    compStmt = "mov ecx, [" + token.value + "]\n" + compStmt;
                                else
                                    compStmt = "mov ecx, " + token.value + "\n" + compStmt;
                                stage++;
                            }
                            else
                                stage = -1;
                            break;
                        case 6:
                            if (token.token == END)
                            {
                                aCodeText += endOfLine + compStmt;
                                stage++;
                            }
                            else
                                stage = -1;
                            break;
                        case 7:
                            if (token.token == IDEN || token.token == IMM)
                                stage++;
                            else
                                stage = -1;
                            break;
                        case 8:
                            if (token.token == LOGICAL)
                            {
                                aCodeText += "cmp ecx, ";
                                if (token.value == "<")
                                    jumpStmt = "jge end" + to_string(loopCounter) + "\n";
                                if (token.value == ">")
                                    jumpStmt = "jle end" + to_string(loopCounter) + "\n";
                                if (token.value == "<=")
                                    jumpStmt = "jg end" + to_string(loopCounter) + "\n";
                                if (token.value == ">=")
                                    jumpStmt = "jl end" + to_string(loopCounter) + "\n";
                                stage++;
                            }
                            else
                                stage = -1;
                            break;
                        case 9:
                            if (token.token == IDEN || token.token == IMM)
                            {      
                                if (token.token == IDEN)
                                    aCodeText += "[" + token.value + "]\n" + jumpStmt;
                                else if (token.token == IMM)
                                    aCodeText += token.value + "\n" + jumpStmt;
                                endOfFor += "jmp loop" + to_string(loopCounter) + "\n\n"
                                            "end" + to_string(loopCounter) + ":\n";
                                stage++;
                            }
                            else
                                stage = -1;
                            break;
                        case 10:
                            if (token.token == END)
                                stage++;
                            else
                                stage = -1;
                            break;
                        case 11:
                            if (token.token == IDEN)
                                stage++;
                            else
                                stage = -1;
                            break;
                        case 12:
                            if (token.token == OPER)
                            {
                                if (token.value == "++")
                                {
                                    endOfFor = "inc ecx\n" + endOfFor;
                                    stage++;
                                }
                            }
                            else
                                stage = -1;
                            break;
                        case 13:
                            if (token.token == PR_END)
                                stage++;
                            else
                                stage = -1;
                            break;
                        case 14:
                            if (token.token == BR_BEGIN)
                            {
                                compStmt = "";
                                endOfLine = "";
                                jumpStmt = "";
                                functionsInUse.push(endOfFor);
                                endOfFor = "";
                                loopCounter++;
                                stage = 0;
                                pathChosen = false;
                            }
                            else
                                stage = -1;
                            break;
                        default:
                            compStmt = "";
                            endOfLine = "";
                            jumpStmt = "";
                            endOfFor = "";
                            stage = 0;
                            pathChosen = false;

                            hasError = true;
                            cout << "Error at line " << lineCounter << "\n";
                            break;
                    }
                    break;

                case IF:
                    switch (stage)
                    {
                        case 0:
                            if (token.token == JUMP)
                                stage++;
                            else
                                stage = -1;
                            break;
                        case 1:
                            if (token.token == PR_BEGIN)
                                stage++;
                            else
                                stage = -1;
                            break;
                        case 2:
                            if (token.token == IDEN || token.token == IMM)
                            {
                                if (token.token == IDEN)
                                    aCodeText += "mov ecx, [" + token.value + "]\n";
                                else
                                    aCodeText += "mov ecx, " + token.value + "\n";
                                stage++;
                            }
                            else
                                stage = -1;
                            break;
                        case 3:
                            if (token.token == LOGICAL)
                            {
                                aCodeText += "cmp ecx, ";
                                if (token.value == "<")
                                    jumpStmt = "jge else" + to_string(loopCounter) + "\n";
                                if (token.value == ">")
                                    jumpStmt = "jle else" + to_string(loopCounter) + "\n";
                                if (token.value == "<=")
                                    jumpStmt = "jg else" + to_string(loopCounter) + "\n";
                                if (token.value == ">=")
                                    jumpStmt = "jl else" + to_string(loopCounter) + "\n";
                                stage++;
                            }
                            else
                                stage = -1;
                            break;
                        case 4:
                            if ((token.token == IDEN) || (token.token == IMM))
                            {
                                if (token.token == IDEN)
                                    aCodeText += "[" + token.value + "]\n" + jumpStmt;
                                else if (token.token == IMM)
                                    aCodeText += token.value + "\n" + jumpStmt;
                                    beforePRBegin = true;
                                functionsInUse.push("else" + to_string(loopCounter) + ":\n");
                                stage++;
                            }
                            else
                                stage = -1;
                            break;
                        case 5:
                            if (token.token == PR_END)
                                stage++;
                            else
                                stage = -1;
                            break;
                        case 6:
                            if (token.token == BR_BEGIN)
                            {
                                jumpStmt = "";
                                beforePRBegin = false;
                                loopCounter++;
                                stage = 0;
                                pathChosen = false;
                            }
                            else
                                stage = -1;
                            break;
                        default:
                            if (beforePRBegin)
                                functionsInUse.push("Filler");
                            jumpStmt = "";
                            beforePRBegin = false;
                            stage = 0;
                            pathChosen = false;

                            hasError = true;
                            cout << "Error at line " << lineCounter << "\n";
                            break;
                    }
                    break;

                case EXPR:
                    if (stage == 0 && token.token == IDEN)
                    {
                        if (varsUsed[token.value])
                        {
                            endOfLine = "mov [" + token.value + "], eax\n";
                            stage++;
                        }
                    }
                    else if (stage == 1 && token.token == EQUAL)
                    {
                        stage++;
                    }
                    else if (stage == 2 && token.token != END)
                    {
                        if (token.token == IDEN)
                        {
                            if (!varsUsed[token.value])
                            {
                                stage += 2;
                                return;
                            }
                        }
                        string left; 
                        string right; 
                        if (currLine.length() == 0)
                        {
                            left = "";
                            right = "";
                        }

                        else
                        {
                            switch(value.at(0))
                            {
                                case '+':
                                    exprIndex = currLine.length();
                                    break;
                                case '-':
                                    exprIndex = mdIndex;
                                    break;
                            }
                            left = currLine.substr(0,exprIndex);
                            right = currLine.substr(exprIndex, currLine.length());
                        }

                        
                        // cout << left << "~~~\n" << right << endl;
                        // cout << currLineIndex << endl << endl;

                        switch (token.token)
                        {
                            case IDEN:
                                if (needOper)
                                {
                                    stage = -1;
                                    return;
                                }
                                else
                                {
                                    secondVal = ~secondVal;
                                    currLine = left + "push [" + token.value + "]\n" + right;
                                    exprIndex = (left + "push [" + token.value + "]\n").length();
                                    mdIndex += ("push [" + token.value + "]\n").length();
                                    needOper = true;
                                }
                                break;
                            case IMM:
                                if (needOper)
                                {
                                    stage = -1;
                                    return;
                                }
                                else
                                {
                                    secondVal = ~secondVal;
                                    currLine = left + "push " + token.value + "\n" + right;
                                    exprIndex = (left + "push " + token.value + "\n").length();
                                    mdIndex += ("push " + token.value + "\n").length();
                                    needOper = true;
                                }
                                break;
                            case OPER:
                                if (!needOper)
                                {
                                    stage = -1;
                                    return;
                                }
                                else
                                {
                                    needOper = false;
                                    noOper = false;
                                    switch(token.value.at(0))
                                    {
                                        case '+':
                                            exprIndex = left.length();
                                            currLine = left + "pop ebx\n"
                                                        "pop eax\n"
                                                        "add eax, ebx\n"
                                                        "push eax\n" + right;
                                            break;
                                        case '-':
                                        exprIndex = left.length();
                                        currLine = left + "pop ebx\n"
                                                    "pop eax\n"
                                                    "sub eax, ebx\n"
                                                    "push eax\n" + right;
                                        break;
                                        case '*':
                                        exprIndex = left.length();
                                        currLine = left + "pop ebx\n"
                                                    "pop eax\n"
                                                    "mul ebx\n"
                                                    "push eax\n" + right;
                                        mdIndex = (left + "pop ebx\n"
                                                    "pop eax\n"
                                                    "mul ebx\n"
                                                    "push eax\n").length();
                                        break;
                                        case '/':
                                        exprIndex = left.length();
                                        currLine = left + "pop ebx\n"
                                                    "pop eax\n"
                                                    "div ebx\n"
                                                    "push eax\n" + right;
                                        mdIndex = (left + "pop ebx\n"
                                                    "pop eax\n"
                                                    "div ebx\n"
                                                    "push eax\n").length();
                                        break;
                                    }
                                }
                                break;
                        }
                    }
                    else if (stage == 2 && token.token == END)
                    {
                        if (noOper)
                        {
                            currLine += "pop eax\n";
                        }
                        aCodeText += currLine + endOfLine + "\n";

                        // Reset everything
                        currLine = "";
                        endOfLine = "";
                        exprIndex = 0;
                        mdIndex = 0;
                        secondVal = false;
                        needOper = false;

                        stage = 0;
                        pathChosen = false;
                    }
                    else
                    {
                        currLine = "";
                        endOfLine = "";
                        exprIndex = 0;
                        mdIndex = 0;
                        secondVal = false;
                        needOper = false;
                        stage = 0;
                        pathChosen = false;

                        hasError = true;
                        cout << "Error at line " << lineCounter << "\n";
                    }
                    break;

                case RETURN:
                    if (stage == 0 && token.token == RET)
                    {
                        aCodeText += "Invoke ExitProcess, 0\n";
                        stage++;
                    }
                    else if (stage == 1 && (token.token == END || token.token == IDEN || token.token == IMM))
                    {
                        if (token.token == END)
                        {
                            stage = 0;
                            pathChosen = false;
                        }
                        else
                            stage ++;
                    }
                    else if (stage == 2 && token.token == END)
                    {
                        stage = 0;
                        pathChosen = false;
                    }
                    else
                    {
                        hasError = true;
                        cout << "Error at line " << lineCounter << "\n";
                        stage = 0;
                        pathChosen = false;
                    }
            }
        }
    }
}

void File::print()
{
    if (!hasError)
        cout << aCodeData + "\n" + aCodeText + "END MAIN\n\n";
    else
        cout << "Errors present, no code generated\n\n";
}

Token File::assignToken(string value)
{
    Token temp;
    map<string, TokenType> tokenMap = {
        {"int", DATATYPE}, {"char", DATATYPE}, {"short", DATATYPE}, {"long", DATATYPE}, {"float", DATATYPE}, {"double", DATATYPE}, 
        {"+", OPER}, {"-", OPER}, {"*", OPER}, {"/", OPER}, {"++", OPER},
        {"while", JUMP}, {"for", JUMP}, {"if", JUMP},
        {"<", LOGICAL}, {">", LOGICAL}, {"<=", LOGICAL}, {">=", LOGICAL},
        {"++", INCDEC}, {"--", INCDEC},
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

bool File::isInt(string value)
{
    for (int i = 0; i < value.length(); i++)
    {
        if (!isdigit(value[i]))
            return false;
    }
    return true;
}

bool File::isIden(string value)
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