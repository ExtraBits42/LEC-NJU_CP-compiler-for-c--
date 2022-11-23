#pragma once

#include"sym_node.h"

typedef struct SymNode SymNode;

typedef enum TypeType{STRUCT_T, ARRAY_T, BASIC_T}TypeType;

#define VARIABLE 1
#define CONSTANT 0

typedef struct TypeNode{
    int if_const;   // 1-变量；0-常量
    enum TypeType ntype;
    struct{
        struct SymNode* def_ptr;
    } struct_v;
    struct{
        int size;
        struct TypeNode* ele_type;
    } array_v;
    struct
    {
        char* basic_type;
    } basic_v;
}TypeNode;


// 基本初始化
TypeNode* new_type_node();
// 克隆节点
TypeNode* clone_type_node(TypeNode* ori_node);
// 销毁节点
void destory_type_node(TypeNode* node);

// 新建符号-结构类型
TypeNode* new_struct_type_node(SymNode* struct_def, int if_const);
// 新建符号-数组类型
TypeNode* new_array_type_node(TypeNode* ele_t, int size, int if_const);
// 新建符号-基本类型
TypeNode* new_basic_type_node(char* basic_type, int if_const);


// 打印符号节点
void printf_type_node(TypeNode* type);