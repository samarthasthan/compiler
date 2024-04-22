%{
#include <stdio.h>
%}
 
%%
 
[ \t\n]            ; // Ignore whitespace and newline characters
 
[0-9]+             { printf("NUMBER: %s\n", yytext); } // Match integers
"+"                { printf("ADD\n"); }                // Match addition operator
"-"                { printf("SUBTRACT\n"); }          // Match subtraction operator
"*"                { printf("MULTIPLY\n"); }          // Match multiplication operator
"/"                { printf("DIVIDE\n"); }            // Match division operator
"("                { printf("LEFT_PAREN\n"); }        // Match left parenthesis
")"                { printf("RIGHT_PAREN\n"); }       // Match right parenthesis
 
.                  { printf("UNKNOWN CHARACTER: %s\n", yytext); } // Catch anything else
 
%%
 
int main(int argc, char** argv) {
   yyin = stdin;
   yylex();
   return 0;
}