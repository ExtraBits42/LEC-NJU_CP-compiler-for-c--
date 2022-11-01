#include"stmt_check.h"
#include"../utils/utils.h"

#include"../data_struct/type_node.h"
#include"../data_struct/sym_node.h"
#include"../data_struct/sym_table.h"

#include"vdec_check.h"
#include"sdec_check.h"
#include"fdec_check.h"

extern SymNode* cur_func;
extern SymHashStk* sym_table;
extern SymHashStk* dec_table;

void stmt_compst_anls(Node* compst, int* pass){
    /*顶层函数-分析节点CompSt，检查复合语句*/
    /**
     * CompSt->LC DefList StmtList RC
     * **/
    stack_push(sym_table);          //【 复合语句-新建作用域符号表
    Node* deflist = compst->children->next;
    Node* stmtlist = compst->children->next->next;
    // Step01.分析复合语句中定义部分
    sdecs_deflist_anls(deflist, pass, 0);
    // Step02.分析复合语句中语句部分
    stmt_stmtlist_anls(stmtlist, pass);
    stack_pop(sym_table);          // 】复合语句-销毁作用域符号表
}

void stmt_stmtlist_anls(Node* stmtlist, int* pass){
    /*分析节点StmtList,获取Stmt语句*/
    /**
     * StmtList->Stmt StmtList | epsilon
     * **/
    Node* cursor = stmtlist;
    while(1){
        if(cursor->children == NULL) break;
        Node* stmt = cursor->children;
        stmt_stmt_anls(stmt, pass);
        cursor = cursor->children->next;
    }
}

void stmt_stmt_anls(Node* stmt, int* pass){
    /*分析节点Stmt*/
    /**
     * Stmt-> CompSt
     * | Exp SEMI
     * | RETURN Exp SEMI
     * | WHILE LP Exp Rp Stmt
     * | IF LP Exp RP Stmt
     * | IF LP Exp RP Stmt ELSE Stmt
     * **/

    // Stmt->CompSt
    if(STREQ(stmt->children->name, "CompSt")){
        Node* compst = stmt->children;
        stmt_compst_anls(compst, pass);
    }
    // Stmt->Exp SEMI
    else if(STREQ(stmt->children->name, "Exp")){
        Node* exp = stmt->children;
        stmt_exp_anls(exp, pass);
    }
    // Stmt->RETURN Exp SEMI
    else if(STREQ(stmt->children->name, "RETURN")){
        Node* exp = stmt->children->next;
        TypeNode* return_type = stmt_exp_anls(exp, pass);
        if(!equal_type_nodes(cur_func->FuncInfo.func_return_type, return_type, pass)){
            LOG_SEMANTIC_ANLS_ERROR(8, exp->u.syn_line, "return value type not match function definition here.");
        }
    }
    // Stmt->WHILE LP Exp Rp Stmt
    else if(STREQ(stmt->children->name, "WHILE")){
        Node* exp = stmt->children->next->next;
        Node* stmt1 = exp->next->next;
        stmt_exp_anls(exp, pass);
        stmt_stmt_anls(stmt1, pass);
    }
    else{
        // Stmt->IF LP Exp RP Stmt
        if(stmt->children->next->next->next->next->next == NULL){
            Node* exp = stmt->children->next->next;
            Node* stmt1 = exp->next->next;
            stmt_exp_anls(exp, pass);
            stmt_stmt_anls(stmt1, pass);
        }
        // Stmt->IF LP Exp RP Stmt ELSE Stmt
        else{
            Node* exp = stmt->children->next->next;
            Node* stmt1 = exp->next->next;
            Node* stmt2 = stmt1->next->next;
            stmt_exp_anls(exp, pass);
            stmt_stmt_anls(stmt1, pass);
            stmt_stmt_anls(stmt2, pass);
        }
    }
}

TypeNode* stmt_exp_anls(Node* exp, int*pass){
    /*分析Exp节点*/
    // BASIS
    /**
     * Exp->ID
     * | INT
     * | FLOAT
     * | ID LP Args RP
     * | ID LP RP
     * **/
    if(STREQ(exp->children->name, "ID") || STREQ(exp->children->name, "INT10") ||
       STREQ(exp->children->name, "FLOAT")){
        if(STREQ(exp->children->name, "INT10")){// Exp->INT
            return new_constant_type_node(BASIC_TYPE_INT);
        }
        else if(STREQ(exp->children->name, "FLOAT")){// Exp->FLOAT
            return new_constant_type_node(BASIC_TYPE_FLOAT);
        }
        else{
            return stmt_exp_id_call_anls(exp, pass);
        }
    }
    // Induction
    /**
     * Exp->MINUS Exp
     * | NOT Exp
     * | Exp ASSIGNOP Exp
     * | Exp DOT ID
     * | Exp AND Exp
     * | Exp OR Exp
     * | Exp RELOP Exp
     * | Exp PLUS Exp
     * | Exp MINUS Exp
     * | Exp STAR Exp
     * | Exp DIV Exp
     * | LP Exp RP
     * | Exp LB Exp RB
     * **/
    else{
        if(STREQ(exp->children->name, "MINUS")){// Exp->MINUS Exp
            Node* exp1 = exp->children->next;
            TypeNode* t = stmt_exp_anls(exp1, pass);
            if(t == NULL ||
               !(STREQ(t->type_name, BASIC_TYPE_INT) || STREQ(t->type_name, BASIC_TYPE_FLOAT)))
            {
                LOG_SEMANTIC_ANLS_ERROR(7, exp->u.syn_line, "Operation type not match another operation or operator");
                return NULL;
            }
            else if(STREQ(t->type_name, BASIC_TYPE_INT)) return new_constant_type_node(BASIC_TYPE_INT);
            else if(STREQ(t->type_name, BASIC_TYPE_FLOAT)) return new_constant_type_node(BASIC_TYPE_FLOAT);
        }
        else if(STREQ(exp->children->name, "NOT")){// Exp->NOT Exp
            Node* exp1 = exp->children->next;
            TypeNode* t = stmt_exp_anls(exp1, pass);
            if(t == NULL || !(STREQ(t->type_name, BASIC_TYPE_INT))){
                LOG_SEMANTIC_ANLS_ERROR(7, exp->u.syn_line, "Operation type not match another operation or operator");
                return NULL;
            }
            return new_constant_type_node(BASIC_TYPE_INT);
        }
        else if(STREQ(exp->children->next->name, "ASSIGNOP")){// Exp->Exp ASSIGNOP Exp
            Node* exp1 = exp->children;
            Node* exp2 = exp->children->next->next;
            TypeNode* t1 = stmt_exp_anls(exp1, pass);
            TypeNode* t2 = stmt_exp_anls(exp2, pass);
            if(t1 != NULL && t1->type == CONSTANT){
                LOG_SEMANTIC_ANLS_ERROR(6, exp->u.syn_line, "constant in right of assign operator.");
            }
            if(t1 == NULL || t2 == NULL || !equal_type_nodes(t1, t2, pass)){
                LOG_SEMANTIC_ANLS_ERROR(5, exp->u.syn_line, "Types of two operations are not matched.");
            }
            return NULL;
        }
        else if(STREQ(exp->children->next->name, "DOT")){// Exp->Exp DOT ID
            Node* exp1 = exp->children;
            Node* mem = exp->children->next->next;
            TypeNode* t1 = stmt_exp_anls(exp1, pass);
            SymNode* cursor = NULL;
            int check_pass = 1;
            if(t1 == NULL || t1->type != STRUCTTYPE){
                LOG_SEMANTIC_ANLS_ERROR(13, exp->u.syn_line, "Use operator Dot for not struct type.");
                check_pass = 0;
            }
            else if(t1 != NULL && t1->type == STRUCTTYPE){
                char* mem_name = mem->u.lex_val;
                SymNode* svar = t1->struct_type;
                cursor = svar->StructInfo.field_next;
                while(cursor != NULL){
                    if(STREQ(cursor->name, mem_name)) break;
                    cursor = cursor->StructInfo.field_next;
                }
                if(cursor == NULL){
                    LOG_SEMANTIC_ANLS_ERROR(14, exp->u.syn_line, "call the demo in strcut which not defined.");
                    check_pass = 0;
                }
            }
            if(check_pass == 0) return NULL;
            else return cursor->VarInfo.var_type;
        }
        else if(STREQ(exp->children->next->name, "AND") || // Exp->Exp AND Exp
                STREQ(exp->children->next->name, "OR") // Exp->Exp OR Exp
                ){
            Node* exp1 = exp->children;
            Node* exp2 = exp->children->next->next;
            TypeNode* t1 = stmt_exp_anls(exp1, pass);
            TypeNode* t2 = stmt_exp_anls(exp2, pass);
            if(t1 == NULL || t2 == NULL || !(equal_type_nodes(t1, t2, pass) && STREQ(t1->type_name, BASIC_TYPE_INT))){
                LOG_SEMANTIC_ANLS_ERROR(7, exp->u.syn_line, "Operation type not match another operation or operator");
                return NULL;
            }
            else return new_constant_type_node(BASIC_TYPE_INT);
        }
        else if(STREQ(exp->children->next->name, "RELOP")){// Exp->Exp RELOP Exp
            Node* exp1 = exp->children;
            Node* exp2 = exp->children->next->next;
            TypeNode* t1 = stmt_exp_anls(exp1, pass);
            TypeNode* t2 = stmt_exp_anls(exp2, pass);
            if(t1 == NULL || t2 == NULL){
                LOG_SEMANTIC_ANLS_ERROR(7, exp->u.syn_line, "Operation type not match another operation or operator");
                return NULL;
            }
            else if(equal_type_nodes(t1, t2, pass)) return new_constant_type_node(BASIC_TYPE_INT);
            else return NULL;
        }
        else if(STREQ(exp->children->next->name, "PLUS") ||// Exp->Exp PLUS Exp
                STREQ(exp->children->next->name, "MINUS") ||// Exp->Exp MINUS Exp
                STREQ(exp->children->next->name, "STAR") ||// Exp->Exp STAR Exp
                STREQ(exp->children->next->name, "DIV")// Exp->Exp DIV Exp
                ){
            Node* exp1 = exp->children;
            Node* exp2 = exp->children->next->next;
            TypeNode* t1 = stmt_exp_anls(exp1, pass);
            TypeNode* t2 = stmt_exp_anls(exp2, pass);
            if(t1 == NULL || t2 == NULL){
                LOG_SEMANTIC_ANLS_ERROR(7, exp->u.syn_line, "Operation type not match another operation or operator");
                return NULL;
            }
            else if(!(equal_type_nodes(t1, t2, pass) && (STREQ(t1->type_name, BASIC_TYPE_INT) || STREQ(t1->type_name, BASIC_TYPE_FLOAT)))){
                LOG_SEMANTIC_ANLS_ERROR(7, exp->u.syn_line, "Operation type not match another operation or operator");
                return NULL;
            }
            else if(STREQ(t1->type_name, BASIC_TYPE_INT)) return new_constant_type_node(BASIC_TYPE_INT);
            else if(STREQ(t2->type_name, BASIC_TYPE_FLOAT)) return new_constant_type_node(BASIC_TYPE_FLOAT);
        }
        else if(STREQ(exp->children->name, "LP")){// Exp->LP Exp RP
            Node* exp1 = exp->children->next;
            return stmt_exp_anls(exp1, pass);
        }
        else if(STREQ(exp->children->next->name, "LB")){// Exp->Exp LB Exp RB
            Node* exp1 = exp->children;
            Node* exp2 = exp->children->next->next;
            TypeNode* t1 = stmt_exp_anls(exp1, pass);
            TypeNode* t2 = stmt_exp_anls(exp2, pass);
            int check_pass = 1;
            if(t2 == NULL || !((t2->type == BASIC || t2->type == CONSTANT) && STREQ(t2->type_name, BASIC_TYPE_INT))){
                LOG_SEMANTIC_ANLS_ERROR(12, exp->u.syn_line, "Array index must be integer type here.");
                check_pass = 0;
            }
            if(t1 == NULL || t1->type != ARRAY){
                LOG_SEMANTIC_ANLS_ERROR(10, exp->u.syn_line, "Use [] operation for not array type.");
                check_pass = 0;
            }
            if(check_pass == 1) return t1->arr_ele_type;
            else return NULL;
        }
    }
    return NULL;
}

TypeNode* stmt_exp_id_call_anls(Node* exp, int* pass){
    /*分析表达式：ID调用(变量引用 + 函数调用)*/
    Node* id_node = exp->children;
    SymNode* notation = hash_found_global(sym_table, id_node->u.lex_val);
    if(exp->children->next == NULL){// Exp->ID
        // 检查引用的ID是否存在
        if(notation == NULL){
            LOG_SEMANTIC_ANLS_ERROR(1, exp->u.syn_line, "use undefined variable here.");
            return NULL;
        }
        // 是否为变量类型
        return notation->VarInfo.var_type;
    }
    else{// Exp->ID LP RP | ID LP Args RP
        // 检查引用的ID是否存在，以及参数是否匹配
        if(notation == NULL){
            LOG_SEMANTIC_ANLS_ERROR(2, exp->u.syn_line, "call undefined function here.");
        }
        // 检查引用的ID是否对应函数
        if(notation != NULL && notation->type != FUNC){
            LOG_SEMANTIC_ANLS_ERROR(11, exp->u.syn_line, "call a variable as afunction here.");
            // LOG_SEMANTIC_ANLS_ERROR(2, exp->u.syn_line, "call undefined function here.");
        }
        // 检查参数列表是否匹配
        TypeNode* actual_params = NULL;
        if(STREQ(exp->children->next->next->name, "RP")){// Exp->ID LP RP
            actual_params = NULL;
            TypeNode* check_flag = compare_params_actual(notation, actual_params, pass);
            if(check_flag == NULL){
                LOG_SEMANTIC_ANLS_ERROR(9, exp->u.syn_line, "actual params do not match.");
            }
            else return check_flag;
        }
        else{// Exp->ID LP Args RP
            Node* args = exp->children->next->next;
            actual_params = stmt_args_anls(args, pass);
        }
        if(notation != NULL && notation->type == FUNC){
            TypeNode* check_flag = compare_params_actual(notation, actual_params, pass);
            if(check_flag == NULL){
                LOG_SEMANTIC_ANLS_ERROR(9, exp->u.syn_line, "actual params do not match.");
            }
            else return check_flag;
        }
        else return NULL;
    }
    return NULL;
}

TypeNode* stmt_args_anls(Node* args, int* pass){
    /*分析Args节点，检查形式参数表*/
    /**
     * Args->Exp COMMA Args
     * | Exp
     * **/
    TypeNode* rst = NULL;
    TypeNode* tail = NULL;
    Node* cursor = args;
    while(cursor != NULL){
        Node* exp = cursor->children;
        TypeNode* p = stmt_exp_anls(exp, pass);
        if(p == NULL){
            // TODO： 销毁rst
            return NULL;
        }
        else{
            TypeNode* tmp = clone_type_node(p);
            if(rst == NULL){
                rst = tmp;
                tail = tmp;
            }
            else{
                tail->next = tmp;
                tail = tail->next;
            }
        }
        if(cursor->children->next == NULL) cursor = NULL;
        else cursor = cursor->children->next->next;
    }
    return rst;
}


TypeNode* compare_params_actual(SymNode* notation, TypeNode* actual_params, int* pass){
    TypeNode* cursor = actual_params;
    SymNode* p = notation->FuncInfo.params_list;
    while(cursor != NULL && p != NULL){
        if(!equal_type_nodes(cursor, p->VarInfo.var_type, pass)){
            return NULL;
        }
        cursor = cursor->next;
        p = p->FuncInfo.params_list;
    }
    if(cursor == NULL && p == NULL) return notation->FuncInfo.func_return_type;
    else return NULL;
}



