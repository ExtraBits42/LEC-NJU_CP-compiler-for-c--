#pragma once
#include<stdarg.h>

typedef struct TreeNode{
    enum {LEXICAL, SYNTAX}ntype;
    char* name;
    union {
        int syn_line;
        char* lex_val;
    }u;
    struct TreeNode* child;
    struct TreeNode* tail;
    struct TreeNode* next;
} Node;

void add_child(Node* root, Node* p);
void add_children(int num, ...);
void set_node(Node* root, char* n);