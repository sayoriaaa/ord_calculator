%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "o.h"

int control=1;
%}

%union{
    struct ast *a;
    double d;
}

%token <d> NUMBER
%token EOL

%type <a> exp term factor


%%
calclist:
| calclist exp EOL {
    if(control==1) printf("%.4g\n",eval($2));
    if(control==2) preorder($2);
    if(control==3) postorder($2);
    if(control==4) print_tree($2);
    freetree($2);
}

exp: term
| exp '+' term { $$=newast('+',$1,$3); }
| exp '-' term { $$=newast('-',$1,$3); }
term: factor
| term '*' factor { $$=newast('*',$1,$3); }
| term '/' factor { $$=newast('/',$1,$3); }
factor: NUMBER {$$=newnum($1);}
| '(' exp ')' {$$=$2;}
| '-' factor { $$=newast('M',$2,NULL); } 
%%

void yyerror(char *s){
    printf("parse error %s",s);
}

int main(int argc, char **argv){
    control=atoi(argv[1]);
    for(int i=1; i<argc; i++){
        if(strcmp(argv[i], "-pre")==0){
            control=2;
        }
        if(strcmp(argv[i], "-post")==0){
            control=3;
        }
        if(strcmp(argv[i], "-d")==0){
            control=4;
        }
        if(strcmp(argv[i], "-v")==0){//版本信息
            printf("ord version 1.0.0\nauthor sayoriaaa\n");
            return 0;
        }
        if(strcmp(argv[i], "-h")==0){//帮助
            printf("initialization:\nno additional inf: use as ordinary calculator\n-pre: output prefix expression\n-post: output postfix expression\n-d: draw expression tree");
            return 0;
        }
    }
    return yyparse();
}

