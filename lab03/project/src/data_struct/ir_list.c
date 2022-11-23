#include"../general.h"
#include"../utils/utils.h"

#include"ir_list.h"

IrStmt* new_ir_stmt(int num, ...){
    /*
    创建一条基本的中间代码语句, 初始化相关参数
    Format : new_ir_stmt(<params_num> + 1, <ir_stmt_type>, <param>...)
    */
    IrStmt* p = (IrStmt*)malloc(sizeof(IrStmt));

    p->param_num = num-1;
    p->params = (char**)malloc((num-1) * sizeof(char*));

    va_list valist;
    va_start(valist, num);
    p->ir_type = va_arg(valist, IrType);

    for(int i = 0; i < num-1; i++){
        char* tmp = va_arg(valist, char*);
        STRCP(p->params[i], tmp);
    }

    p->next = NULL;
    p->prev = NULL;
    return p;
}

void add_ir_stmt(IrStmt** head, IrStmt** tail, IrStmt* p){
    /*在中间代码链表尾部追加一条中间代码语句*/
    if((*head) == NULL){
        (*head) = p;
        (*tail) = p;
    }
    else{
        (*tail)->next = p;
        p->prev = (*tail);
        (*tail) = p;
    }
}

void fprintf_ir(FILE* ir_file, IrStmt* mem_irs){
    /*将中间代码从内存打印到文件中*/
    if(mem_irs == NULL) return;
    IrStmt* p = mem_irs;
    while(p != NULL){
        switch (p->ir_type)
        {
        case IR_LABEL:          fprintf(ir_file, "LABEL %s :", p->params[0]);                                                       break;
        case IR_FUNCTION:       fprintf(ir_file, "FUNCTION %s :", p->params[0]);                                                    break;
        case IR_C_ASSIGNOP:     fprintf(ir_file, "%s := %s", p->params[0], p->params[1]);                                           break;
        case IR_VA_ASSIGNOP:    fprintf(ir_file, "%s := &%s", p->params[0], p->params[1]);                                          break;
        case IR_VP_ASSIGNOP:    fprintf(ir_file, "%s := *%s", p->params[0], p->params[1]);                                          break;        
        case IR_PV_ASSIGNOP:    fprintf(ir_file, "*%s := %s", p->params[0], p->params[1]);                                          break;
        case IR_VF_ASSIGNOP:    fprintf(ir_file, "%s := CALL %s", p->params[0], p->params[1]);                                      break;
        case IR_PLUS:           fprintf(ir_file, "%s := %s + %s", p->params[0], p->params[1], p->params[2]);                        break;
        case IR_MINUS:          fprintf(ir_file, "%s := %s - %s", p->params[0], p->params[1], p->params[2]);                        break;
        case IR_STAR :          fprintf(ir_file, "%s := %s * %s", p->params[0], p->params[1], p->params[2]);                        break;
        case IR_DIV:            fprintf(ir_file, "%s := %s / %s", p->params[0], p->params[1], p->params[2]);                        break;
        case IR_GOTO:           fprintf(ir_file, "GOTO %s", p->params[0]);                                                          break;
        case IR_IF_GOTO:        fprintf(ir_file, "IF %s %s %s GOTO %s", p->params[0], p->params[1], p->params[2], p->params[3]);    break;
        case IR_RETURN:         fprintf(ir_file, "RETURN %s", p->params[0]);                                                        break;
        case IR_DEC:            fprintf(ir_file, "DEC %s %s", p->params[0], p->params[1]);                                          break;
        case IR_ARG:            fprintf(ir_file, "ARG %s", p->params[0]);                                                           break;
        case IR_PARAM:          fprintf(ir_file, "PARAM %s", p->params[0]);                                                         break;
        case IR_READ:           fprintf(ir_file, "READ %s", p->params[0]);                                                          break;
        case IR_WRITE:          fprintf(ir_file, "WRITE %s", p->params[0]);                                                         break;        
        default: assert(0); break;
        }
        fprintf(ir_file, "\n");
        p = p->next;
    }
}