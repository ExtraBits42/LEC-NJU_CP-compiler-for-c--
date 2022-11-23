#pragma once

#include"type_node.h"

typedef struct TypeNode TypeNode;

typedef enum SymType {VAR_SYM, STRUCT_SYM, FUNC_SYM}SymType;

typedef struct SymNode{
    char* name;
    enum SymType ntype;
    struct{
        struct TypeNode* vtype;
    } var_v;
    struct{
        int mem_num;
        struct SymNode* members;
    } struct_v;
    struct
    {
        struct TypeNode* rtype;    // return type
        int param_num;
        struct SymNode* params;
    } func_v;
    struct SymNode* hash_next;
} SymNode;


// 基本初始化
SymNode* new_sym_node(char* name);
// 克隆符号节点
SymNode* clone_sym_node(SymNode* ori_node);
// 销毁/回收符号节点
void destory_sym_node(SymNode* node);

// 新建变量-符号节点
SymNode* new_var_sym_node(char* name, TypeNode* type_node);
// 新建结构体定义-符号节点
SymNode* new_struct_node(char* name, int mem_num, SymNode* members);
// 新建函数定义-符号节点
SymNode* new_func_node(char* name, TypeNode* rtype, int param_num, SymNode* params);



