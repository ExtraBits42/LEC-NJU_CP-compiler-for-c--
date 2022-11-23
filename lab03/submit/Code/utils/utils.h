/*
模块:
    Name: Utiliys-多功能工具包
*/

#pragma once

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"../general.h"

#include"../data_struct/sym_node.h"

// 最大值/最小值
int max(int a, int b);
int min(int a, int b);

// 获取int型数字在十进制表示下的长度
int get_int_len(int n);
// 创建序列化命名字符串: <指定的变量名><数字序号>
char* new_name_xxx(int n, char* vname);
// 整数转换为字符串数字 : xxx->"xxx"
char* int2str(int n);
// 整数转换为字符串立即数 : xxx->"#xxx"
char* int2_imstr(int n);

// 获取数组类型大小
int get_array_size(TypeNode* arrayt);
// 获取数组信息 : 基本元素类型、维度数目、各个维度尺寸
void get_array_info(TypeNode* arrayt, TypeNode** ele_type, int* dimension_num, int** dimensions);
// 获取结构体类型大小
int get_struct_size(SymNode* structv);
// 获取结构体中指定成员的偏移量
int get_member_offset(SymNode* structv, char* tar_mem);
// 获取类型节点所指类型的大小
int get_type_size(TypeNode* type_node);

// 合并两个SymNode符号数组，结果存储在src1中，原本的两个数组将被清空
void syms_pieces_cat(SymNode** src1, int size1, SymNode** src2, int size2);

// 打印语法分析树
void print_tree(Node* root, int blank_num);

# define DEBUG_STAKE printf("\nLook here boy! You get here!\n");

/*宏定义-简化C Language函数*/
#define BASIC_TYPE_INT "int"
#define BASIC_TYPE_FLOAT "float"

#define BASIC_TYPE_SIZE 4

#define STREQ(str1, str2) \
    (strcmp(str1, str2) == 0? 1 : 0)

#define STRCP(dest, src) \
    { \
        dest = (char*)malloc((strlen((src)) + 1) * sizeof(char)); \
        strcpy((dest), (src)); \
    }
