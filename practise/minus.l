%{
#include<iostream>
#include<cstdlib>
using namespace std;
int current_number = 0;
bool expecting_number = true;
int current_sum = 0;

#define NEWLINE_TOKEN 1000
%}


%%
[a-zA-Z] {
    cout << "Found a word" << endl;
}

[0-9]+ {
    current_number = atoi(yytext);

    if (expecting_number){
        current_sum+=current_number;
        cout<<"Found a number"<<endl;
        expecting_number = false; 
    }
    else{
        cerr << "Unexpected number: " << yytext << endl;
    }
}

\- {
    if (!expecting_number) {
        cout << "Found a '+' operator" << endl;
        expecting_number = true; // Now expecting a number
    } else {
        // If we were not expecting an operator, print an error message
        cerr << "Unexpected '+' operator" << endl;
    }
}

\n {
    return NEWLINE_TOKEN;
}

[ \t]+ {

}

. {
    // Handle unknown characters
    cerr << "Unknown character: " << yytext << endl;
}
%%

int main(){
    int token;

    while((token=yylex()!=NEWLINE_TOKEN)){

    }
    cout << "The total sum is: " << current_sum << endl;
    return 0;
}

int yywrap(){
    return 1;
}