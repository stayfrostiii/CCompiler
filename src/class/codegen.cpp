#include <iostream>
#include <string>
#include <vector>
#include "../header/stmt.h"

using namespace std;

// Global counter for unique labels
static int label_counter = 0;

// Helper function to generate unique labels
string generate_label() {
    return "L" + to_string(label_counter++);
}

// Generate assembly for IF statement
void generate_if(ASTNode* node) {
    string false_label = generate_label(); // Label for false block
    string end_label = generate_label();   // Label for end of IF

    // Condition
    cout << "; IF condition" << endl;
    node->children[0]->stmt->generateAssembly(); // Generate code for condition
    cout << "CMP R0, 0" << endl;                // Compare result to 0
    cout << "JE " << false_label << endl;       // Jump to false block if condition is false

    // True block
    cout << "; True block" << endl;
    if (node->children[1]) {
        generate_code(node->children[1]); // Generate code for true block
    }
    cout << "JMP " << end_label << endl; // Skip the false block

    // False block
    cout << false_label << ":" << endl;
    if (node->num_children == 3) { // False block exists
        cout << "; False block" << endl;
        generate_code(node->children[2]);
    }

    // End of IF
    cout << end_label << ":" << endl;
}

// Generate assembly for WHILE loop
void generate_while(ASTNode* node) {
    string start_label = generate_label(); // Label for loop start
    string end_label = generate_label();   // Label for loop end

    // Start label
    cout << start_label << ":" << endl;

    // Condition
    cout << "; WHILE condition" << endl;
    node->children[0]->stmt->generateAssembly(); // Generate code for condition
    cout << "CMP R0, 0" << endl;                // Compare result to 0
    cout << "JE " << end_label << endl;         // Exit loop if condition is false

    // Loop body
    cout << "; Loop body" << endl;
    if (node->children[1]) {
        generate_code(node->children[1]); // Generate code for loop body
    }
    cout << "JMP " << start_label << endl; // Jump back to start of loop

    // End label
    cout << end_label << ":" << endl;
}

// Generate assembly for FOR loop
void generate_for(ASTNode* node) {
    string start_label = generate_label(); // Label for loop start
    string end_label = generate_label();   // Label for loop end

    // Initialization
    cout << "; FOR initialization" << endl;
    if (node->children[0]) {
        node->children[0]->stmt->generateAssembly(); // Generate initialization code
    }

    // Start label
    cout << start_label << ":" << endl;

    // Condition
    cout << "; FOR condition" << endl;
    if (node->children[1]) {
        node->children[1]->stmt->generateAssembly(); // Generate condition code
    }
    cout << "CMP R0, 0" << endl;                   // Compare result to 0
    cout << "JE " << end_label << endl;            // Exit loop if condition is false

    // Loop body
    cout << "; Loop body" << endl;
    if (node->children[3]) {
        generate_code(node->children[3]); // Generate code for loop body
    }

    // Increment
    cout << "; FOR increment" << endl;
    if (node->children[2]) {
        node->children[2]->stmt->generateAssembly(); // Generate increment code
    }
    cout << "JMP " << start_label << endl; // Jump back to start of loop

    // End label
    cout << end_label << ":" << endl;
}

// Main code generation function
void generate_code(ASTNode* node) {
    if (!node) return;

    switch (node->type) {
        case NODE_IF:
            generate_if(node);
            break;
        case NODE_WHILE:
            generate_while(node);
            break;
        case NODE_FOR:
            generate_for(node);
            break;
        case NODE_ASSIGN:
        case NODE_EXPRESSION:
            node->stmt->generateAssembly();
            break;
        default:
            cout << "; Unknown node type" << endl;
            break;
    }
}

