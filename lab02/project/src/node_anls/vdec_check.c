#include"../general.h"
#include"../utils/utils.h"

#include"../data_struct/sym_node.h"
#include"../data_struct/sym_table.h"
#include"../data_struct/type_node.h"

#include"vdec_check.h"
#include"sdec_check.h"

extern SymHashStk* sym_table;
extern SymHashStk* dec_table;

void vdecs_anls(Node* extdef, int* pass){
    /*顶层函数:分析变量定义*/
    /**
     * ExtDef->Specifier ExtDecList SEMI
     * **/
    Node* specifier = extdef->children;
    Node* extdeclist = extdef->children->next;
    char* type;
    int vdec_pass = 1;
    type = vdecs_specifier_anls(specifier, &vdec_pass);
    vdecs_extdeclist_anls(type, extdeclist, &vdec_pass);
}

char* vdecs_specifier_anls(Node* specifier, int* pass){
    /*分析Spefifier节点，获取变量类型*/
    /**
     * Specifier->TYPE | StructSpecifier;
     * StructSpecifier->STRUCT Tag | STRUCT OptTag LC DefList RC
     * **/
    Node* type_node = NULL;
    char* type = "";
    // Specifier->TYPE
    if(STREQ(specifier->children->name, "TYPE")){
        type_node = specifier->children;
        STRCP(type, type_node->u.lex_val);
    }
    // Specifier->StructSpecifier->STRUCT Tag; Tag->ID
    else if(STREQ(specifier->children->children->next->name, "Tag")){
        type_node = specifier->children->children->next->children;
        STRCP(type, type_node->u.lex_val);
    }
    // Specifier->StructSpecifier->STRUCT OptTag LC DefList RC
    else{
        // 结构体定义+变量定义
        Node* struct_spec = specifier->children;
        type = sdecs_structspecifier_anls(struct_spec, pass);
    }
    // 类型是否存在
    if(!vdecs_if_type_exist(type)){
        (*pass) = 0;
        LOG_SEMANTIC_ANLS_ERROR(17, specifier->u.syn_line, "use undefined struct type defines var.");
    }
    return type;
}

void vdecs_extdeclist_anls(char* type, Node* extdeclist, int* pass){
    /*分析ExtDecList节点，获取节点列表*/   
    /**
     * ExtDecList->VarDec | VarDec COMMA ExtDecList
     * **/
    while(extdeclist != NULL){
        Node* vardec = extdeclist->children;
        SymNode* p = vdecs_vardec_anls(type, vardec, pass);
        if(p == NULL){
            // TODO: 发现变量重名后的后续处理
            LOG_SEMANTIC_ANLS_ERROR(3, vardec->u.syn_line, "The var has already beed defined.");
        }
        else{
            if((*pass) == 1) hash_insert(sym_table, p);
        }
        // ExtDecList->VarDec COMMA [ExtDecList]
        if(extdeclist->children->next != NULL)
            extdeclist = extdeclist->children->next->next;
        else extdeclist = NULL;
    }
}

SymNode* vdecs_vardec_anls(char* type, Node* vardec, int* pass){
    /*分析Varec节点，获取节点定义，追加到符号表*/
    /**
     * VarDec-> [ID] | VarDec LB [INT] RB
     * **/
    // 获取变量ID
    Node* id_node = vardec->children;
    while(id_node != NULL && !STREQ(id_node->name, "ID")){
        id_node = id_node->children;
    }
    // 错误检查: 重复定义变量
    SymNode* scheck = hash_found_global(sym_table, id_node->u.lex_val);
    if(scheck != NULL && scheck->type == STRUCTT){
        (*pass) = 0;
        return NULL;
    }

    SymNode* find =  hash_found_cur_scope(sym_table, id_node->u.lex_val);
    if(find != NULL){
        (*pass) = 0;
        return NULL;
    }
    // Case01: VarDec->ID
    TypeNode* type_node = NULL;
    if(vardec->children->next == NULL){
        type_node = new_common_type_node(type);
    }
    // Case02: VarDec->VarDec LB INT RB
    else{
        type_node = new_common_type_node(type);
        Node* cursor = vardec->children;
        while(cursor != NULL && cursor->next != NULL){
            Node* size_node = cursor->next->next;
            int size = atoi(size_node->u.lex_val);
            TypeNode* tmp = new_array_type_node(size);
            tmp->arr_ele_type = type_node;
            type_node = tmp;
            cursor = cursor->children;
        }
    }
    SymNode* p = new_var_sym_node(id_node->u.lex_val, type_node);
    return p;
}

int vdecs_if_type_exist(char* type){
    /*辅助函数：检查类型是否存在: returnval: 1-存在；0-不存在*/
    if(STREQ(type, BASIC_TYPE_INT) || STREQ(type, BASIC_TYPE_FLOAT)) return 1;
    SymNode* found_type = hash_found_global(sym_table, type);
    if(found_type != NULL && found_type->type == STRUCTT) return 1;
    else return 0;
}

