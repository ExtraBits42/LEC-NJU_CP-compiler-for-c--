#include<stdio.h>
#include<stdlib.h>
#include"general.h"

/*declarations*/
extern int yyleng;
extern char* yytext;
extern FILE* yyin;
extern int yylex(void);
extern int yyrestart(FILE* f);
extern int yyparse();

/*global variables*/
int pass = 1;               // 标识：1-通过编译，0-未通过编译，出现错误
Node *root = NULL;          // 语法分析树根节点

/*main function*/
int main(int argc, char** argv){
    pass = 1;

    if(argc <= 1) return 1;
    FILE* f = fopen(argv[1], "r");
    if(!f){
        perror(argv[1]);
        return 1;
    }
    yyrestart(f);
    yyparse();
    if(pass == 1){
        print_tree(root, 0);
    }
    return 0;
}

