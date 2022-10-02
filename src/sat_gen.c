#include"sat_gen.h"
#include<stdio.h>

extern int yylineno;

inline void add_child(Node* root, Node* p){
    if(root->child == NULL){
        root->child = p;
        root->tail = p;
    }
    else{
        root->tail->next = p;
        root->tail = p;
    }
}

void add_children(int num, ...){
    va_list valist;
    va_start(valist, num);
    Node* root;
    for(int i = 0; i < num; i++){
        if(i == 0) root = va_arg(valist, Node*);
        else{
            Node* p = va_arg(valist, Node*);
            add_child(root, p);
        }
    }
    va_end(valist);
}

void set_node(Node* root, char* n){
    root->ntype = SYNTAX;
    root->name = n;
    root->u.syn_line = yylineno;
}