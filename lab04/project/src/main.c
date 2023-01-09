#include<stdio.h>
#include<stdlib.h>
#include"general.h"
#include"utils/utils.h"

#include"data_struct/spt.h"

#include"data_struct/sym_table.h"
#include"node_anls/semantic_anls.h"

#include"data_struct/ir_list.h"
#include"ir_gen/ir_gen.h"

#include"mips_gen/mips_gen.h"

#define GEN_MIPS_CODE

/*declarations*/
extern int yyleng;
extern char* yytext;
extern FILE* yyin;
extern int yylex(void);
extern int yyrestart(FILE* f);
extern int yyparse();

/*global variables*/
int pass = 1;               // 标识：1-通过编译，0-未通过编译，出现错误

Node *root = NULL;          // 词法分析&语法分析 : 语法分析树根节点

SymTable* table = NULL;     // 语义分析 : 符号表

IrStmt* irlist_head = NULL; // 中间代码生成
IrStmt* irlist_tail = NULL;

int main(int argc, char** argv){
    /*main function*/
    /*Step00. Get C-- Source File*/
    if(argc <= 1) return 1;
    FILE* f = fopen(argv[1], "r");
    if(!f){
        perror(argv[1]);
        return 1;
    }

    /*Step01. Lexical Analysis、Syntax Parsing*/
    yyrestart(f);
    yyparse();
    if(pass == 0){
        print_tree(root, 0);
        return 0;
    }

    /*Step02. Semantic Analysis*/
    semantic_anls(root);

    /*Step03. Generate Intermediate Code*/
    // FILE* ir_file = fopen(argv[2], "w");
    FILE* ir_file = fopen("./guigui.s", "w");
    if(!ir_file){
        perror(argv[2]);
        return 1;
    }
    ir_gen(ir_file, root);
    fclose(ir_file);

    /*Step04. Generate MIPS Code*/
#ifdef GEN_MIPS_CODE
    // FILE* mips_file = fopen("./guigui1.s", "w");
    FILE* mips_file = fopen(argv[2], "w");
    if(!mips_file){
        perror(argv[2]);
        return 1;
    }
    mips_gen(mips_file);
    fclose(mips_file);
#endif
    return 0;
}
