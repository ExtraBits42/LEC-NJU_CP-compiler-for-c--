/*
模块:
    Name: Symbol Table-符号表
*/

#pragma once
#include"../general.h"


// 构建符号表
void init_symbol_table(int notation_num, int max_depth, SymHashStk** sym_table);

// 字符串哈希函数
int BKDR_hash(SymHashStk* symble_table, char* name);

// 符号表查询操作: 当前作用域/全局
SymNode* hash_found_global(SymHashStk* table, char* tar_name);
SymNode* hash_found_cur_scope(SymHashStk* table, char* tar_name);

// 符号表追加表项
void hash_insert(SymHashStk* table, SymNode* p);

// 作用域符号表操作: 新建作用域符号表/销毁作用域符号表
void stack_push(SymHashStk* table);
void stack_pop(SymHashStk* table);

