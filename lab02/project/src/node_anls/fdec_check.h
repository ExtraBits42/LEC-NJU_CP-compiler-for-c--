/*
模块:
    Name: Function Declaration/Definition Check-函数定义/声明检查
*/

#pragma once
#include"../general.h"

//顶层函数-函数定义分析
void fdecs_anls(Node* extdef, int*pass, int ifdec);

//节点分析函数
SymNode* fdecs_fundec_anls(Node* fundec, int*pass, int ifdec);
SymNode* fdecs_varlist_anls(Node* varlist, int*pass);
SymNode* fdecs_paramdec_anls(Node* paramdec, int*pass);


