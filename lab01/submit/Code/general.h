#pragma once
#include<stdarg.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>


# define YYLTYPE_IS_DECLARED 1
typedef struct YYLTYPE  
{  
    int first_line;  
    int first_column;  
    int last_line;  
    int last_column;  
} YYLTYPE;

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

void add_child(Node* root, Node* p);
void add_children(int num, ...);
Node* build_lexical_node(char* n, char* v);
Node* build_syntax_node(char* n, YYLTYPE location);
void print_tree(Node* root, int blank_num);

