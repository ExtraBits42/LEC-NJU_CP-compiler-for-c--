#pragma once
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<assert.h>
#include<stdarg.h>

# define YYLTYPE_IS_DECLARED 1
typedef struct YYLTYPE  
{  
    int first_line;  
    int first_column;  
    int last_line;  
    int last_column;  
} YYLTYPE;

/*语法分析-构建语法分析树*/
typedef enum{LEXICAL, SYNTAX}Ntype;
typedef struct TreeNode{
    Ntype ntype;
    char* name;
    union {
        int syn_line;
        char* lex_val;
    }u;
    struct TreeNode* children;
    struct TreeNode* tail;
    struct TreeNode* next;
} Node;

/*语义分析-构建符号表*/
typedef enum{STRUCTT, FUNC, VAR}SymType;
typedef enum{CONSTANT, BASIC, ARRAY, STRUCTTYPE}DataType;

typedef struct SymNode SymNode;

typedef struct TypeNode
{
    DataType type;
    // 基本类型、结构体类型使用
    char* type_name;
    // 结构体类型
    SymNode* struct_type;
    // 数组类型使用
    int arr_size;
    struct TypeNode* arr_ele_type;
    // 备用
    struct TypeNode* next;
} TypeNode;

struct SymNode{
    char* name;
    SymType type;

    struct
    {
        struct TypeNode* var_type;
    }VarInfo;
    struct
    {
        struct TypeNode* func_return_type;
        struct SymNode* params_list;
    }FuncInfo;
    struct
    {
        struct SymNode* field_next;
    }StructInfo;
    
    struct SymNode* hash_next;
    struct SymNode* scope_next;
};

typedef struct SymHashStack{
    int hash_size;
    int stack_size;
    int st_ptr;
    struct SymNode** hash_data;
    struct SymNode** stack_data;
    struct SymNode** stack_tails;
}SymHashStk;

/*宏定义-简化C Language函数*/
#define BASIC_TYPE_INT "int"
#define BASIC_TYPE_FLOAT "float"

#define LOG_SEMANTIC_ANLS_ERROR(code, line, info) \
    printf("Error type %d at Line %d: %s.\n", code, line, info);

#define STREQ(str1, str2) \
    (strcmp(str1, str2) == 0? 1 : 0)

#define STRCP(dest, src) \
    { \
        dest = (char*)malloc(strlen((src)) * sizeof(char)); \
        strcpy((dest), (src)); \
    }



