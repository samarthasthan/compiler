%{ 
// Include any C/C++ headers or declarations
#include <iostream>
%}


%%
[0-9]+    { std::cout << "Found a number: " << yytext << std::endl; }
[a-zA-Z]+ { std::cout << "Found an identifier: " << yytext << std::endl; }
%%  


int main() {
    yylex();  // Call the generated lexer
    return 0;
}

int yywrap(){
    return 0;
}