#include"../general.h"
#include"utils.h"

#include"../data_struct/type_node.h"
#include"../data_struct/sym_node.h"
#include"../data_struct/sym_table.h"

int max(int a, int b){
    /*最大值*/
    return (a > b ? a : b);
}

int min(int a, int b){
    /*最小值*/
    return (a > b ? b : a);
}

int get_int_len(int n){
    /*获取int型数字n十进制表示下的长度*/
    int int_len = 0;
    if(n == 0) int_len = 1;
    else{
        while(n != 0){
            n /= 10;
            int_len++;
        }
    }
    return int_len;
}

char* int2str(int n){
    /*整数转换为字符串数字 : xxx->"xxx"*/
    int n_len = get_int_len(n);
    char* rst = (char*)malloc(n_len * sizeof(char));
    if(n == 0) rst[0] = '0';
    else{
        while(n_len != 0){
            rst[n_len - 1] = ('0' + n % 10);
            n /= 10;
            n_len--;
        }
    }
    return rst;
}

char* int2_imstr(int n){
    /*整数转换为字符串立即数 : xxx->"#xxx"*/
    char* str_num = int2str(n);
    char* rst = (char*)malloc(sizeof(char) * (strlen(str_num) + 2));
    strcpy(rst, "#");
    strcat(rst, str_num);
    free(str_num);
    return rst;
}

char* new_name_xxx(int n, char* vname){
    /*创建序列化命名字符串：<指定名称><数字序号>*/
    char* num = int2str(n);
    char* rst = (char*)malloc(sizeof(char)*(strlen(vname) + strlen(num) + 1));
    strcpy(rst, vname);
    strcat(rst, num);
    free(num);
    return rst;
}


void syms_pieces_cat(SymNode** src1, int size1, SymNode** src2, int size2){
    /*合并两个SymNode符号数组，结果存储在src1中，原本的两个数组将被清空*/
    SymNode* tmp = (SymNode*)malloc((size1 + size2) * sizeof(SymNode));
    if(size1 > 0) memcpy(tmp, (*src1), size1 * sizeof(SymNode));
    if(size2 > 0) memcpy((tmp + size1), (*src2), size2 * sizeof(SymNode));
    free(*src1);
    free(*src2);
    (*src1) = tmp;
    (*src2) = NULL;
}

int get_array_size(TypeNode* arrayt){
    /*获取数组类型大小*/
    assert(arrayt != NULL);
    if(arrayt->ntype == BASIC_T){
        assert((
            STREQ(arrayt->basic_v.basic_type, BASIC_TYPE_INT) ||
            STREQ(arrayt->basic_v.basic_type, BASIC_TYPE_FLOAT)
        ));
        return BASIC_TYPE_SIZE;
    }
    else if(arrayt->ntype == STRUCT_T){
        assert(arrayt->struct_v.def_ptr != NULL);
        return get_struct_size(arrayt->struct_v.def_ptr);
    }
    else if(arrayt->ntype == ARRAY_T){
        return arrayt->array_v.size * get_array_size(arrayt->array_v.ele_type);
    }
    else assert(0);
}

void get_array_info(TypeNode* arrayt, TypeNode** ele_type, int* dimension_num, int** dimensions){
    /*获取数组信息 : 基本元素类型、维度数目、各个维度尺寸*/
    assert(arrayt != NULL);
    assert(arrayt->ntype == ARRAY_T);
    TypeNode* cursor = arrayt;
    while(cursor->ntype == ARRAY_T){
        (*dimension_num) += 1;
        cursor = cursor->array_v.ele_type;
    }
    (*dimensions) = (int*)malloc(sizeof(int) * (*dimension_num));
    cursor = arrayt;
    int index = 0;
    while(cursor->ntype == ARRAY_T){
        (*dimensions)[index] = cursor->array_v.size;
        index += 1;
        cursor = cursor->array_v.ele_type;
    }
    (*ele_type) = cursor;
}

int get_struct_size(SymNode* structv){
    /*获取结构体类型大小*/
    assert(structv->ntype == STRUCT_SYM);
    int structv_size = 0;
    for(int i = 0; i < structv->struct_v.mem_num; i++){
        SymNode* member = &(structv->struct_v.members[i]);
        assert(member->ntype == VAR_SYM);
        TypeNode* type_node = member->var_v.vtype;
        if(type_node->ntype == STRUCT_T){         // 结构体类型成员
            structv_size += get_struct_size(member);
        }
        else if(type_node->ntype == ARRAY_T){     // 数组类型成员
            structv_size += get_array_size(member->var_v.vtype);
        }
        else if(type_node->ntype == BASIC_T){     // 基本类型成员
            assert((
                STREQ(type_node->basic_v.basic_type, BASIC_TYPE_INT) ||
                STREQ(type_node->basic_v.basic_type, BASIC_TYPE_FLOAT)
            ));
            structv_size += BASIC_TYPE_SIZE;
        }
        else assert(0);
    }
    return structv_size;
}

int get_type_size(TypeNode* type_node){
    /*获取类型节点所指类型的大小*/
    assert(type_node != NULL);
    if(type_node->ntype == ARRAY_T){
        return get_array_size(type_node);
    }
    else if(type_node->ntype == STRUCT_T){
        return get_struct_size(type_node->struct_v.def_ptr);
    }
    else if(type_node->ntype == BASIC_T){
        assert((
            STREQ(type_node->basic_v.basic_type, BASIC_TYPE_INT) ||
            STREQ(type_node->basic_v.basic_type, BASIC_TYPE_FLOAT)
        ));
        return BASIC_TYPE_SIZE;
    }
    else assert(0);
}

int get_member_offset(SymNode* structv, char* tar_mem){
    /*获取结构体中指定成员相对于结构体的偏移量*/
    assert(structv != NULL);
    assert(structv->ntype == STRUCT_SYM);
    int offset_size = 0;
    int i = 0;
    for(i = 0; i < structv->struct_v.mem_num; i++){
        if(STREQ(structv->struct_v.members[i].name, tar_mem)) break;
        else{
            SymNode* member = &(structv->struct_v.members[i]);
            assert(member->ntype == VAR_SYM);
            TypeNode* mem_type = member->var_v.vtype;
            if(mem_type->ntype == BASIC_T){
                assert((
                    STREQ(mem_type->basic_v.basic_type, BASIC_TYPE_INT) ||
                    STREQ(mem_type->basic_v.basic_type, BASIC_TYPE_FLOAT)
                ));
                offset_size += BASIC_TYPE_SIZE;
            }
            else if(mem_type->ntype == STRUCT_T){
                SymNode* mem_struct_def = mem_type->struct_v.def_ptr;
                offset_size += get_struct_size(mem_struct_def);
            }
            else if(mem_type->ntype == ARRAY_T){
                offset_size += get_array_size(mem_type);
            }
            else assert(0);
        }
    }
    assert(i < structv->struct_v.mem_num);
    return offset_size;
}

void print_tree(Node* root, int blank_num){
    /*打印语法分析树*/
    if(root->ntype == LEXICAL){
        for(int i = 0; i < blank_num; i++) printf(" ");
        if(STREQ(root->name, "ID")) printf("ID: %s\n", root->u.lex_val);
        else if(STREQ(root->name, "TYPE")) printf("TYPE: %s\n", root->u.lex_val);
        else if(STREQ(root->name, "INT")) printf("INT: %d\n", atoi(root->u.lex_val));
        else if(STREQ(root->name, "FLOAT")) printf("FLOAT: %f\n", atof(root->u.lex_val));
        else printf("%s\n", root->name);
    }
    else if(root->ntype == SYNTAX){
        if(root->children != NULL){
            for(int i = 0; i < blank_num; i++) printf(" ");
            printf("%s (%d)\n", root->name, root->u.syn_line);
        }
        Node* cursor = root->children;
        while(cursor != NULL){
            print_tree(cursor, blank_num+2);
            cursor = cursor->next;
        }
    }
}



