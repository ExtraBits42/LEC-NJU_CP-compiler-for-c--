#include"../general.h"
#include"../utils/utils.h"

#include"semantic_checker.h"

#include"../data_struct/type_node.h"
#include"../data_struct/sym_node.h"
#include"../data_struct/sym_table.h"

extern SymTable* table;

SymNode* check_semantic_error_1(Node* id){
    // 检查错误号 : 1 - 变量在使用时未经定义
    assert(STREQ(id->name, "ID"));
    SymNode* found = hash_find(id->u.lex_val, table);
    if(found == NULL){
        LOG_SEMANTIC_ANLS_ERROR(1, id->u.syn_line, "Undefined variable.");
        return NULL;
    }
    else return found;
}

SymNode* check_semantic_error_3(Node* id){
    // 检查错误号 : 3 - 变量重复定义OR变量与已定义的结构体名字重复
    assert(STREQ(id->name, "ID"));
    SymNode* found = hash_find(id->u.lex_val, table);
    if(found != NULL){
        LOG_SEMANTIC_ANLS_ERROR(3, id->u.syn_line, "Redefined variable.");
        return found;
    }
    else return NULL;
}

