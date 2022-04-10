%option noyywrap
%{
#include "o.h"
#include "o.tab.h"
%}

EXP ([Ee][+-]?[0-9]+)

%%
"+" |
"-" |
"*" |
"/" |
"(" |
")" {return yytext[0];}

[0-9]+"."[0-9]*{EXP}? |
"."?[0-9]+{EXP}? {yylval.d=atof(yytext); return NUMBER;}
\n {return EOL;}
. {}
%%

