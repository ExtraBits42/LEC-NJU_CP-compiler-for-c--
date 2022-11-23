/*
模块:
    Name: Symbol Table-符号表
*/
#pragma once

#include"sym_node.h"

typedef struct SymHashTable{
    int hash_size;
    struct SymNode** hash_data;
}SymTable;

// 创建并初始化符号表
void init_sym_table(int sym_num, SymTable** sym_table);
// 销毁及回收符号表数据
void destory_sym_table(SymTable* sym_table);
// 符号表操作-查找符号
SymNode* hash_find(char* name, SymTable* table);
// 符号表操作-添加符号
void hash_insert(SymNode* p, SymTable* table);

// 打印符号表
void printf_sym_table(SymTable* table);
