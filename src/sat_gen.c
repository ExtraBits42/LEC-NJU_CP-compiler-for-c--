#include"sat_gen.h"

extern int yylineno;

int int_o2d(char *v){
    /*整数：八进制转十进制*/
    int len = strlen(v);
    int ans = 0;
    for(int i = 1; i < len; i++){
        ans = (ans << 3) + (v[i] - '0');
    }
    return ans;
}

int int_h2d(char *v){
    /*整数：十六进制转十进制*/
    int len = strlen(v);
    int ans = 0;
    for(int i = 2; i < len; i++){
        int tmp = 0;
        if(v[i] >= '0' && v[i] <= '9') tmp = v[i] - '0';
        else if(v[i] >= 'a' && v[i] <= 'f') tmp = 10 + v[i] - 'a';
        else if(v[i] >= 'A' && v[i] <= 'F') tmp = 10 + v[i] - 'A';
        ans = (ans << 4) + tmp;
    }
    return ans;
}

inline void add_child(Node* root, Node* p){
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

Node* build_lexical_node(char* n, char* v){
    /*初始化终结符号节点*/
    Node* root = (Node *)malloc(sizeof(Node));
    root->ntype = LEXICAL;
    root->name = (char *)malloc(sizeof(n));
    strcpy(root->name, n);
    root->u.lex_val = (char *)malloc(sizeof(v));
    strcpy(root->u.lex_val, v);
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

void print_tree(Node* root, int blank_num){
    /*打印语法分析树*/
    if(root->ntype == LEXICAL){
        for(int i = 0; i < blank_num; i++) printf(" ");
        if(strcmp(root->name, "ID") == 0) printf("ID: %s\n", root->u.lex_val);
        else if(strcmp(root->name, "TYPE") == 0) printf("TYPE: %s\n", root->u.lex_val);
        else if(strcmp(root->name, "INT10") == 0) printf("INT: %d\n", atoi(root->u.lex_val));
        else if(strcmp(root->name, "INT8") == 0) printf("INT: %d\n", int_o2d(root->u.lex_val));
        else if(strcmp(root->name, "INT16") == 0) printf("INT: %d\n", int_h2d(root->u.lex_val));
        else if(strcmp(root->name, "FLOAT") == 0) printf("FLOAT: %f\n", atof(root->u.lex_val));
        else printf("%s\n", root->name);
    }
    else if(root->ntype == SYNTAX){
        if(root->children != NULL){
            for(int i = 0; i < blank_num; i++) printf(" ");
            printf("%s (%d)\n", root->name, root->u.syn_line);
        }
        Node* cursor = root->children;
        while(cursor != NULL){
            print_tree(cursor, blank_num+2);
            cursor = cursor->next;
        }
    }
}