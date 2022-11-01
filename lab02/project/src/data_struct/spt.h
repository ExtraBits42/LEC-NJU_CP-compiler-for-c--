/*
模块：
    Name: Syntax Parsing Tree-语法分析树
*/

#pragma once
#include"../general.h"

// 构建语法分析树
void add_children(int num, ...);
void add_child(Node* root, Node* p);

// 构建语法分析树节点
Node* build_lexical_node(char* n, char* v);
Node* build_syntax_node(char* n, YYLTYPE location);

// 打印语法分析树
void print_tree(Node* root, int blank_num);


