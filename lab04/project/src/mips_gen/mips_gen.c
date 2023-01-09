#include"../general.h"
#include"../utils/utils.h"

#include"../data_struct/spt.h"
#include"../data_struct/type_node.h"
#include"../data_struct/sym_node.h"
#include"../data_struct/sym_table.h"
#include"../data_struct/ir_list.h"

#include"mips_gen.h"

extern SymTable* table;

extern IrStmt* irlist_head;
extern IrStmt* irlist_tail;

IrSymTable* ir_symt_head = NULL;
IrSymTable* ir_symt_tail = NULL;

void init_data_seg(FILE* mips_file){
    /*初始化数据段*/
    fprintf(mips_file, ".data\n");
    fprintf(mips_file, "_prompt: .asciiz \"Enter an integer:\"\n");
    fprintf(mips_file, "_ret: .asciiz \"\\n\"\n");
    fprintf(mips_file, ".globl main\n");
}

void init_text_read_and_write(FILE* mips_file){
    /*初始化读写函数*/
    fprintf(mips_file, ".text\n");

    fprintf(mips_file, "read:\n");
    fprintf(mips_file, "\tli $v0, 4\n");
    fprintf(mips_file, "\tla $a0, _prompt\n");
    fprintf(mips_file, "\tsyscall\n");
    fprintf(mips_file, "\tli $v0, 5\n");
    fprintf(mips_file, "\tsyscall\n");
    fprintf(mips_file, "\tjr $ra\n");
    fprintf(mips_file, "\n");

    fprintf(mips_file, "write:\n");
    fprintf(mips_file, "\tli $v0, 1\n");
    fprintf(mips_file, "\tsyscall\n");
    fprintf(mips_file, "\tli $v0, 4\n");
    fprintf(mips_file, "\tla $a0, _ret\n");
    fprintf(mips_file, "\tsyscall\n");
    fprintf(mips_file, "\tmove $v0, $0\n");
    fprintf(mips_file, "\tjr $ra\n");
    fprintf(mips_file, "\n");
}

void mips_gen(FILE* mips_file){
    /*主函数-代码生成*/
    init_data_seg(mips_file);
    init_text_read_and_write(mips_file);
    ir_stmt2mips_static_anls();
    ir_stmt2mips(mips_file);
}

IrSymTable* new_ir_symt(){
    /*IR符号表-新建一个符号表*/
    IrSymTable* rst = (IrSymTable*)malloc(sizeof(IrSymTable));
    rst->func_name = NULL;
    rst->mem_size = 0;
    rst->vars_head = NULL;
    rst->vars_tail = NULL;
    rst->next = NULL;
    return rst;
}

IrSymNode* new_ir_symn(){
    /*IR符号表-新建一个符号表项*/
    IrSymNode* rst = (IrSymNode*)malloc(sizeof(IrSymNode));
    rst->size = 0;
    rst->offset = 0;
    rst->if_param = 0;
    rst->var_name = NULL;
    rst->next = NULL;
    return rst;
}

void add_new_symn2symt(IrSymTable* symt_tail, char* new_sym, int size, int if_param){
    /*IR符号表-追加新的符号表项*/
    if(new_sym[0] == '#') return;

    IrSymNode* cursor = symt_tail->vars_head;
    while(cursor != NULL){
        if(STREQ(cursor->var_name, new_sym)) break;
        else cursor = cursor->next;
    }
    if(cursor == NULL){
        IrSymNode* p = new_ir_symn();
        STRCP(p->var_name, new_sym);
        p->size = size;
        p->if_param = if_param;

        if(symt_tail->vars_head == NULL){
            if(if_param == 0) p->offset = 0;
            symt_tail->vars_head = p;
            symt_tail->vars_tail = p;
        }
        else{
            if(if_param == 0) p->offset = symt_tail->mem_size;
            symt_tail->vars_tail->next = p;
            symt_tail->vars_tail = p;
        }
        if(if_param == 0) symt_tail->mem_size += size;
    }
    else return;
}

IrSymTable* found_symt(char* tname, IrSymTable* tables){
    /*IR符号表-查找到指定名称的表格*/
    IrSymTable* cursor = tables;
    while(cursor != NULL){
        if(STREQ(cursor->func_name, tname)) return cursor;
        else cursor = cursor->next;
    }
    return NULL;
}

IrSymNode* found_symn(char* tname, char* nname){
    /*IR符号表, 指定名称表格下查找指定名称符号*/
    IrSymTable* table = found_symt(tname, ir_symt_head);
    IrSymNode* p = table->vars_head;
    while (p != NULL)
    {
        if(STREQ(p->var_name, nname)) return p;
        else p = p->next;
    }
    return NULL;
}

void fprintf_symt(FILE* fp, IrSymTable* table){
    /*IR符号表-打印符号表表项*/
    IrSymTable* cursor = table;
    while(cursor != NULL){
        fprintf(fp, "%s:\tsize:%d\n", cursor->func_name, cursor->mem_size);
        IrSymNode* p = cursor->vars_head;
        while(p != NULL){
            fprintf(fp, "\toffset:%d\tsize:%d\tparam:%s\t:\t%s\n", p->offset, p->size, (p->if_param?"YES":"NO"), p->var_name);
            p = p->next;
        }
        cursor = cursor->next;
    }
}

void ir_stmt2mips_static_anls(){
    /*静态分析IR中间代码符号表*/
    ir_symt_head = NULL;
    ir_symt_tail = NULL;

    IrStmt* cursor = irlist_head;
    FILE* fp = fopen("./guigui2.ir", "w");
    while(cursor != NULL){
        if(cursor->ir_type == IR_FUNCTION){
            IrSymTable* p = new_ir_symt();
            STRCP(p->func_name, cursor->params[0]);
            if(ir_symt_head == NULL){
                ir_symt_head = p;
                ir_symt_tail = p;
            }
            else{
                ir_symt_tail->next = p;
                ir_symt_tail = p;
            }
        }
        else if(
            cursor->ir_type == IR_C_ASSIGNOP ||
            cursor->ir_type == IR_VA_ASSIGNOP ||
            cursor->ir_type == IR_VP_ASSIGNOP ||
            cursor->ir_type == IR_PV_ASSIGNOP
        ){
            add_new_symn2symt(ir_symt_tail, cursor->params[0], 4, 0);
            add_new_symn2symt(ir_symt_tail, cursor->params[1], 4, 0);
        }
        else if(cursor->ir_type == IR_PARAM){
            add_new_symn2symt(ir_symt_tail, cursor->params[0], 4, 1);
        }
        else if(
            cursor->ir_type == IR_VF_ASSIGNOP ||
            cursor->ir_type == IR_ARG ||
            cursor->ir_type == IR_READ ||
            cursor->ir_type == IR_WRITE ||
            cursor->ir_type == IR_RETURN
        ){
            add_new_symn2symt(ir_symt_tail, cursor->params[0], 4, 0);
        }
        else if(
            cursor->ir_type == IR_PLUS ||
            cursor->ir_type == IR_MINUS ||
            cursor->ir_type == IR_STAR ||
            cursor->ir_type == IR_DIV
        ){
            add_new_symn2symt(ir_symt_tail, cursor->params[0], 4, 0);
            add_new_symn2symt(ir_symt_tail, cursor->params[1], 4, 0);
            add_new_symn2symt(ir_symt_tail, cursor->params[2], 4, 0);
        }
        else if(cursor->ir_type == IR_DEC){
            add_new_symn2symt(ir_symt_tail, cursor->params[0], atoi(cursor->params[1]), 0);
        }
        else if(cursor->ir_type == IR_IF_GOTO){
            add_new_symn2symt(ir_symt_tail, cursor->params[0], 4, 0);
            add_new_symn2symt(ir_symt_tail, cursor->params[2], 4, 0);
        }

        cursor = cursor->next;
    }

    IrSymTable* correct_symt = ir_symt_head;
    while(correct_symt != NULL){
        IrSymNode* p = correct_symt->vars_head;
        int temp = correct_symt->mem_size;
        while(p != NULL){
            if(p->if_param == 1){
                p->offset = temp;
                temp += p->size;
            }
            p = p->next;
        }
        correct_symt = correct_symt->next;
    }
    fprintf_symt(fp, ir_symt_head);
}

void ir_stmt2mips(FILE* mips_file){
    /*IR中间代码翻译为MIPS汇编代码*/
    int if_args = 0;
    char* cur_func = NULL;
    IrStmt* cursor = irlist_head;
    while(cursor != NULL){
        if(cursor->ir_type == IR_LABEL){
            fprintf(mips_file, "%s:\n", cursor->params[0]);
        }
        else if(cursor->ir_type == IR_FUNCTION){
            fprintf(mips_file, "\n%s:\n", cursor->params[0]);
            cur_func = cursor->params[0];
        }
        else if(cursor->ir_type == IR_C_ASSIGNOP){
            if(cursor->params[1][0] == '#'){
                fprintf(mips_file, "\tli $t0, %s\n",(cursor->params[1]+1));
                fprintf(mips_file, "\tsw $t0, %d($sp)\n", found_symn(cur_func, cursor->params[0])->offset + (if_args));
            }
            else{
                fprintf(mips_file, "\tlw $t0, %d($sp)\n", found_symn(cur_func, cursor->params[1])->offset + (if_args));
                fprintf(mips_file, "\tsw $t0, %d($sp)\n", found_symn(cur_func, cursor->params[0])->offset + (if_args));
            }
        }
        else if(cursor->ir_type == IR_VA_ASSIGNOP){
            fprintf(mips_file, "\tla $t0, %d($sp)\n", found_symn(cur_func, cursor->params[1])->offset + (if_args));
            fprintf(mips_file, "\tsw $t0, %d($sp)\n", found_symn(cur_func, cursor->params[0])->offset + (if_args));

            // fprintf(mips_file, "\tmove $t0, $sp\n");
            // fprintf(mips_file, "\taddi $t0, $t0, %d\n", found_symn(cur_func, cursor->params[1])->offset + (if_args));
            // fprintf(mips_file, "\tsw $t0, %d($sp)\n", found_symn(cur_func, cursor->params[0])->offset + (if_args));
        }
        else if(cursor->ir_type == IR_VP_ASSIGNOP){
            fprintf(mips_file, "\tlw $t0, %d($sp)\n", found_symn(cur_func, cursor->params[1])->offset + (if_args));
            fprintf(mips_file, "\tlw $t0, 0($t0)\n");
            fprintf(mips_file, "\tsw $t0, %d($sp)\n", found_symn(cur_func, cursor->params[0])->offset + (if_args));
        }
        else if(cursor->ir_type == IR_PV_ASSIGNOP){
            fprintf(mips_file, "\tlw $t0, %d($sp)\n", found_symn(cur_func, cursor->params[0])->offset + (if_args));
            fprintf(mips_file, "\tlw $t1, %d($sp)\n", found_symn(cur_func, cursor->params[1])->offset + (if_args));
            fprintf(mips_file, "\tsw $t1 0($t0)\n");
        }
        else if(cursor->ir_type == IR_VF_ASSIGNOP){
            if(if_args == 0){
                fprintf(mips_file, "\taddi $sp, $sp, -4\n");
                fprintf(mips_file, "\tsw $ra, 0($sp)\n");
            }

            fprintf(mips_file, "\taddi $sp, $sp, -%d\n", found_symt(cursor->params[1], ir_symt_head)->mem_size);

            fprintf(mips_file, "\tjal %s\n", cursor->params[1]);

            int return_params = found_symt(cursor->params[1], ir_symt_head)->mem_size;
            IrSymNode* a = found_symt(cursor->params[1], ir_symt_head)->vars_head;
            while(a != NULL){
                if(a->if_param == 1) return_params += a->size;
                a = a->next;
            }
            if(return_params != 0){
                fprintf(mips_file, "\taddi $sp, $sp, %d\n", return_params);
            }

            fprintf(mips_file, "\tlw $ra, 0($sp)\n");
            fprintf(mips_file, "\taddi $sp, $sp, 4\n");
            if_args = 0;

            fprintf(mips_file, "\tsw $v0, %d($sp)\n", found_symn(cur_func, cursor->params[0])->offset + (if_args));
        }
        else if(
            cursor->ir_type == IR_PLUS ||
            cursor->ir_type == IR_MINUS ||
            cursor->ir_type == IR_STAR ||
            cursor->ir_type == IR_DIV
        ){
            if(cursor->ir_type == IR_PLUS && cursor->params[2][0] == '#'){
                fprintf(mips_file, "\tlw $t1, %d($sp)\n", found_symn(cur_func, cursor->params[1])->offset + (if_args));
                fprintf(mips_file, "\taddi $t0, $t1, %s\n", (cursor->params[2] + 1));
                fprintf(mips_file, "\tsw $t0, %d($sp)\n", found_symn(cur_func, cursor->params[0])->offset + (if_args));
            }
            else if(cursor->ir_type == IR_MINUS && cursor->params[2][0] == '#'){
                fprintf(mips_file, "\tlw $t1, %d($sp)\n", found_symn(cur_func, cursor->params[1])->offset + (if_args));
                fprintf(mips_file, "\taddi $t0, $t1, -%s\n", (cursor->params[2] + 1));
                fprintf(mips_file, "\tsw $t0, %d($sp)\n", found_symn(cur_func, cursor->params[0])->offset + (if_args));
            }
            else{
                fprintf(mips_file, "\tlw $t1, %d($sp)\n", found_symn(cur_func, cursor->params[1])->offset + (if_args));
                fprintf(mips_file, "\tlw $t2, %d($sp)\n", found_symn(cur_func, cursor->params[2])->offset + (if_args));
                if(cursor->ir_type == IR_PLUS) fprintf(mips_file, "\tadd $t0, $t1, $t2\n");
                else if(cursor->ir_type == IR_MINUS) fprintf(mips_file, "\tsub $t0, $t1, $t2\n");
                else if(cursor->ir_type == IR_STAR) fprintf(mips_file, "\tmul $t0, $t1, $t2\n");
                else if(cursor->ir_type == IR_DIV) fprintf(mips_file, "\tdiv $t0, $t1, $t2\n");
                fprintf(mips_file, "\tsw $t0, %d($sp)\n", found_symn(cur_func, cursor->params[0])->offset + (if_args));
            }
        }
        else if(cursor->ir_type == IR_GOTO){
            fprintf(mips_file, "\tj %s\n", cursor->params[0]);
        }
        else if(cursor->ir_type == IR_IF_GOTO){
            fprintf(mips_file, "\tlw $t0, %d($sp)\n", found_symn(cur_func, cursor->params[0])->offset + (if_args));
            fprintf(mips_file, "\tlw $t1, %d($sp)\n", found_symn(cur_func, cursor->params[2])->offset + (if_args));
            if(STREQ(cursor->params[1], "==")) fprintf(mips_file, "\tbeq $t0, $t1, %s\n", cursor->params[3]);
            else if(STREQ(cursor->params[1], "!=")) fprintf(mips_file, "\tbne $t0, $t1, %s\n", cursor->params[3]);
            else if(STREQ(cursor->params[1], ">")) fprintf(mips_file, "\tbgt $t0, $t1, %s\n", cursor->params[3]);
            else if(STREQ(cursor->params[1], "<")) fprintf(mips_file, "\tblt $t0, $t1, %s\n", cursor->params[3]);
            else if(STREQ(cursor->params[1], ">=")) fprintf(mips_file, "\tbge $t0, $t1, %s\n", cursor->params[3]);
            else if(STREQ(cursor->params[1], "<=")) fprintf(mips_file, "\tble $t0, $t1, %s\n", cursor->params[3]);
            else assert(0);
        }
        else if(cursor->ir_type == IR_RETURN){
            fprintf(mips_file, "\tlw $v0, %d($sp)\n", found_symn(cur_func, cursor->params[0])->offset + (if_args));
            fprintf(mips_file, "\tjr $ra\n");
        }
        else if(cursor->ir_type == IR_ARG){
            if(if_args == 0){
                if_args = 4;
                fprintf(mips_file, "\taddi $sp, $sp, -4\n");
                fprintf(mips_file, "\tsw $ra, 0($sp)\n");
            }
            fprintf(mips_file, "\tlw $t0, %d($sp)\n", found_symn(cur_func, cursor->params[0])->offset + (if_args));
            fprintf(mips_file, "\taddi $sp, $sp, -4\n");
            fprintf(mips_file, "\tsw $t0, 0($sp)\n");
            if_args += 4;
        }
        else if(cursor->ir_type == IR_WRITE){
            fprintf(mips_file, "\tlw $a0, %d($sp)\n", found_symn(cur_func, cursor->params[0])->offset + (if_args));

            fprintf(mips_file, "\taddi $sp, $sp, -4\n");
            fprintf(mips_file, "\tsw $ra, 0($sp)\n");
            fprintf(mips_file, "\tjal write\n");
            fprintf(mips_file, "\tlw $ra, 0($sp)\n");
            fprintf(mips_file, "\taddi $sp, $sp, 4\n");
        }
        else if(cursor->ir_type == IR_READ){
            fprintf(mips_file, "\taddi $sp, $sp, -4\n");
            fprintf(mips_file, "\tsw $ra, 0($sp)\n");
            fprintf(mips_file, "\tjal read\n");
            fprintf(mips_file, "\tlw $ra, 0($sp)\n");
            fprintf(mips_file, "\taddi $sp, $sp, 4\n");

            fprintf(mips_file, "\tsw $v0, %d($sp)\n", found_symn(cur_func, cursor->params[0])->offset + (if_args));
        }
        cursor = cursor->next;
    }
}


