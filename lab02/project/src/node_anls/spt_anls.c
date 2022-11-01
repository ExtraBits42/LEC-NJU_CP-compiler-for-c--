#include"../general.h"
#include"../utils/utils.h"

#include"../data_struct/sym_table.h"

#include"vdec_check.h"
#include"sdec_check.h"
#include"fdec_check.h"

/*Gloabal Variables*/
SymNode* cur_func = NULL;
SymHashStk* dec_table = NULL;
SymHashStk* sym_table = NULL;

void static_notations_anls_dfs(Node* root, int* num, int* cur_depth, int* max_depth){
    /*程序静态分析1，获取：可能符号数量，最大作用域深度*/
    if(root == NULL) return;
    if(STREQ(root->name, "OptTag") ||
        STREQ(root->name, "VarDec") || STREQ(root->name, "FunDec")){
        (*num) = (*num) + 1;
       }
    if(STREQ(root->name, "LC")){
        (*cur_depth) = (*cur_depth) + 1;
        (*max_depth) = max((*max_depth), (*cur_depth));
    }
    else if(STREQ(root->name, "RC")){
        (*cur_depth) = (*cur_depth) - 1;
    }
    Node* cursor = root->children;
    while(cursor != NULL){
        static_notations_anls_dfs(cursor, num, cur_depth, max_depth);
        cursor = cursor->next;
    }
}

void get_static_notations_info(Node* root, int* num, int* depth){
    /*程序静态分析2，获取：可能符号数量，最大作用域深度*/
    int cur_depth = 1;
    assert(root->children == NULL || root->children->next == NULL);
    static_notations_anls_dfs(root, num, &cur_depth, depth);
}


void top_dfs_anls(Node* root, int* pass){
    /*语义分析-核心分析部分*/
    Node* cursor = root->children;              // Program->ExtDefList
    if(cursor == NULL) return;                  // ExtDefList->epsilon
    // 定义部分
    while(cursor != NULL){
        if(cursor->children == NULL) break;     // ExtDefList->epsilon
        Node* extdef = cursor->children;        // ExtDefList->ExtDef ExtDefList
        cursor = cursor->children->next;
        //分析节点ExtDef
        /**
         * ExtDef->Specifier SEMI
         * | Specifier ExtDecList SEMI
         * | Specifier FunDec CompSt
         * | Specifier FunDec SEMI
         * **/
        // 结构体定义: ExtDef->Specifier SEMI
        if(STREQ(extdef->children->next->name, "SEMI")){
            sdecs_anls(extdef, pass);
        }
        // 变量定义: ExtDef->Specifier ExtDecList SEMI
        else if(STREQ(extdef->children->next->name, "ExtDecList")){
            vdecs_anls(extdef, pass);
        }
        // 函数定义: ExtDef->Specifier FunDec CompSt
        else if(STREQ(extdef->children->next->next->name, "CompSt")){
            fdecs_anls(extdef, pass, 0);
        }
        (*pass) = 1;
    }

    // 声明部分
    cursor = root->children;
    while(cursor != NULL){
        if(cursor->children == NULL) break;
        Node* extdef = cursor->children;
        cursor = cursor->children->next;
        // 函数声明: Extdef->Specifier FunDec SEMI
        if(extdef->children->next->next != NULL && STREQ(extdef->children->next->next->name, "SEMI") &&
          STREQ(extdef->children->next->name, "FunDec")){
            fdecs_anls(extdef, pass, 1);
        }
    }
}


void semantic_anls(Node* root, int* pass){
    /*语义分析主程序*/
    // Step01.静态分析，获取程序: 最大可能符号数量，最大作用域深度
    int notation_num = 0, max_depth = 1;
    get_static_notations_info(root, &notation_num, &max_depth);
    // Step02.初始化符号表
    init_symbol_table(notation_num, max_depth+1, &sym_table);

    init_symbol_table(notation_num, 2, &dec_table);
    // Step03.语义分析核心阶段
    top_dfs_anls(root, pass);
}
