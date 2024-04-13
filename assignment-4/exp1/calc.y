%{
#include <stdio.h>
int yylex();
void yyerror(const char *);
%}

%token NUMBER

%%
calc:   /* empty */
        | calc expr '\n' { printf("= %d\n", $2); }
        ;

expr:   NUMBER            { $$ = $1; }
        | expr '+' expr   { $$ = $1 + $3; }
        | expr '-' expr   { $$ = $1 - $3; }
        | expr '*' expr   { $$ = $1 * $3; }
        | expr '/' expr   { 
            if ($3 == 0) {
                yyerror("Division by zero");
            } else {
                $$ = $1 / $3;
            }
        }
        | '(' expr ')'    { $$ = $2; }
        ;

%%

void yyerror(const char *s) {
    printf("Error: %s\n", s);
}

int main() {
    yyparse();
    return 0;
}
