	%{
#include <stdio.h>
%}

%token NUM
%token IDENTIFIER

%left '+' '-'
%left '*' '/'
%right NEGATIVE

%%

S:  E { printf("\n"); }
    ;

E:  E '+' E { printf("+"); }
    |   E '*' E { printf("*"); }
    |   E '-' E { printf("-"); }
    |   E '/' E { printf("/"); }
    |   '(' E ')' {}
    |   '-' E %prec NEGATIVE { printf("-"); }
    |   NUM     { printf("%d", yylval); }
    |   IDENTIFIER { printf("%c", yylval); }
    ;

%%

int main() {
    printf("Enter the expression: ");
    yyparse();
}

int yyerror (char *msg) {
    return printf ("error YACC: %s\n", msg);
}

