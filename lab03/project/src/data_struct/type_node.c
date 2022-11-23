#include"../general.h"
#include"../utils/utils.h"

#include"type_node.h"

TypeNode* new_type_node(){
    /*类型节点-初始化*/
    TypeNode* p = (TypeNode*)malloc(sizeof(TypeNode));

    p->if_const = 0;
    p->ntype = STRUCT_T;

    p->struct_v.def_ptr = NULL;

    p->array_v.size = 0;
    p->array_v.ele_type = NULL;

    p->basic_v.basic_type = NULL;

    return p;
}

TypeNode* new_struct_type_node(SymNode* struct_def, int if_const){
    /*新建符号-结构类型*/
    TypeNode* p = new_type_node();
    p->if_const = if_const;
    p->ntype = STRUCT_T;

    p->struct_v.def_ptr = struct_def;

    return p;
}

TypeNode* new_array_type_node(TypeNode* ele_t, int size, int if_const){
    /*新建符号-数组类型*/
    TypeNode* p = new_type_node();
    p->if_const = if_const;
    p->ntype = ARRAY_T;

    p->array_v.size = size;
    p->array_v.ele_type = ele_t;

    return p;
}

TypeNode* new_basic_type_node(char* basic_type, int if_const){
    /*新建符号-基本类型*/
    TypeNode* p = new_type_node();
    p->if_const = if_const;
    p->ntype = BASIC_T;

    STRCP(p->basic_v.basic_type, basic_type);

    return p;
}

TypeNode* clone_type_node(TypeNode* ori_node){
    /*类型节点-克隆*/
    TypeNode* p = new_type_node();
    p->if_const = ori_node->if_const;
    if(ori_node->ntype == STRUCT_T){
        p->ntype = STRUCT_T;
        p->struct_v.def_ptr = ori_node->struct_v.def_ptr;
    }
    else if(ori_node->ntype == ARRAY_T){
        p->ntype = ARRAY_T;
        p->array_v.size = ori_node->array_v.size;
        p->array_v.ele_type = clone_type_node(ori_node->array_v.ele_type);
    }
    else if(ori_node->ntype == BASIC_T){
        p->ntype = BASIC_T;
        STRCP(p->basic_v.basic_type, ori_node->basic_v.basic_type);
    }
    else assert(0);
    return p;
}

void destory_type_node(TypeNode* node){
    /*类型节点-销毁&回收内容*/
    if(node->ntype == STRUCT_T){
        node->struct_v.def_ptr = NULL;
    }
    else if(node->ntype == ARRAY_T){
        destory_type_node(node->array_v.ele_type);
    }
    else if(node->ntype == BASIC_T){
        free(node->basic_v.basic_type);
    }
    free(node);
}


void printf_type_node(TypeNode* type){
    /*打印符号节点*/
    if(type == NULL) return;
    if(type->ntype == BASIC_T){
        printf("%s", type->basic_v.basic_type);
    }
    else if(type->ntype == STRUCT_T){
        printf("%s", type->struct_v.def_ptr->name);
    }
    else if(type->ntype == ARRAY_T){
        TypeNode* ele_type = type;
        while(ele_type->ntype == ARRAY_T) ele_type = ele_type->array_v.ele_type;
        printf_type_node(ele_type);

        while(type->ntype == ARRAY_T){
            printf("[%s]", int2str(type->array_v.size));
            type = type->array_v.ele_type;
        }
    }
}