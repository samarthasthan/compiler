%{
#include <iostream>
#include <cstdlib>  // For atoi function

using namespace std;

int current_sum = 0; // Holds the current sum
bool expecting_number = true; // To keep track of expected input (number or operator)
int current_number = 0; // Holds the current number

// Define token constants for better readability
#define NEWLINE_TOKEN 1000
%}

%%
[0-9]+ {
    // Convert the matched number from string to integer
    current_number = atoi(yytext);
    
    // If we were expecting a number, add it to the current sum
    if (expecting_number) {
        current_sum += current_number;
        cout << "Found a number: " << yytext << endl;
        expecting_number = false; // Now expecting an operator
    } else {
        // If we were not expecting a number, print an error message
        cerr << "Unexpected number: " << yytext << endl;
    }
}

\+ {
    // Check if we were expecting an operator
    if (!expecting_number) {
        cout << "Found a '+' operator" << endl;
        expecting_number = true; // Now expecting a number
    } else {
        // If we were not expecting an operator, print an error message
        cerr << "Unexpected '+' operator" << endl;
    }
}

\n {
    // On a newline character, stop the lexer
    // by returning a special token to indicate end of input
    return NEWLINE_TOKEN;
}

[ \t]+ {
    // Ignore other whitespace characters (spaces, tabs)
}

. {
    // Handle unknown characters
    cerr << "Unknown character: " << yytext << endl;
}
%%

int main() {
    int token;

    // Read tokens from the lexer
    while ((token = yylex()) != NEWLINE_TOKEN) {
        // Continue scanning input until a newline character is encountered
        // and a special token is returned
    }
    
    // Output the final sum
    cout << "The total sum is: " << current_sum << endl;
    
    return 0;
}

int yywrap() {
    // Return 1 to signal end of input
    return 1;
}
