#include"../general.h"
#include"../utils/utils.h"

#include"../data_struct/spt.h"
#include"../data_struct/type_node.h"
#include"../data_struct/sym_node.h"
#include"../data_struct/sym_table.h"

#include"../data_struct/ir_list.h"

#include"ir_gen.h"
#include"ir_spt_anls.h"

#define IR_OPTIMIZATION

extern SymTable* table;

extern IrStmt* irlist_head;
extern IrStmt* irlist_tail;

void init_read_and_write(SymTable* table){
    /*在符号表中初始化READ和WRITE函数*/
    TypeNode* r_rtype = new_basic_type_node(BASIC_TYPE_INT, CONSTANT);
    SymNode* read = new_func_node(READ_FUNC, r_rtype, 0, NULL);

    TypeNode* w_rtype = new_basic_type_node(BASIC_TYPE_INT, CONSTANT);
    TypeNode* w_ptype = new_basic_type_node(BASIC_TYPE_INT, CONSTANT);
    SymNode* param = new_var_sym_node("x", w_ptype);
    SymNode* write = new_func_node(WRITE_FUNC, w_rtype, 1, param);

    hash_insert(read, table);
    hash_insert(write, table);
}

void ir_opt1_immx2(){
    /*中间代码优化-常量消除*/
    IrStmt* cursor = irlist_head;
    while(cursor->next->next->next != NULL){
        IrStmt* imm1 = cursor->next;
        IrStmt* imm2 = cursor->next->next;
        IrStmt* fs = cursor->next->next->next;
        if(
            imm1->ir_type == IR_C_ASSIGNOP && imm1->params[1][0] == '#' &&
            imm2->ir_type == IR_C_ASSIGNOP && imm2->params[1][0] == '#' &&
            (fs->ir_type == IR_PLUS || fs->ir_type == IR_MINUS || fs->ir_type == IR_STAR || fs->ir_type == IR_DIV) &&
            STREQ(fs->params[1], imm1->params[0]) &&
            STREQ(fs->params[2], imm2->params[0])
        ){
            int temp = atoi(imm1->params[1] + 1) * atoi(imm2->params[1] + 1);
            fs->ir_type = IR_C_ASSIGNOP;
            fs->params[1] = int2_imstr(temp);
            cursor->next = cursor->next->next->next;
        }
        else if(
            imm1->ir_type == IR_C_ASSIGNOP && imm1->params[1][0] == '#' &&
            imm2->ir_type == IR_C_ASSIGNOP &&
            STREQ(imm1->params[0], imm2->params[1])
        ){
            imm2->params[1] = imm1->params[1];
            cursor->next = cursor->next->next;
        }
        cursor = cursor->next;
    }
}

void ir_opt2_repeat_assign(){
    /*中间代码优化-重复赋值消除*/
    IrStmt* cursor = irlist_head;
    while(cursor->next->next != NULL){
        IrStmt* stmt1 = cursor->next;
        IrStmt* stmt2 = cursor->next->next;
        if(
            stmt1->ir_type == IR_C_ASSIGNOP &&
            stmt2->ir_type == IR_C_ASSIGNOP &&
            STREQ(stmt1->params[0], stmt2->params[0])
        ){
            cursor->next = cursor->next->next;
        }
        else if(
            (stmt1->ir_type == IR_PLUS || stmt1->ir_type == IR_MINUS || stmt1->ir_type == IR_STAR || stmt1->ir_type == IR_DIV) &&
            stmt2->ir_type == IR_C_ASSIGNOP &&
            STREQ(stmt2->params[1], stmt1->params[0])
        ){
            STRCP(stmt1->params[0], stmt2->params[0]);
            cursor->next->next = cursor->next->next->next;
        }
        else if(
            (stmt1->ir_type == IR_C_ASSIGNOP && stmt1->params[1][0] == '#') &&
            ((stmt2->ir_type == IR_PLUS || stmt2->ir_type == IR_MINUS) && STREQ(stmt2->params[2], stmt1->params[0]))
        ){
            stmt2->params[2] = stmt1->params[1];
            cursor->next = cursor->next->next;
        }
        else if(
            (stmt1->ir_type == IR_PLUS || stmt1->ir_type == IR_MINUS || stmt1->ir_type == IR_STAR || stmt1->ir_type == IR_DIV) &&
            (stmt1->ir_type == stmt2->ir_type) &&
            (STREQ(stmt1->params[1], stmt2->params[1])) &&
            (STREQ(stmt1->params[2], stmt2->params[2]))
        ){
            stmt2->ir_type = IR_C_ASSIGNOP;
            stmt2->params[1] = stmt1->params[0];
            cursor = cursor->next;
        }
        else cursor = cursor->next;
    }
}

void ir_opt3_zero(){
    /*中间代码优化-零值消除*/
    IrStmt* cursor = irlist_head;
    while(cursor->next != NULL){
        if(cursor->ir_type == IR_PLUS && STREQ(cursor->params[2], "#0")){
            cursor->ir_type = IR_C_ASSIGNOP;
        }
        cursor = cursor->next;
    }
}

int if_dead_var(IrStmt* irlist, char* var){
    int rst = 1;
    IrStmt* cursor = irlist;
    while(cursor != NULL){
        if(
            (cursor->ir_type == IR_C_ASSIGNOP ||
            cursor->ir_type == IR_VP_ASSIGNOP ||
            cursor->ir_type == IR_VF_ASSIGNOP ||
            cursor->ir_type == IR_VA_ASSIGNOP ||
            cursor->ir_type == IR_PLUS ||
            cursor->ir_type == IR_MINUS ||
            cursor->ir_type == IR_STAR ||
            cursor->ir_type == IR_DIV ||
            cursor->ir_type == IR_READ) &&
            STREQ(cursor->params[0], var)
        ){
            rst = 0;
            break;
        }
        cursor = cursor->next;
    }
    return rst;
}

int if_can_replace(IrStmt* irlist, char* var, char* new_var){
    int rst = 1;
    int if_changed = 0;
    IrStmt* cursor = irlist;
    while(cursor != NULL){
        if(
            (cursor->ir_type == IR_C_ASSIGNOP ||
            cursor->ir_type == IR_VP_ASSIGNOP ||
            cursor->ir_type == IR_VF_ASSIGNOP ||
            cursor->ir_type == IR_VA_ASSIGNOP ||
            cursor->ir_type == IR_PLUS ||
            cursor->ir_type == IR_MINUS ||
            cursor->ir_type == IR_STAR ||
            cursor->ir_type == IR_DIV ||
            cursor->ir_type == IR_READ) &&
            STREQ(cursor->params[0], new_var)
        ){
            if_changed = 1;
        }

        if(
            (cursor->ir_type == IR_C_ASSIGNOP ||
            cursor->ir_type == IR_VP_ASSIGNOP ||
            cursor->ir_type == IR_VF_ASSIGNOP ||
            cursor->ir_type == IR_VA_ASSIGNOP) &&
            STREQ(cursor->params[1], var) &&
            if_changed == 1
        ){
            rst = 0;
            break;
        }
        else if(
            (cursor->ir_type == IR_PLUS ||
            cursor->ir_type == IR_MINUS ||
            cursor->ir_type == IR_STAR ||
            cursor->ir_type == IR_DIV) &&
            (STREQ(cursor->params[1], var) || STREQ(cursor->params[2], var)) &&
            if_changed == 1
        ){
            rst = 0;
            break;
        }
        else if(
            (cursor->ir_type == IR_IF_GOTO) &&
            (STREQ(cursor->params[0], var) || STREQ(cursor->params[2], var)) &&
            if_changed == 1
        ){
            rst = 0;
            break;
        }
        else if(
            (cursor->ir_type == IR_PV_ASSIGNOP) &&
            (STREQ(cursor->params[0], var) || STREQ(cursor->params[1], var)) &&
            if_changed == 1
        ){
            rst = 0;
            break;
        }
        else if(
            (cursor->ir_type == IR_ARG ||
            cursor->ir_type == IR_WRITE ||
            cursor->ir_type == IR_RETURN) &&
            (STREQ(cursor->params[0], var)) &&
            if_changed == 1
        ){
            rst = 0;
            break;
        }
        cursor = cursor->next;
    }
    return rst;
}

void replace_dead(IrStmt* irlist, char* var, char* nvar){
    IrStmt* cursor = irlist;
    while(cursor != NULL){
        if(
            (cursor->ir_type == IR_C_ASSIGNOP ||
            cursor->ir_type == IR_VP_ASSIGNOP ||
            cursor->ir_type == IR_VF_ASSIGNOP ||
            cursor->ir_type == IR_VA_ASSIGNOP) &&
            STREQ(cursor->params[1], var)
        ){
            cursor->params[1] = nvar;
        }
        else if(
            (cursor->ir_type == IR_PLUS ||
            cursor->ir_type == IR_MINUS ||
            cursor->ir_type == IR_STAR ||
            cursor->ir_type == IR_DIV) &&
            (STREQ(cursor->params[1], var) || STREQ(cursor->params[2], var))
        ){
            if(STREQ(cursor->params[1], var)) cursor->params[1] = nvar;
            if(STREQ(cursor->params[2], var)) cursor->params[2] = nvar;
        }
        else if(
            (cursor->ir_type == IR_IF_GOTO) &&
            (STREQ(cursor->params[0], var) || STREQ(cursor->params[2], var))
        ){
            if(STREQ(cursor->params[0], var)) cursor->params[0] = nvar;
            if(STREQ(cursor->params[2], var)) cursor->params[2] = nvar;
        }
        else if(
            (cursor->ir_type == IR_PV_ASSIGNOP) &&
            (STREQ(cursor->params[0], var) || STREQ(cursor->params[1], var))
        ){
            if(STREQ(cursor->params[0], var)) cursor->params[0] = nvar;
            if(STREQ(cursor->params[1], var)) cursor->params[1] = nvar;
        }
        else if(
            (cursor->ir_type == IR_ARG ||
            cursor->ir_type == IR_WRITE ||
            cursor->ir_type == IR_RETURN) &&
            (STREQ(cursor->params[0], var))
        ){
            cursor->params[0] = nvar;
        }
        cursor = cursor->next;
    }
}

typedef struct NotDead{
    char* name;
    struct NotDead* next;
}NotDead;

struct NotDead* nodead_head = NULL;
struct NotDead* nodead_tail = NULL;

NotDead* if_nodead(char* name){
    NotDead* cursor = nodead_head;
    while(cursor != NULL){
        if(STREQ(cursor->name, name)) return cursor;
        cursor = cursor->next;
    }
    return NULL;
}

void add_nodead(char* name){
    NotDead* p = (NotDead*)malloc(sizeof(NotDead));
    p->name = name;
    if(nodead_tail == NULL){
        nodead_head = p;
        nodead_tail = p;
    }
    else{
        nodead_tail->next = p;
        nodead_tail = p;
    }
}

void ir_opt4_dead(){
    /*中间代码优化-死亡变量消除*/
    IrStmt* cursor = irlist_head;
    while(cursor->next != NULL){
        IrStmt* checker = cursor->next;
        if(if_nodead(checker->params[0]) != NULL);
        else if(checker->ir_type == IR_C_ASSIGNOP && if_dead_var(checker->next, checker->params[0]) && checker->params[1][0] != '#'){
            if(if_can_replace(checker->next, checker->params[0], checker->params[1])){
                replace_dead(checker->next, checker->params[0], checker->params[1]);
                cursor->next = checker->next;
            }
        }
        else if(checker->ir_type == IR_C_ASSIGNOP && !if_dead_var(checker->next, checker->params[0]) && checker->params[1][0] != '#'){
            add_nodead(checker->params[0]);
        }
        cursor = cursor->next;
    }
}

void ir_optimization(){
    ir_opt1_immx2();
    ir_opt2_repeat_assign();
    ir_opt3_zero();
    // ir_opt4_dead();
}

void ir_gen(FILE* ir_file, Node* root){
    /*主函数-中间代码生成*/
    init_read_and_write(table);
    program_ir_anls(root);
#ifdef IR_OPTIMIZATION
    ir_optimization();
#endif
    fprintf_ir(ir_file, irlist_head);
}


