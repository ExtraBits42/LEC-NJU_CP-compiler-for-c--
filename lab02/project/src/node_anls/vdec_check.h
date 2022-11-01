/*
模块:
    Name: Variable Declaration Check-变量定义检查
*/

# pragma once
#include"../general.h"

static int annoy_struct_index = 0;

//顶层函数-变量定义分析
void vdecs_anls(Node* extdef, int* pass);

//节点分析函数
char* vdecs_specifier_anls(Node* specifier, int* pass);
void vdecs_extdeclist_anls(char* type, Node* extdeclist, int* pass);
SymNode* vdecs_vardec_anls(char* type, Node* vardec, int* pass);

//辅助函数
int vdecs_if_type_exist(char* type);