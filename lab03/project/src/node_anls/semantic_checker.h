#pragma once

#include"../general.h"
#include"../utils/utils.h"

#include"../data_struct/type_node.h"
#include"../data_struct/sym_node.h"
#include"../data_struct/sym_table.h"

/*
Error Code Table:
- 变量类:
    1. 变量在使用时未经定义
    3. 变量重复定义OR变量与已定义的结构体名字重复
    17. 直接使用未定义过的结构体来定义变量
- 数组类:
    10. 对非数组变量使用"[]"操作符
    12. 数组访问操作符"[]"中出现非整数
- 函数类:
    2. 函数在调用时未经定义
    4. 函数出现重复定义
- 结构体类:
    13. 对非结构体类型变量使用"."操作符
    14. 访问结构体中未定义过的域
    15. 结构体中域名重复定义，或在定义时对域进行初始化
    16. 结构体名字与前面定义过的结构体或变量的名字重复
- 运算类:
    5. 赋值号两边的表达式类型不匹配
    6. 赋值号左边出现一个只有右值的表达式
    7. 操作数类型不匹配或操作数类型与操作符不匹配
    8. return语句的返回类型与函数定义的返回类型不匹配
    9. 函数调用时实参与形参类型不匹配
    11. 对普通变量使用"()"(函数调用)操作符
*/

#define LOG_SEMANTIC_ANLS_ERROR(code, line, info) \
    printf("Error type %d at Line %d: %s.\n", code, line, info); \
    pass = 0;

// 检查错误号 : 1 - 变量在使用时未经定义
int check_semantic_error_1(Node* id);
// 检查错误号 : 3 - 变量重复定义OR变量与已定义的结构体名字重复
int check_semantic_error_3(Node* id);
// 检查错误号 : 17 - 直接使用未定义过的结构体来定义变量
int check_semantic_error_17(TypeNode* type, Node* helper);

// 检查错误号 : 10 - 对非数组变量使用"[]"操作符
int check_semantic_error_10(TypeNode* type, Node* exp);
// 检查错误号 : 12 - 数组访问操作符"[]"中出现非整数
int check_semantic_error_12(TypeNode* type, Node* exp);
// 检查错误号 : 2 - 函数在调用时未经定义
int check_semantic_error_2(Node* func_id, Node* exp);
// 检查错误号 : 4 - 函数出现重复定义
int check_semantic_error_4(Node* func_id, Node* fundec);

// 检查错误号 : 13 - 对非结构体类型变量使用"."操作符
int check_semantic_error_13(TypeNode* type, Node* exp);
// 检查错误号 : 14 - 访问结构体中未定义过的域
SymNode* check_semantic_error_14(TypeNode* type, Node* exp);
// 检查错误号 : 15 - 结构体中域名重复定义，或在定义时对域进行初始化
// TODO
// 检查错误号 : 16 - 结构体名字与前面定义过的结构体或变量的名字重复
int check_semantic_error_16(char* struct_name, Node* opttag);

// 检查错误号 : 5 - 赋值号两边的表达式类型不匹配
int check_semantic_error_5(TypeNode* type1, TypeNode* type2, Node* helper);
// 检查错误号 : 6 - 赋值号左边出现一个只有右值的表达式
int check_semantic_error_6(TypeNode* type, Node* helper);
// 检查错误号 : 7 - 操作数类型不匹配或操作数类型与操作符不匹配



