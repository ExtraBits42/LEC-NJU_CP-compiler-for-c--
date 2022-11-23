#include"../general.h"
#include"../utils/utils.h"

#include"../data_struct/spt.h"
#include"../data_struct/type_node.h"
#include"../data_struct/sym_node.h"
#include"../data_struct/sym_table.h"

#include"../data_struct/ir_list.h"

#include"ir_gen.h"
#include"ir_spt_anls.h"

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

void ir_gen(FILE* ir_file, Node* root){
    /*主函数-中间代码生成*/
    // init_read_and_write(table);
    program_ir_anls(root);
    fprintf_ir(ir_file, irlist_head);
}


