#include"../general.h"
#include"sym_node.h"
#include"type_node.h"

SymNode* new_sym_node(char* name){
    /*新建、初始化基本符号节点*/
    SymNode* p = (SymNode*)malloc(sizeof(SymNode));

    STRCP(p->name, name);
    p->ntype = VAR_SYM;

    p->var_v.vtype = NULL;

    p->struct_v.mem_num = 0;
    p->struct_v.members = NULL;

    p->func_v.rtype = NULL;
    p->func_v.param_num = 0;
    p->func_v.params = NULL;

    p->hash_next = NULL;

    return p;
}

SymNode* new_var_sym_node(char* name, TypeNode* type_node){
    /*新建变量-符号节点*/
    SymNode* p = new_sym_node(name);
    p->ntype = VAR_SYM;

    p->var_v.vtype = type_node;

    return p;
}

SymNode* new_struct_node(char* name, int mem_num, SymNode* members){
    /*新建结构体定义-符号节点*/
    SymNode* p = new_sym_node(name);
    p->ntype = STRUCT_SYM;

    p->struct_v.mem_num = mem_num;
    p->struct_v.members = members;

    return p;
}

SymNode* new_func_node(char* name, TypeNode* rtype, int param_num, SymNode* params){
    /*新建函数定义-符号节点*/
    SymNode* p = new_sym_node(name);
    p->ntype = FUNC_SYM;

    p->func_v.rtype = rtype;
    p->func_v.param_num = param_num;
    p->func_v.params = params;

    return p;
}

SymNode* clone_sym_node(SymNode* ori_node){
    /*符号节点-克隆*/
    if(ori_node->ntype != VAR_SYM) assert(0);   // 不存在克隆函数定义、结构体定义的情况

    SymNode* p = new_sym_node(ori_node->name);
    if(ori_node->ntype == VAR_SYM){
        p->ntype = VAR_SYM;

        p->var_v.vtype= ori_node->var_v.vtype;
    }

    return p;
}

void destory_sym_node(SymNode* node){
    /*符号节点-销毁&回收内容*/
    free(node->name);
    if(node->ntype == VAR_SYM){
        destory_type_node(node->var_v.vtype);
    }
    else if(node->ntype == STRUCT_SYM){
        for(int i = 0; i < node->struct_v.mem_num; i++){
            destory_sym_node(&(node->struct_v.members[i]));
        }
    }
    else if(node->ntype == FUNC_SYM){
        destory_type_node(node->func_v.rtype);
        for(int i = 0; i < node->func_v.param_num; i++){
            destory_sym_node(&(node->func_v.params[i]));
        }
    }
    free(node);
}

