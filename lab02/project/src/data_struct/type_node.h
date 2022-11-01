/*
模块:
    Name: Type Node-类型节点
*/

#include"../general.h"

// 新建类型节点
// Basic: 新建空白类型节点
TypeNode* new_type_node();
// Induction: 新建变量/常量/数组类型节点
TypeNode* new_common_type_node(char* v_type);
TypeNode* new_array_type_node(int arr_size);
TypeNode* new_constant_type_node(char* v_type);

// 克隆类型节点
TypeNode* clone_type_node(TypeNode* ori_node);

// 比较类型节点
int equal_type_nodes(TypeNode* node1, TypeNode* node2, int* pass);

// 删除类型节点
void free_type_node(TypeNode* node);

