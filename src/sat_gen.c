#include"sat_gen.h"



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

Node* set_node(char* n){
    Node* root = (Node *)malloc(sizeof(Node));
    root->ntype = SYNTAX;
    root->name = n;
    root->u.syn_line = yylineno;
    root->child = NULL;
    root->tail = NULL;
    root->next = NULL;
    return root;
}

void print_tree(Node* root, int blank_num){
    for(int i = 0; i < blank_num; i++) printf(" ");
    if(root->ntype == LEXICAL){
        if(root->name == "ID") printf("ID: %s\n", root->u.lex_val);
        else if(root->name == "TYPE") printf("TYPE: %s\n", root->u.lex_val);
        else if(root->name == "INT") printf("INT: %s\n", root->u.lex_val);
        else if(root->name == "FLOAT") printf("FLOAT: %s\n", root->u.lex_val);
        else printf("%s\n", root->name);
    }
    else if(root->ntype == SYNTAX){
        printf("%s (%d)\n", root->name, root->u.syn_line);
        Node* cursor = root->child;
        while(cursor != NULL){
            print_tree(cursor, blank_num+2);
            cursor = cursor->next;
        }
    }
}