#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include"o.h"

int node_num=0;

struct ast* newast(int nodetype, struct ast *l, struct ast *r){
    struct ast *a = malloc(sizeof(struct ast));
    if(!a){
        yyerror("out of space");
        exit(0);
    }
    a->nodetype=nodetype;
    a->l=l;
    a->r=r;
    return a;
}

struct ast* newnum(double d){
    struct numval *a = malloc(sizeof(struct numval));
    if(!a){
        yyerror("out of space");
        exit(0);
    }
    a->nodetype='K';
    a->number=d;
    return (struct ast*)a;
}

double eval(struct ast *a){
    double v;
    switch(a->nodetype){
        case 'K': v=((struct numval *)a)->number;break;
        case '+': v=eval(a->l)+eval(a->r);break;
        case '-': v=eval(a->l)-eval(a->r);break;
        case '*': v=eval(a->l)*eval(a->r);break;
        case '/': v=eval(a->l)/eval(a->r);break;
        case 'M': v=-1*eval(a->l);break;
        default: printf("calculation error\n");
    }
    return v;
}

void freetree(struct ast *a){
    switch(a->nodetype){
        case '+':
        case '-':
        case '*':
        case '/':
            freetree(a->r);
        case 'M':
            freetree(a->l);
        case 'K':
            free(a);
            break;
        default: printf("free error\n");       
    }
}

void preorder(struct ast *a){
    switch(a->nodetype){
        case 'K': printf(" %.4g ",((struct numval *)a)->number);return;
        case 'M': printf("-");preorder(a->l);break;
        case '+': 
        case '-': 
        case '*': 
        case '/': printf("%c",a->nodetype);preorder(a->l);preorder(a->r);break;
        default: printf("visit error\n");
    }   
}

void postorder(struct ast *a){
    switch(a->nodetype){
        case 'K': printf(" %.4g ",((struct numval *)a)->number);return;
        case 'M': printf("-");postorder(a->l);break;
        case '+': 
        case '-': 
        case '*': 
        case '/': postorder(a->l);postorder(a->r);printf("%c",a->nodetype);break;
        default: printf("visit error\n");
    }   
}

void print_tree_prob(struct ast *a){//先跑一遍确定编号值
    node_num+=1;
    switch(a->nodetype){
        case 'K': 
            return;
        case 'M': 
            print_tree_prob(a->l);
            break;
        case '+': 
        case '-': 
        case '*': 
        case '/': 
            print_tree_prob(a->l);
            print_tree_prob(a->r);
            break;
        default: printf("visit error\n");
    }
}

void print_node(struct ast *a, FILE *f, int num){
    switch(a->nodetype){
        case 'K':
            fprintf(f,"node[shape=record,style=filled,color=red,fontcolor=white];\n"); 
            fprintf(f,"%d[label=\"%.4g\"];\n", num, ((struct numval *)a)->number);
            return;
        case 'M': 
            fprintf(f,"node[shape=record,style=filled,color=red,fontcolor=white];\n");
            fprintf(f,"%d[label=\"-\"];\n", num);
            return;
        case '+': 
        case '-': 
        case '*': 
        case '/': 
            fprintf(f,"node[shape=ellipse,style=filled,color=black,fontcolor=white];\n");
            fprintf(f,"%d[label=\"%c\"];\n", num, a->nodetype); 
            return;
        default: return;
    }
}

void print_tree_node(struct ast *a, FILE *f){
    node_num+=1;
    int current_node_num=node_num;
    switch(a->nodetype){
        case 'K': 
            //print_node(a, f, node_num);        
            return;
        case 'M': 
            print_node(a, f, node_num);      
            fprintf(f,"%d:sw->%d;\n", node_num, node_num+1);
            print_tree_node(a->l, f);
            break;
        case '+': 
        case '-': 
        case '*': 
        case '/':   
            print_node(a, f, node_num);
            print_node(a->l, f, node_num+1);
            fprintf(f,"%d:sw->%d;\n", node_num, node_num+1); 
            print_tree_prob(a->l);
            print_node(a->r, f, node_num+1);
            fprintf(f,"%d:se->%d;\n", current_node_num, node_num+1);
            node_num=current_node_num;

            print_tree_node(a->l, f);           
            print_tree_node(a->r, f);
            break;
        default: printf("visit error\n");
    }

}

void print_tree(struct ast *a){
    FILE *f;
    f = fopen("ast.dot","w+");
    fprintf(f,"digraph G{\n");
    print_tree_node(a,f);
    fprintf(f,"}");
    fclose(f);
    system("dot -Tpng ast.dot -o ast.png");
}