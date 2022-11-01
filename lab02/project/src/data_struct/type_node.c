#include"../general.h"
#include"../utils/utils.h"

#include"type_node.h"
#include"sym_node.h"
#include"sym_table.h"

extern SymHashStk* sym_table;

TypeNode* new_type_node(){
    /*新建基本类型节点*/
    TypeNode* rst = (TypeNode*)malloc(sizeof(TypeNode));
    rst->type_name = "";
    rst->arr_ele_type = NULL;
    rst->next = NULL;
    rst->struct_type = NULL;
    return rst;
}

TypeNode* new_common_type_node(char* v_type){
    /*新建变量类型节点*/
    TypeNode* p = new_type_node();
    if(STREQ(v_type, BASIC_TYPE_FLOAT) || STREQ(v_type, "FLOAT")){
        if(STREQ(v_type, BASIC_TYPE_FLOAT)) p->type = BASIC;
        else p->type = CONSTANT;
        STRCP(p->type_name, BASIC_TYPE_FLOAT);
    }
    else if(STREQ(v_type, BASIC_TYPE_INT) || STREQ(v_type, "INT10")){
        if(STREQ(v_type, BASIC_TYPE_INT)) p->type = BASIC;
        else p->type = CONSTANT;
        STRCP(p->type_name, BASIC_TYPE_INT);
    }
    else{
        p->type = STRUCTTYPE;
        STRCP(p->type_name, v_type);
        p->struct_type = hash_found_global(sym_table, v_type);
    }
    return p;
}

TypeNode* new_constant_type_node(char* v_type){
    /*新建常量类型节点*/
    TypeNode* p = new_type_node();
    if(STREQ(v_type, BASIC_TYPE_INT)){
        p->type = CONSTANT;
        STRCP(p->type_name, BASIC_TYPE_INT);
    }
    else if(STREQ(v_type, BASIC_TYPE_FLOAT)){
        p->type = CONSTANT;
        STRCP(p->type_name, BASIC_TYPE_FLOAT);
    }
    else{
        p->type = STRUCTTYPE;
        STRCP(p->type_name, v_type);
        p->struct_type = hash_found_global(sym_table, v_type);
    }
    return p;
}

TypeNode* new_array_type_node(int arr_size){
    /*新建数组类型节点*/
    TypeNode* p = new_type_node();
    p->type = ARRAY;
    p->arr_size = arr_size;
    return p;
}

TypeNode* clone_type_node(TypeNode* ori_node){
    /*克隆类型节点*/
    if(ori_node == NULL) return NULL;
    TypeNode* new_node = new_type_node();
    new_node->type = ori_node->type;
    if(ori_node->type == BASIC || ori_node->type == CONSTANT || ori_node->type == STRUCTTYPE){
        STRCP(new_node->type_name, ori_node->type_name);
        new_node->struct_type = ori_node->struct_type;
    }
    else if(ori_node->type == ARRAY){
        new_node->arr_size = ori_node->arr_size;
        new_node->arr_ele_type = clone_type_node(ori_node->arr_ele_type);
    }
    return new_node;
}

int equal_type_nodes(TypeNode* node1, TypeNode* node2, int* pass){
    /*比较两个类型节点是否相等，returnval: 1-相等，0-不相等*/
    if(node1 == NULL && node2 == NULL) return 1;
    else if(node1 != NULL && node2 == NULL) return 0;
    else if(node1 == NULL && node2 != NULL) return 0;
    else{
        // Class01: 双基本类型判断
        if((STREQ(node1->type_name, BASIC_TYPE_INT) || STREQ(node2->type_name, BASIC_TYPE_FLOAT)) &&
           (STREQ(node1->type_name, BASIC_TYPE_INT) || STREQ(node2->type_name, BASIC_TYPE_FLOAT)))
        {
            return STREQ(node1->type_name, node2->type_name);
        }
        // Class02: 双数组类型判断
        else if(node1->type == ARRAY && node2->type == ARRAY){
            TypeNode* p1 = node1;
            TypeNode* p2 = node2;
            while(p1 != NULL && p2 != NULL && p1->type == ARRAY && p2->type == ARRAY){
                // 数组赋值仅要求数组维数、数组基类型相同即可：if(p1->arr_size != p2->arr_size) return 0;
                p1 = p1->arr_ele_type;
                p2 = p2->arr_ele_type;
            }
            if(p1 == NULL && p2 != NULL) return 0;
            else if(p1 == NULL && p2 == NULL) return 0;
            else if(p1 != NULL && p2 == NULL) return 0;
            if(p1->type == BASIC && p2->type == BASIC){
                return STREQ(p1->type_name, p2->type_name);
            }
            else if(p1->type == STRUCTTYPE && p2->type == STRUCTTYPE){
                return equal_type_nodes(p1, p2, pass);
            }
            else return 0;
        }
        // Class03: 双结构体类型判断-结构等价的实现
        else if(node1->type == STRUCTTYPE && node2->type == STRUCTTYPE){
            SymNode* t1 = node1->struct_type;
            SymNode* t2 = node2->struct_type;
            SymNode* mem1 = t1->StructInfo.field_next;
            SymNode* mem2 = t2->StructInfo.field_next;
            while(mem1 != NULL && mem2 != NULL){
                if(!(equal_type_nodes(mem1->VarInfo.var_type, mem2->VarInfo.var_type, pass))) return 0;
                mem1 = mem1->StructInfo.field_next;
                mem2 = mem2->StructInfo.field_next;
            }
            if(mem1 == NULL && mem2 == NULL) return 1;
            else return 0;
        }
        return 0;
    }
    return 0;
}

void free_type_node(TypeNode* node){
    /*删除类型节点-归还类型节点申请的动态内存空间*/
    if(node == NULL) return;
    if(node->type == ARRAY){
        TypeNode* tmp = node->arr_ele_type;
        free_type_node(tmp);
        node->arr_ele_type = NULL;
    }
    else{
        free(node->type_name);
        node->struct_type = NULL;
    }
    free(node);
}