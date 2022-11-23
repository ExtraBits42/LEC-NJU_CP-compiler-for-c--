#include"../general.h"
#include"../utils/utils.h"

#include"semantic_checker.h"

#include"../data_struct/type_node.h"
#include"../data_struct/sym_node.h"
#include"../data_struct/sym_table.h"

extern int pass;
extern SymTable* table;

int check_semantic_error_1(Node* id){
    // 检查错误号 : 1 - 变量在使用时未经定义
    assert(STREQ(id->name, "ID"));
    if(
        STREQ(id->u.lex_val, "write") ||
        STREQ(id->u.lex_val, "read")
    ){
        return 1;
    }

    SymNode* found = hash_find(id->u.lex_val, table);
    if(found == NULL){
        LOG_SEMANTIC_ANLS_ERROR(1, id->u.syn_line, "Undefined variable.");
        return 0;
    }
    else return 1;
}

int check_semantic_error_3(Node* id){
    // 检查错误号 : 3 - 变量重复定义OR变量与已定义的结构体名字重复
    assert(STREQ(id->name, "ID"));
    SymNode* found = hash_find(id->u.lex_val, table);
    if(found != NULL){
        LOG_SEMANTIC_ANLS_ERROR(3, id->u.syn_line, "Redefined variable.");
        return 0;
    }
    else return 1;
}

int check_semantic_error_17(TypeNode* type, Node* helper){
    // 检查错误号 : 17 - 直接使用未定义过的结构体来定义变量
    if(type == NULL || (type->ntype == STRUCT_T && type->struct_v.def_ptr == NULL)){
        LOG_SEMANTIC_ANLS_ERROR(17, helper->u.syn_line, "Undefined structure.");
        return 0;
    }
    if(type->ntype == BASIC_T) return 1;
    else if(type->ntype == STRUCT_T) return 1;
    else if(type->ntype == ARRAY_T){
        TypeNode* cursor = type;
        while(cursor != NULL && cursor->ntype == ARRAY_T)
            cursor = cursor->array_v.ele_type;
        if(cursor == NULL || (type->ntype == STRUCT_T && type->struct_v.def_ptr == NULL)){
            LOG_SEMANTIC_ANLS_ERROR(17, helper->u.syn_line, "Undefined structure.");
            return 0;
        }
        else return 1;
    }
    else return 0;
}

int check_semantic_error_10(TypeNode* type, Node* exp){
    // 检查错误号 : 10 - 对非数组变量使用"[]"操作符
    assert(STREQ(exp->name, "Exp"));
    if(type == NULL || type->ntype != ARRAY_T){
        LOG_SEMANTIC_ANLS_ERROR(10, exp->u.syn_line, "Use operation [] with a no array vaiable.");
        return 0;
    }
    else return 1;
}

int check_semantic_error_12(TypeNode* type, Node* exp){
    // 检查错误号 : 12 - 数组访问操作符"[]"中出现非整数
    assert(STREQ(exp->name, "Exp"));
    if(type == NULL || type->ntype != BASIC_T || !STREQ(type->basic_v.basic_type, BASIC_TYPE_INT)){
        LOG_SEMANTIC_ANLS_ERROR(12, exp->u.syn_line, "Use no integer variable as array's index.");
        return 0;
    }
    else return 1;
}

int check_semantic_error_2(Node* func_id, Node* exp){
    // 检查错误号 : 2 - 函数在调用时未经定义
    assert(STREQ(func_id->name, "ID"));
    assert(STREQ(exp->name, "Exp"));
    char* func_name = func_id->u.lex_val;
    SymNode* found = hash_find(func_name, table);
    if(found == NULL || found->ntype != FUNC_SYM){
        LOG_SEMANTIC_ANLS_ERROR(2, exp->u.syn_line, "Call undefined function.");
        return 0;
    }
    else return 1;
}

int check_semantic_error_4(Node* func_id, Node* fundec){
    // 检查错误号 : 4 - 函数出现重复定义
    assert(STREQ(func_id->name, "ID"));
    assert(STREQ(fundec->name, "FunDec"));
    char* func_name = func_id->u.lex_val;
    SymNode* found = hash_find(func_name, table);
    if(found != NULL && found->ntype == FUNC_SYM){
        LOG_SEMANTIC_ANLS_ERROR(4, fundec->u.syn_line, "Redefined function.");
        return 0;
    }
    else return 1;
}

int check_semantic_error_13(TypeNode* type, Node* exp){
    // 检查错误号 : 13 - 对非结构体类型变量使用"."操作符
    assert(STREQ(exp->name, "Exp"));
    if(type == NULL || type->ntype != STRUCT_T || type->struct_v.def_ptr == NULL){
        LOG_SEMANTIC_ANLS_ERROR(13, exp->u.syn_line, "Illegal use of \".\".")
        return 0;
    }
    else return 1;
}
SymNode* check_semantic_error_14(TypeNode* type, Node* exp){
    // 检查错误号 : 14 - 访问结构体中未定义过的域
    assert(STREQ(exp->name, "Exp"));
    assert(STREQ(exp->children->next->name, "DOT"));
    assert(STREQ(exp->children->next->next->name, "ID"));
    char* mem_id = exp->children->next->next->u.lex_val;
    if(type == NULL || type->ntype != STRUCT_T || type->struct_v.def_ptr == NULL){
        LOG_SEMANTIC_ANLS_ERROR(14, exp->u.syn_line, "Non-existent field.");
        return NULL;
    }
    else{
        SymNode* struct_def = type->struct_v.def_ptr;
        for(int i = 0; i < struct_def->struct_v.mem_num; i++){
            if(STREQ(mem_id, struct_def->struct_v.members[i].name)){
                return &(struct_def->struct_v.members[i]);
            }
        }
        LOG_SEMANTIC_ANLS_ERROR(14, exp->u.syn_line, "Non-existent field.");
        return NULL;
    }
}

// TODO : 15

int check_semantic_error_16(char* struct_name, Node* opttag){
    // 检查错误号 : 16 - 结构体名字与前面定义过的结构体或变量的名字重复
    assert(STREQ(opttag->name, "OptTag"));
    SymNode* found = hash_find(struct_name, table);
    if(found != NULL){
        LOG_SEMANTIC_ANLS_ERROR(16, opttag->u.syn_line, "Duplicated structure name.");
        return 0;
    }
    else return 1;
}

int check_semantic_error_5(TypeNode* type1, TypeNode* type2, Node* helper){
    // 检查错误号 : 5 - 赋值号两边的表达式类型不匹配
    if(type1 == NULL || type2 == NULL){
        LOG_SEMANTIC_ANLS_ERROR(5, helper->u.syn_line, "Type mismatched for assignment.");
        return 0;
    }
    if(
        (type1->ntype == BASIC_T && type2->ntype == BASIC_T && STREQ(type1->basic_v.basic_type, type2->basic_v.basic_type)) ||
        (type1->ntype == ARRAY_T && type2->ntype == ARRAY_T) ||
        (type1->ntype == STRUCT_T && type2->ntype == STRUCT_T && type1->struct_v.def_ptr == type2->struct_v.def_ptr)
    ){
        return 1;
    }
    else{
        LOG_SEMANTIC_ANLS_ERROR(5, helper->u.syn_line, "Type mismatched for assignment.");
        return 0;
    }
}

int check_semantic_error_6(TypeNode* type, Node* helper){
    // 检查错误号 : 6 - 赋值号左边出现一个只有右值的表达式
    if(type == NULL) return 0;
    if(type->if_const == 1){
        LOG_SEMANTIC_ANLS_ERROR(6, helper->u.syn_line, "The left-hand side of an assignment must be a variable.");
        return 0;
    }
    else return 1;
}


