#include"../general.h"
#include"../utils/utils.h"

#include"sym_node.h"
#include"sym_table.h"

int BKDR_hash(SymHashStk* table, char* name){
    /*字符串哈希函数-计算哈希值*/
    int hash_size = table->hash_size;
    int seed = 31;
    int rst = 0;
    while(*name){
        rst = rst * seed + (*name++);
    }
    return (rst & (hash_size - 1));
}

void init_symbol_table(int notation_num, int max_depth, SymHashStk** sym_table){
    /*创建并初始化符号表【哈希表+栈->十字链表】*/
    (*sym_table) = (SymHashStk*)malloc(sizeof(SymHashStk));
    SymHashStk* table = (*sym_table);
    table->hash_size = notation_num;
    table->stack_size = max_depth;
    table->st_ptr = 0;
    table->hash_data = (SymNode**)malloc(notation_num * sizeof(SymNode*));
    table->stack_data = (SymNode**)malloc(max_depth * sizeof(SymNode*));
    table->stack_tails = (SymNode**)malloc(max_depth * sizeof(SymNode*));
    for(int i = 0; i < notation_num; i++) table->hash_data[i] = NULL;
    for(int i = 0; i < max_depth; i++) table->stack_data[i] = NULL;
    for(int i = 0; i < max_depth; i++) table->stack_tails[i] = NULL;
}

SymNode* hash_found_global(SymHashStk* table, char* tar_name){
    /*查询符号表-全局查找*/
    int hash_index = BKDR_hash(table, tar_name);
    SymNode* cursor = table->hash_data[hash_index];
    while(cursor != NULL){
        if(STREQ(cursor->name, tar_name)) return cursor;
        cursor = cursor->hash_next;
    }
    return NULL;
}

SymNode* hash_found_cur_scope(SymHashStk* table, char* tar_name){
    /*查询符号表-当前作用域查找*/
    SymNode* cursor = table->stack_data[table->st_ptr];
    while(cursor != NULL){
        if(STREQ(cursor->name, tar_name)) return cursor;
        cursor = cursor->scope_next;
    }
    return NULL;
}

void hash_insert(SymHashStk* table, SymNode* p){
    /*符号表追加表项*/
    // Step01.下挂到当前作用域对应的栈帧中
    if(table->stack_data[table->st_ptr] == NULL){
        table->stack_data[table->st_ptr] = p;
        table->stack_tails[table->st_ptr] = p;
    }
    else{
        table->stack_tails[table->st_ptr]->scope_next = p;
        table->stack_tails[table->st_ptr] = p;
    }
    // Step02.头插法插入到哈希表中
    int hash_index = BKDR_hash(table, p->name);
    p->hash_next = table->hash_data[hash_index];
    table->hash_data[hash_index] = p;
}

void stack_push(SymHashStk* table){
    /*作用域栈-压栈操作，新建作用域符号表*/
    table->st_ptr++;
    assert(table->st_ptr < table->stack_size);
}

void stack_pop(SymHashStk* table){
    /*作用域栈-退栈操作，删除作用域符号表*/
    // Step01.删除所弹出栈帧的内容
    SymNode* cursor = table->stack_data[table->st_ptr];
    while(cursor != NULL){
        SymNode* tmp = cursor;
        cursor = cursor->scope_next;
        int hash_index = BKDR_hash(table, tmp->name);
        if(cursor != NULL && cursor->hash_next == tmp){
            cursor->hash_next = tmp->hash_next;
        }
        else{
            if(tmp->hash_next == NULL) table->hash_data[hash_index] = NULL;
            else table->hash_data[hash_index] = tmp->hash_next;
        }
        tmp->scope_next = NULL;
    }
    // Step02.栈弹出栈帧
    table->stack_data[table->st_ptr] = NULL;
    table->stack_tails[table->st_ptr] = NULL;
    table->st_ptr--;
    assert(table->st_ptr >= 0);
}

