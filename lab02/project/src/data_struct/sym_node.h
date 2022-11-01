/*
模块：
    Name: Symbol Node-符号表表项节点
*/

#pragma once
#include"../general.h"

// 新建符号节点
// Basis: 初始化基础符号节点
SymNode* new_sym_node();
// Induction: 新建变量/结构体类型/函数符号节点
SymNode* new_var_sym_node(char* v_name, TypeNode* v_type);
SymNode* new_struct_sym_node(char* s_name);
SymNode* new_func_sym_node(char* f_name);

// 克隆符号节点
SymNode* clone_var_sym_node(SymNode* ori_node);

// 删除符号节点
void free_sym_node(SymNode* node);