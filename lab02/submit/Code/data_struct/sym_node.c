#include"../general.h"
#include"../utils/utils.h"

#include"sym_node.h"
#include"type_node.h"

SymNode* new_sym_node(){
    /*新建基本符号节点，初始化所有指针类型成员*/
    SymNode* p = (SymNode*)malloc(sizeof(SymNode));
    p->VarInfo.var_type = NULL;
    p->FuncInfo.func_return_type = NULL;
    p->FuncInfo.params_list = NULL;
    p->StructInfo.field_next = NULL;

    p->hash_next = NULL;
    p->scope_next = NULL;
}

SymNode* new_var_sym_node(char* v_name, TypeNode* v_type){
    /*新建变量节点*/
    SymNode* p = new_sym_node();
    STRCP(p->name, v_name);
    p->type = VAR;
    p->VarInfo.var_type = v_type;
    return p;
}

SymNode* new_struct_sym_node(char* s_name){
    /*新建结构体类型节点*/
    SymNode* p = new_sym_node();
    STRCP(p->name, s_name);
    p->type = STRUCTT;
    p->StructInfo.field_next = NULL;
    return p;
}

SymNode* new_func_sym_node(char* f_name){
    /*新建函数节点*/
    SymNode* p = new_sym_node();
    STRCP(p->name, f_name);
    p->type = FUNC;
    p->FuncInfo.func_return_type = NULL;
    p->FuncInfo.params_list = NULL;
    return p;
}

SymNode* clone_var_sym_node(SymNode* ori_node){
    /*克隆变量节点*/
    if(ori_node == NULL) return NULL;
    SymNode* new_node = new_sym_node();
    STRCP(new_node->name, ori_node->name);
    new_node->type = ori_node->type;

    assert(ori_node->type == VAR);
    if(ori_node->type == VAR){
        new_node->VarInfo.var_type = clone_type_node(ori_node->VarInfo.var_type);
    }
    return new_node;
}

void free_sym_node(SymNode* node){
    /*删除符号节点-归还符号节点申请的动态空间*/
    if(node == NULL) return;
    free(node->name);
    if(node->type == VAR){// Case01: 删除常量/变量符号节点
        free_type_node(node->VarInfo.var_type);
    }
    else if(node->type == FUNC){// Case02: 删除函数符号节点
        free_type_node(node->FuncInfo.func_return_type);
        SymNode* param = node->FuncInfo.params_list;
        while(param != NULL){
            SymNode* tmp = param;
            param = param->FuncInfo.params_list;
            free_sym_node(tmp);
        }
    }
    else if(node->type == STRUCTT){// Case03: 删除结构体符号节点
        SymNode* field = node->StructInfo.field_next;
        while(field != NULL){
            SymNode* tmp = field;
            field = field->StructInfo.field_next;
            free_sym_node(tmp);
        }
    }
    free(node);
}