%{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
int yylex(void);
int yyerror(const char *s);
%}

%token NUMBER

%%

input: /* empty */
     | input line
     ;

line: '\n'
    | exp '\n' { printf("Result: %d\n", $1); }
    ;

exp: term
   | exp '+' term { $$ = $1 + $3; }
   | exp '-' term { $$ = $1 - $3; }
   ;

term: factor
    | term '*' factor { $$ = $1 * $3; }
    | term '/' factor { $$ = $1 / $3; }
    ;

factor: primary
       | primary '^' factor { $$ = pow($1, $3); }        ;

primary: NUMBER
        | '-' NUMBER { $$ = -$2; } // Unary minus
        | '(' exp ')' { $$ = $2; }
        ;

%%

int yyerror(const char *s) {
    printf("Error: %s\n", s);
    return 0;
}

int main() {
        yyparse();
    return 0;
}
