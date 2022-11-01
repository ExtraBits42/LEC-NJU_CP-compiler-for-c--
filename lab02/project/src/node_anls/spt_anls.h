#include"../general.h"

// 静态分析: 获取符号最大数目、最大作用域深度
void semantic_anls(Node* root, int* pass);
void get_static_notations_info(Node* root, int* num, int* depth);
void static_notations_anls_dfs(Node* root, int* num, int* cur_depth, int* max_depth);

// 语义分析主程序
void top_dfs_anls(Node* root, int* pass);


