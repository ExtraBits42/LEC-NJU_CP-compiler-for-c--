#pragma once

typedef struct IrSymNode
{
    char* var_name;
    int size;
    int offset;
    int if_param;
    struct IrSymNode* next;
}IrSymNode;

typedef struct IrSymTable
{
    char* func_name;
    int mem_size;
    struct IrSymNode* vars_head;
    struct IrSymNode* vars_tail;
    struct IrSymTable* next;
}IrSymTable;

// 初始化-数据段
void init_data_seg(FILE* mips_file);
// 初始化-读写函数
void init_text_read_and_write(FILE* mips_file);
// 静态分析IR中间代码符号表
void ir_stmt2mips_static_anls();
// IR中间代码翻译为MIPS汇编代码
void ir_stmt2mips(FILE* mips_file);

// 主函数-代码生成
void mips_gen(FILE* mips_file);

