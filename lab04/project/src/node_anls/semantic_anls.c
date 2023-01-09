#include"../general.h"
#include"../utils/utils.h"

#include"../data_struct/spt.h"
#include"../data_struct/sym_table.h"

#include"semantic_anls.h"
#include"nodes_anls.h"


extern SymTable* table;

void static_sym_num_dfs_anls(Node* root, int* num){
    /*程序静态分析-获取-可能符号数量*/
    if(root == NULL) return;
    if(STREQ(root->name, "OptTag") ||
        STREQ(root->name, "VarDec") || STREQ(root->name, "FunDec")){
            (*num) = (*num) + 1;
        }
    Node* cursor = root->children;
    while(cursor != NULL){
        static_sym_num_dfs_anls(cursor, num);
        cursor = cursor->next;
    }
}

void semantic_anls(Node* root){
    /*语义分析-顶层流程控制*/
    Node* cursor = root->children;
    if(cursor == NULL) return;
    int sym_num = 0;
    static_sym_num_dfs_anls(root, &sym_num);
    init_sym_table(sym_num, &table);
    program_anls(root);
}



