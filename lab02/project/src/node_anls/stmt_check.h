/*
模块:
    Statement Check-语句检查
*/
#pragma once
#include"../general.h"

//顶层函数-复合语句分析
void stmt_compst_anls(Node* compst, int* pass);

//节点分析函数
void stmt_stmtlist_anls(Node* stmtlist, int* pass);
void stmt_stmt_anls(Node* stmt, int* pass);

TypeNode* stmt_exp_anls(Node* exp, int*pass);
TypeNode* stmt_exp_id_call_anls(Node* exp, int* pass);
TypeNode* stmt_args_anls(Node* args, int* pass);

/*辅助函数*/
TypeNode* compare_params_actual(SymNode* notation, TypeNode* actual_params, int* pass);

