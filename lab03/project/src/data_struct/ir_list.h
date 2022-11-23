#pragma once

typedef enum IrType{
    IR_LABEL,       // LABEL x :                : 定义标号x   
    IR_FUNCTION,    // FUNCTION f :             : 定义函数f
    IR_C_ASSIGNOP,  // x := y                   : 赋值操作        
    IR_VA_ASSIGNOP, // x := &y                  : 取y地址赋给x
    IR_VP_ASSIGNOP, // x := *y                  : 取以y值为地址的内存单元的内容赋给x
    IR_PV_ASSIGNOP, // *x := y                  : 取y值赋给以x值为地址的内存单元
    IR_VF_ASSIGNOP, // x := CALL f              : 调用函数，并将其返回值赋给x
    IR_PLUS,        // x := y + z               : 加法操作
    IR_MINUS,       // x := y - z               : 减法操作
    IR_STAR,        // x := y * z               : 乘法操作
    IR_DIV,         // x := y / z               : 除法操作
    IR_GOTO,        // GOTO x                   : 无条件跳转至标号x
    IR_IF_GOTO,     // IF x [relop] y GOTO z    : 如果x与y满足[relop]关系则跳转至标号z
    IR_RETURN,      // RETURN x                 : 退出当前函数并返回x值
    IR_DEC,         // DEC x [size]             : 内存空间申请，大小为4的倍数
    IR_ARG,         // ARG x                    : 传实参x
    IR_PARAM,       // PARAM x                  : 函数参数声明
    IR_READ,        // READ x                   : 从控制台读取x的值
    IR_WRITE,       // WRITE x                  : 向控制台打印x的值
}IrType;

typedef struct IrStmt{
    enum IrType ir_type;
    int param_num;
    char** params;
    struct IrStmt* next;
    struct IrStmt* prev;
}IrStmt;

#define READ_FUNC "read"
#define WRITE_FUNC "write"

// 创建一条中间语句代码
IrStmt* new_ir_stmt(int num, ...);

// 将中间代码从内存打印到文件中
void fprintf_ir(FILE* ir_file, IrStmt* mem_irs);

// 在中间代码链表尾部追加一条中间代码语句
void add_ir_stmt(IrStmt** head, IrStmt** tail, IrStmt* p);