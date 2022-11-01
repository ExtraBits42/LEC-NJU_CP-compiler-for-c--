/*
模块:
    Name: Utiliys-多功能工具包
*/

#pragma once

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"../general.h"

// 最大值/最小值
int max(int a, int b);
int min(int a, int b);

// 八进制/十六进制数->十进制数
int int_o2d(char *v);
int int_h2d(char *v);

// Debug-打印符号节点/类型节点
char* pnotation_type(SymType x);
void pdata_type(TypeNode* p);

// 符号表调试用-作用域层次/哈希表层次/详细展示符号表
void show_me_sym_table_scope(SymHashStk* table);
void show_me_sym_table_hash(SymHashStk* table);
void show_me_sym_table_layer_details(SymHashStk* table);

# define DEBUG_STAKE printf("\nLook here boy! You get here!\n");
