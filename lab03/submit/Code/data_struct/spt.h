/*
模块：
    Name: Syntax Parsing Tree-语法分析树
*/

#pragma once

/*语法分析-构建语法分析树*/
typedef enum{LEXICAL, SYNTAX}Ntype;
typedef struct TreeNode{
    Ntype ntype;
    char* name;
    union {
        int syn_line;
        char* lex_val;
    }u;
    struct TreeNode* children;
    struct TreeNode* tail;
    struct TreeNode* next;
} Node;

// 构建语法分析树
void add_children(int num, ...);
void add_child(Node* root, Node* p);

// 构建语法分析树节点
Node* build_lexical_node(char* n, char* v);
Node* build_syntax_node(char* n, YYLTYPE location);



