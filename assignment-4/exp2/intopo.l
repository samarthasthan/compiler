%{
#include "y.tab.h"
extern int yylval;
%}

%%

[0-9]+  { yylval = atoi(yytext); return NUM; }
[a-zA-Z] { yylval = *yytext; return IDENTIFIER; }
\n      return 0;
.       return *yytext;

%%

int yywrap() {
    return 1;
}

