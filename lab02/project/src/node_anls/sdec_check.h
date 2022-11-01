/*
模块:
    Name: Struct Declaration/Definition Check-结构体定义检查
*/

#pragma once
#include"../general.h"

//顶层函数-结构体定义分析
void sdecs_anls(Node* extdef, int* pass);

//节点分析函数
char* sdecs_structspecifier_anls(Node* struct_spec, int* pass);
char* sdecs_opttag_anls(Node* opt_tag);
void sdecs_declist_anls(char* type, Node* declist, int* pass, int ifstruct);
void sdecs_deflist_anls(Node* deflist, int* pass, int ifstruct);




