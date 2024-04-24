%{
#include "y.tab.h"
%}

%%

[0-9]+          { yylval = atoi(yytext); return NUMBER; }
[-+*/^\n]       { return *yytext; }
[ \t]           ; // ignore whitespace
.               ;

%%

int yywrap() {
    return 1;
}
