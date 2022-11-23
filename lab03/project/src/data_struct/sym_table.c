#include"../general.h"
#include"sym_table.h"
#include"sym_node.h"
#include"type_node.h"

void init_sym_table(int sym_num, SymTable** sym_table){
    /*创建并初始化符号表*/
    (*sym_table) = (SymTable*)malloc(sizeof(SymTable));
    SymTable* table = (*sym_table);
    table->hash_size = sym_num;
    table->hash_data = (SymNode**)malloc(sym_num * sizeof(SymNode*));
    for(int i = 0; i < sym_num; i++) table->hash_data[i] = NULL;
}

void destory_sym_table(SymTable* sym_table){
    /*销毁及回收符号表数据*/
    for(int i = 0; i < sym_table->hash_size; i++){
        if(sym_table->hash_data[i] != NULL){
            SymNode* cursor = sym_table->hash_data[i];
            while(cursor != NULL){
                SymNode* del = cursor;
                cursor = cursor->hash_next;
                del->hash_next = NULL;
                destory_sym_node(del);
            }
            sym_table->hash_data[i] = NULL;
        }
    }
    free(sym_table);
}

int BKDR_hash(char* name, SymTable* table){
    /*字符串哈希函数-计算哈希值*/
    int hash_size = table->hash_size;
    int seed = 31;
    int rst = 0;
    while(*name){
        rst = rst * seed + (*name++);
    }
    return (rst & (hash_size - 1));
}

SymNode* hash_find(char* name, SymTable* table){
    /*查询符号表-全局查找*/
    int hash_index = BKDR_hash(name, table);
    SymNode* cursor = table->hash_data[hash_index];
    while(cursor != NULL){
        if(STREQ(cursor->name, name)) return cursor;
        cursor = cursor->hash_next;
    }
    return NULL;
}

void hash_insert(SymNode* p, SymTable* table){
    /*符号表追加表项*/
    int hash_index = BKDR_hash(p->name, table);
    p->hash_next = table->hash_data[hash_index];
    table->hash_data[hash_index] = p;
}


void printf_sym_table(SymTable* table){
    /*打印符号表*/
    printf("########### Sym Table Content ###########\n");
    for(int r = 0; r < table->hash_size; r++){
        SymNode* cursor = table->hash_data[r];
        while(cursor != NULL){
            if(cursor->ntype == VAR_SYM){
                printf("Variable:\n");
                printf("\tName: %s\n", cursor->name);
                printf("\tType: "); printf_type_node(cursor->var_v.vtype); printf("\n");
            }
            else if(cursor->ntype == FUNC_SYM){
                printf("Function:\n");
                printf("\tName: %s\n", cursor->name);
                printf("\tReturnType: "); printf_type_node(cursor->func_v.rtype); printf("\n");
                printf("\tParams: ");
                for(int i = 0; i < cursor->func_v.param_num; i++){
                    printf("("); printf_type_node(cursor->func_v.params[i].var_v.vtype);
                    printf(", %s) ", cursor->func_v.params[i].name);
                }
                printf("\n");
            }
            else if(cursor->ntype == STRUCT_SYM){
                printf("Structure:\n");
                printf("\tName: %s\n", cursor->name);
                printf("\tMembers: ");
                for(int i = 0; i < cursor->struct_v.mem_num; i++){
                    printf("("); printf_type_node(cursor->struct_v.members[i].var_v.vtype);
                    printf(", %s) ", cursor->struct_v.members[i].name);
                }
                printf("\n");
            }
            cursor = cursor->hash_next;
        }
    }
    printf("##################################\n");
}
