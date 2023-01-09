#include"../general.h"

#include"spt.h"

extern int yylineno;

void add_children(int num, ...){
    /*用于Bison：将产生式右侧所有节点作为孩子追加到产生式左侧节点*/
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

void add_child(Node* root, Node* p){
    /*对节点root添加子结点p*/
    if(root->children == NULL){
        root->children = p;
        root->tail = p;
    }
    else{
        root->tail->next = p;
        root->tail = p;
    }
}

Node* build_lexical_node(char* n, char* v){
    /*初始化终结符号节点*/
    Node* root = (Node *)malloc(sizeof(Node));
    root->ntype = LEXICAL;
    STRCP(root->name, n);
    STRCP(root->u.lex_val, v);
    root->children = NULL;
    root->tail = NULL;
    root->next = NULL;
    return root;
}

Node* build_syntax_node(char* n, YYLTYPE location){
    /*初始化非终结符号节点*/
    Node* root = (Node *)malloc(sizeof(Node));
    root->ntype = SYNTAX;
    root->name = n;
    root->u.syn_line = location.first_line;
    root->children = NULL;
    root->tail = NULL;
    root->next = NULL;
    return root;
}
