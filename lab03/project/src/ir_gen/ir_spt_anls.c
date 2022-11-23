#include"../general.h"
#include"../utils/utils.h"

#include"../data_struct/type_node.h"
#include"../data_struct/sym_node.h"
#include"../data_struct/sym_table.h"

#include"../data_struct/ir_list.h"

#include"ir_spt_anls.h"

extern SymTable* table;

extern IrStmt* irlist_head;
extern IrStmt* irlist_tail;

static int temp_num = 0;
static int label_num = 0;

#define ADD_IR_STMT(x) add_ir_stmt(&irlist_head, &irlist_tail, (x))

char* new_temp_name(){
    /*新建中间变量名 : temp<xxx序号>*/
    char* rst = new_name_xxx(temp_num, "temp");
    temp_num += 1;
    return rst;
}

IrNode* new_irn(char* name, TypeType ntype, SymNode* struct_def, TypeNode* array_def){
    /*新建中间变量节点*/
    IrNode* p = (IrNode*)malloc(sizeof(IrNode));
    p->name = name;
    p->ntype = ntype;
    p->if_addr = 1;
    if(ntype == BASIC_T) p->if_addr = 0;
    else if(ntype == STRUCT_T) p->u.struct_def = struct_def;
    else if(ntype == ARRAY_T) p->u.array_def = array_def;
    return p;
}

char* new_label(){
    /*获取一个新的标签名: label<xxx序号>*/
    char* rst = new_name_xxx(label_num, "label");
    label_num++;
    return rst;
}

void program_ir_anls(Node* program){
    /*Program->ExtDefList*/
    assert(STREQ(program->name, "Program"));
    Node* extdeflist = program->children;
    extdeflist_ir_anls(extdeflist);
}

void extdeflist_ir_anls(Node* extdeflist){
    /*ExtDefList->ExtDef ExtDefList | epsilon*/
    assert(STREQ(extdeflist->name, "ExtDefList"));
    while(1){
        if(extdeflist->children == NULL) break;
        Node* extdef = extdeflist->children;
        extdef_ir_anls(extdef);
        extdeflist = extdeflist->children->next;
    }
}

void extdef_ir_anls(Node* extdef){
    /*
    ExtDef->Specifier SEMI              [x]
        | Specifier ExtDecList SEMI     [x]
        | Specifier FunDec CompSt       [v]
    */
    assert(STREQ(extdef->name, "ExtDef"));
    if(STREQ(extdef->children->next->name, "SEMI")){
        /*ExtDef->Specifier SEMI [x]*/
        return;
    }
    else if(STREQ(extdef->children->next->next->name, "SEMI")){
        /*ExtDef->Specifier ExtDecList SEMI [x]*/
        return;
    }
    else if(STREQ(extdef->children->next->next->name, "CompSt")){
        /*ExtDef->Specifier FunDec CompSt [v]*/
        // Node* specifier = extdef->children;
        Node* fundec = extdef->children->next;
        Node* compst = fundec->next;

        translate_fundec(fundec);
        translate_compst(compst);
    }
    else assert(0);
}

void translate_fundec(Node* fundec){
    /*
    FunDec->ID LP VarList RP
        | ID LP RP
    */
    assert(STREQ(fundec->name, "FunDec"));
    Node* id = fundec->children;
    SymNode* func = hash_find(id->u.lex_val, table);
    assert(func != NULL);

    IrStmt* p = new_ir_stmt(2, IR_FUNCTION, func->name);
    ADD_IR_STMT(p);

    for(int i = 0; i < func->func_v.param_num; i++){
        SymNode* param = &(func->func_v.params[i]);
        IrStmt* p = new_ir_stmt(2, IR_PARAM, param->name);
        ADD_IR_STMT(p);
    }
}

void translate_compst(Node* compst){
    /*CompSt->LC DefList StmtList RC*/
    assert(STREQ(compst->name, "CompSt"));
    Node* deflist = compst->children->next;
    Node* stmtlist = compst->children->next->next;

    deflist_ir_anls(deflist);
    stmtlist_ir_anls(stmtlist);
}

void deflist_ir_anls(Node* deflist){
    /*
    DefList->Def DefList | epsilon;*/
    assert(STREQ(deflist->name, "DefList"));
    while(1){
        if(deflist->children == NULL) break;

        Node* def = deflist->children;
        def_ir_anls(def);

        deflist = deflist->children->next;
    }    
}

void def_ir_anls(Node* def){
    /*
    Def->Specifier DecList SEMI;*/
    assert(STREQ(def->name, "Def"));
    // Node* specifier = def->children;
    Node* declist = def->children->next;
    declist_ir_anls(declist);
}

void declist_ir_anls(Node* declist){
    /*
    DecList->Dec | Dec COMMA DecList;*/
    assert(STREQ(declist->name, "DecList"));
    while(1){
        if(declist == NULL) break;

        Node* dec = declist->children;
        translate_dec(dec);

        if(declist->children->next == NULL) declist = NULL;
        else declist = declist->children->next->next;
    }
}

void translate_dec(Node* dec){
    /*
    Dec->VarDec | VarDec ASSIGNOP Exp*/
    assert(STREQ(dec->name, "Dec"));

    Node* found_id = dec->children;
    while(1){
        if(STREQ(found_id->name, "ID")) break;
        found_id = found_id->children;
    }
    char* id = found_id->u.lex_val;
    SymNode* var = hash_find(id, table);
    assert(var != NULL);
    assert(var->ntype == VAR_SYM);

    TypeNode* type_node = var->var_v.vtype;
    if(type_node->ntype == BASIC_T){
        assert((
            STREQ(type_node->basic_v.basic_type, BASIC_TYPE_INT) ||
            STREQ(type_node->basic_v.basic_type, BASIC_TYPE_FLOAT)
        ));
    }
    else if(type_node->ntype == STRUCT_T){
        assert(type_node->struct_v.def_ptr != NULL);
        int type_size = get_struct_size(type_node->struct_v.def_ptr);
        char* size = int2str(type_size);

        IrNode* temp = new_irn(new_temp_name(), STRUCT_T, var, NULL);
        IrStmt* p1 = new_ir_stmt(3, IR_DEC, temp->name, size);
        IrStmt* p2 = new_ir_stmt(3, IR_VA_ASSIGNOP, var->name, temp->name);

        ADD_IR_STMT(p1);
        ADD_IR_STMT(p2);
    }
    else if(type_node->ntype == ARRAY_T){
        int type_size = get_array_size(type_node);
        char* size = int2str(type_size);

        IrNode* temp = new_irn(new_temp_name(), ARRAY_T, NULL, type_node);
        IrStmt* p1 = new_ir_stmt(3, IR_DEC, temp->name, size);
        IrStmt* p2 = new_ir_stmt(3, IR_VA_ASSIGNOP, var->name, temp->name);

        ADD_IR_STMT(p1);
        ADD_IR_STMT(p2);
    }

    if(dec->children->next == NULL){
        /*Dec->VarDec*/
        return; // Do Nothing
    }
    else if(STREQ(dec->children->next->name, "ASSIGNOP")){
        /*Dec->VarDec ASSIGNOP Exp*/
        Node* exp = dec->children->next->next;
        int e1s = 0;
        IrNode* vname = translate_exp(exp);
        assert(var->var_v.vtype->ntype == BASIC_T);

        if(vname->if_addr == 1){
            IrNode* temp1 = new_irn(new_temp_name(), BASIC_T, NULL, NULL);
            IrStmt* p1 = new_ir_stmt(3, IR_VP_ASSIGNOP, temp1->name, vname->name);
            IrStmt* p2 = new_ir_stmt(3, IR_C_ASSIGNOP, var->name, temp1->name);
            ADD_IR_STMT(p1);
            ADD_IR_STMT(p2);
        }
        else{
            IrStmt* p = new_ir_stmt(3, IR_C_ASSIGNOP, var->name, vname->name);
            ADD_IR_STMT(p);
        }
    }
    else assert(0);
}

void stmtlist_ir_anls(Node* stmtlist){
    /*StmtList->Stmt StmtList | epsilon*/
    assert(STREQ(stmtlist->name, "StmtList"));
    while(1){
        if(stmtlist->children == NULL) break;
        Node* stmt = stmtlist->children;
        translate_stmt(stmt);
        stmtlist = stmtlist->children->next;
    }
}

void translate_stmt(Node* stmt){
    /*
    Stmt->CompSt
        | Exp SEMI
        | RETURN Exp SEMI
        | WHILE LP Exp RP Stmt
        | IF LP Exp RP Stmt
        | IF LP Exp RP Stmt ELSE Stmt*/
    assert(STREQ(stmt->name, "Stmt"));
    if(STREQ(stmt->children->name, "CompSt")){
        /*Stmt->CompSt*/
        Node* compst = stmt->children;
        translate_compst(compst);
    }
    else if(STREQ(stmt->children->name, "Exp")){
        /*Stmt->Exp SEMI*/
        Node* exp = stmt->children;
        translate_exp(exp);
    }
    else if(STREQ(stmt->children->name, "RETURN")){
        /*Stmt->RETURN Exp SEMI*/
        Node* exp = stmt->children->next;
        IrNode* temp1 = translate_exp(exp);

        IrStmt* p = new_ir_stmt(2, IR_RETURN, temp1->name);
        add_ir_stmt(&irlist_head, &irlist_tail, p);
    }
    else if(STREQ(stmt->children->name, "WHILE")){
        /*Stmt->WHILE LP Exp RP Stmt*/
        Node* exp = stmt->children->next->next;
        Node* stmt1 = exp->next->next;
        char* label1 = new_label();
        char* label2 = new_label();
        char* label3 = new_label();

        IrStmt* p1 = new_ir_stmt(2, IR_LABEL, label1);
        ADD_IR_STMT(p1);

        translate_cond(exp, label2, label3);

        IrStmt* p2 = new_ir_stmt(2, IR_LABEL, label2);
        ADD_IR_STMT(p2);

        translate_stmt(stmt1);

        IrStmt* p3 = new_ir_stmt(2, IR_GOTO, label1);
        ADD_IR_STMT(p3);

        IrStmt* p4 = new_ir_stmt(2, IR_LABEL, label3);
        ADD_IR_STMT(p4);
    }
    else if(stmt->children->next->next->next->next->next == NULL){
        /*Stmt->IF LP Exp RP Stmt*/
        Node* exp = stmt->children->next->next;
        Node* stmt1 = exp->next->next;
        char* label1 = new_label();
        char* label2 = new_label();

        translate_cond(exp, label1, label2);

        IrStmt* p1 = new_ir_stmt(2, IR_LABEL, label1);
        ADD_IR_STMT(p1);

        translate_stmt(stmt1);

        IrStmt* p2 = new_ir_stmt(2, IR_LABEL, label2);
        ADD_IR_STMT(p2);
    }
    else if(STREQ(stmt->children->name, "IF")){
        /*Stmt->IF LP Exp RP Stmt ELSE Stmt*/
        Node* exp = stmt->children->next->next;
        Node* stmt1 = exp->next->next;
        Node* stmt2 = stmt1->next->next;
        char* label1 = new_label();
        char* label2 = new_label();
        char* label3 = new_label();

        translate_cond(exp, label1, label2);

        IrStmt* p1 = new_ir_stmt(2, IR_LABEL, label1);
        ADD_IR_STMT(p1);

        translate_stmt(stmt1);

        IrStmt* p2 = new_ir_stmt(2, IR_GOTO, label3);
        ADD_IR_STMT(p2);

        IrStmt* p3 = new_ir_stmt(2, IR_LABEL, label2);
        ADD_IR_STMT(p3);

        translate_stmt(stmt2);

        IrStmt* p4 = new_ir_stmt(2, IR_LABEL, label3);
        ADD_IR_STMT(p4);
    }
    else assert(0);
}

IrNode* translate_exp(Node* exp){
    if(STREQ(exp->name, "Exp"));
    else{
        printf("%s %s\n", exp->name, exp->u.lex_val);
        assert(STREQ(exp->name, "Exp"));
    }
    
    if(exp->children->next == NULL && STREQ(exp->children->name, "ID")){
        /*Exp->ID*/
        Node* id_node = exp->children;
        char* id = id_node->u.lex_val;
        SymNode* found = hash_find(id, table);
        assert(found != NULL);
        assert(found->ntype == VAR_SYM);

        TypeNode* type = found->var_v.vtype;

        IrNode* ir_id = new_irn(found->name, type->ntype, type->struct_v.def_ptr, type);
        return ir_id;
    }
    else if(STREQ(exp->children->name, "INT")){
        /*Exp->INT*/
        Node* int_node = exp->children;
        char* str_val = (char*)malloc(sizeof(char) * (strlen(int_node->u.lex_val) + 2));
        strcpy(str_val, "#");
        strcat(str_val, int_node->u.lex_val);

        IrNode* ir_int = new_irn(str_val, BASIC_T, NULL, NULL);

        return ir_int;
    }
    else if(STREQ(exp->children->name, "FLOAT")){
        /*Exp->FLOAT*/
        assert(0);  // 不会出现浮点型常数
    }
    else if(STREQ(exp->children->name, "MINUS")){
        /*Exp->MINUS Exp*/
        Node* exp1 = exp->children->next;
        IrNode* ir_n1 = translate_exp(exp1);

        IrNode* rst = new_irn(new_temp_name(), BASIC_T, NULL, NULL);

        if(ir_n1->if_addr == 0){
            IrStmt* p = new_ir_stmt(4, IR_MINUS, rst->name, "#0", ir_n1->name);
            ADD_IR_STMT(p);
        }
        else{
            IrStmt* p1 = new_ir_stmt(3, IR_VP_ASSIGNOP, rst->name, ir_n1->name);
            IrStmt* p2 = new_ir_stmt(4, IR_MINUS, rst->name, "#0", rst->name);
            ADD_IR_STMT(p1);
            ADD_IR_STMT(p2);
        }
        return rst;
    }
    else if(STREQ(exp->children->name, "NOT") ||
            STREQ(exp->children->next->name, "AND") ||
            STREQ(exp->children->next->name, "OR") ||
            STREQ(exp->children->next->name, "RELOP")
    ){
        /*Exp->NOT Exp | Exp AND Exp | Exp OR Exp | Exp RELOP Exp*/
        char* label1 = new_label();
        char* label2 = new_label();

        IrNode* temp1 = new_irn(new_temp_name(), BASIC_T, NULL, NULL);
        IrStmt* p1 = new_ir_stmt(3, IR_C_ASSIGNOP, temp1->name, "#0");
        ADD_IR_STMT(p1);

        translate_cond(exp, label1, label2);

        IrStmt* p2 = new_ir_stmt(2, IR_LABEL, label1);
        ADD_IR_STMT(p2);

        IrStmt* p3 = new_ir_stmt(3, IR_C_ASSIGNOP, temp1->name, "#1");
        ADD_IR_STMT(p3);

        IrStmt* p4 = new_ir_stmt(2, IR_LABEL, label2);
        ADD_IR_STMT(p4);

        return temp1;
    }
    else if(STREQ(exp->children->next->name, "ASSIGNOP")){
        /*Exp->Exp ASSIGNOP Exp*/
        Node* exp1 = exp->children;
        Node* exp2 = exp->children->next->next;
        IrNode* vname1 = translate_exp(exp1);
        IrNode* vname2 = translate_exp(exp2);

        if(vname1->if_addr == 1){
            if(vname2->if_addr == 1){
                IrNode* temp1 = new_irn(new_temp_name(), BASIC_T, NULL, NULL);
                IrStmt* p1 = new_ir_stmt(3, IR_VP_ASSIGNOP, temp1->name, vname2->name);
                IrStmt* p2 = new_ir_stmt(3, IR_PV_ASSIGNOP, vname1->name, temp1->name);
                ADD_IR_STMT(p1);
                ADD_IR_STMT(p2);
            }
            else{
                IrStmt* p1 = new_ir_stmt(3, IR_PV_ASSIGNOP, vname1->name, vname2->name);
                ADD_IR_STMT(p1);
            }
        }
        else{
            if(vname2->if_addr == 1){
                IrNode* temp1 = new_irn(new_temp_name(), BASIC_T, NULL, NULL);
                IrStmt* p1 = new_ir_stmt(3, IR_VP_ASSIGNOP, temp1->name, vname2->name);
                IrStmt* p2 = new_ir_stmt(3, IR_C_ASSIGNOP, vname1->name, temp1->name);
                ADD_IR_STMT(p1);
                ADD_IR_STMT(p2);
            }
            else{
                IrStmt* p1 = new_ir_stmt(3, IR_C_ASSIGNOP, vname1->name, vname2->name);
                ADD_IR_STMT(p1);
            }
        }      
    }
    else if(STREQ(exp->children->next->name, "PLUS") ||
            STREQ(exp->children->next->name, "MINUS") ||
            STREQ(exp->children->next->name, "STAR") ||
            STREQ(exp->children->next->name, "DIV")
    ){
        /*Exp->
            | Exp PLUS Exp
            | Exp MINUS Exp
            | Exp STAR Exp
            | Exp DIV Exp
        */
        Node* exp1 = exp->children;
        Node* exp2 = exp->children->next->next;
        Node* op = exp->children->next;
        IrNode* vname1 = translate_exp(exp1);
        IrNode* vname2 = translate_exp(exp2);
        IrNode* op1 = vname1, *op2 = vname2;
        IrNode* temp1 = NULL, *temp2 = NULL;
        if(vname1->if_addr == 1){
            temp1 = new_irn(new_temp_name(), BASIC_T, NULL, NULL);
            IrStmt* p1 = new_ir_stmt(3, IR_VP_ASSIGNOP, temp1->name, vname1->name);
            ADD_IR_STMT(p1);
            op1 = temp1;
        }
        if(vname2->if_addr == 1){
            temp2 = new_irn(new_temp_name(), BASIC_T, NULL, NULL);
            IrStmt* p1 = new_ir_stmt(3, IR_VP_ASSIGNOP, temp2->name, vname2->name);
            ADD_IR_STMT(p1);
            op2 = temp2;
        }

        IrNode* rst = new_irn(new_temp_name(), BASIC_T, NULL, NULL);
        IrStmt* p = NULL;
        if(STREQ(op->name, "PLUS")) p = new_ir_stmt(4, IR_PLUS, rst->name, op1->name, op2->name);
        else if(STREQ(op->name, "MINUS")) p = new_ir_stmt(4, IR_MINUS, rst->name, op1->name, op2->name);
        else if(STREQ(op->name, "STAR")) p = new_ir_stmt(4, IR_STAR, rst->name, op1->name, op2->name);
        else if(STREQ(op->name, "DIV")) p = new_ir_stmt(4, IR_DIV, rst->name, op1->name, op2->name);
        else assert(0);
        ADD_IR_STMT(p);

        return rst;
    }
    else if(STREQ(exp->children->name, "LP")){
        /*Exp->LP Exp RP*/
        Node* exp1 = exp->children->next;
        return translate_exp(exp1);
    }
    else if(STREQ(exp->children->next->name, "DOT")){
        /*Exp->Exp DOT ID*/
        Node* exp1 = exp->children;
        Node* id_node = exp->children->next->next;
        char* id = id_node->u.lex_val;
        SymNode* id_sym = hash_find(id, table);
        TypeNode* type = id_sym->var_v.vtype;
        assert(id_sym != NULL);

        IrNode* vname = translate_exp(exp1);

        assert(vname->ntype == STRUCT_T);
        assert(vname->u.struct_def != NULL);

        int offset = get_member_offset(vname->u.struct_def, id);
        char* str_offset = int2_imstr(offset);

        IrNode* temp1 = new_irn(new_temp_name(), type->ntype, type->struct_v.def_ptr, type);
        temp1->if_addr = 1;
        IrStmt* p1 = new_ir_stmt(4, IR_PLUS, temp1->name, vname->name, str_offset);
        ADD_IR_STMT(p1);

        return temp1;
    }
    else if(STREQ(exp->children->next->next->name, "RP")){
        /*Exp->ID LP RP*/
        Node* id = exp->children;
        char* fname = id->u.lex_val;
        IrNode* rst = new_irn(new_temp_name(), BASIC_T, NULL, NULL);
        IrStmt* p = NULL;
        if(STREQ(fname, READ_FUNC)) p = new_ir_stmt(2, IR_READ, rst->name);
        else p = new_ir_stmt(3, IR_VF_ASSIGNOP, rst->name, fname);
        ADD_IR_STMT(p);

        return rst;
    }
    else if(STREQ(exp->children->next->next->name, "Args")){
        /*Exp->ID LP Args RP*/
        Node* id = exp->children;
        Node* args = exp->children->next->next;

        char* fname = id->u.lex_val;
        IrNode* rst = new_irn(new_temp_name(), BASIC_T, NULL, NULL);
        IrStmt* p = NULL;
        if(STREQ(fname, WRITE_FUNC)){
            IrNode* vname1 = translate_exp(args->children);
            IrNode* op1 = vname1;
            IrNode* temp1 = NULL;
            if(vname1->if_addr == 1){
                temp1 = new_irn(new_temp_name(), BASIC_T, NULL, NULL);
                IrStmt* p1 = new_ir_stmt(3, IR_VP_ASSIGNOP, temp1->name, vname1->name);
                ADD_IR_STMT(p1);
                op1 = temp1;
            }
            p = new_ir_stmt(2, IR_WRITE, op1->name);
        }
        else{
            translate_args(args);
            p = new_ir_stmt(3, IR_VF_ASSIGNOP, rst->name, fname);
        }
        ADD_IR_STMT(p);
        
        return rst;
    }
    else if(STREQ(exp->children->next->name, "LB")){
        /*Exp->Exp LB Exp RB*/
        Node* exp1 = exp->children;
        Node* exp2 = exp->children->next->next;

        IrNode* arrayv = translate_exp(exp1);
        if(arrayv->ntype != ARRAY_T){
            print_tree(exp, 2);
            printf("%d %d\n", arrayv->ntype, arrayv->if_addr);
            assert(arrayv->ntype == ARRAY_T);
        }
        
        assert(arrayv->u.array_def != NULL);
        IrNode* index = translate_exp(exp2);

        int offset = 1;
        TypeNode* cursor = arrayv->u.array_def->array_v.ele_type;
        while(cursor->ntype == ARRAY_T){
            offset = offset * cursor->array_v.size;
            cursor = cursor->array_v.ele_type;
        }
        offset = offset * get_type_size(cursor);
        char* str_offset = int2_imstr(offset);

        IrNode* temp1 = new_irn(new_temp_name(), BASIC_T, NULL, NULL);
        TypeNode* ele_type = arrayv->u.array_def->array_v.ele_type;
        IrNode* temp2 = new_irn(new_temp_name(), ele_type->ntype, ele_type->struct_v.def_ptr, ele_type);
        temp2->if_addr = 1;

        IrStmt* p1 = new_ir_stmt(4, IR_STAR, temp1->name, index->name, str_offset);
        IrStmt* p2 = new_ir_stmt(4, IR_PLUS, temp1->name, arrayv->name, temp1->name);
        IrStmt* p3 = new_ir_stmt(3, IR_C_ASSIGNOP, temp2->name, temp1->name);
        ADD_IR_STMT(p1);
        ADD_IR_STMT(p2);
        ADD_IR_STMT(p3);

        return temp2;
    }
    else assert(0);
}

void translate_cond(Node* exp, char* label_true, char* label_false){
    /*
    Exp->
        | NOT Exp
        | Exp AND Exp
        | Exp OR Exp
        | Exp RELOP Exp
        | Other~~*/
    assert(STREQ(exp->name, "Exp"));
    if(STREQ(exp->children->name, "NOT")){
        /*Exp->NOT Exp*/
        Node* exp1 = exp->children->next;
        translate_cond(exp1, label_false, label_true);
    }
    else if(exp->children->next != NULL && STREQ(exp->children->next->name, "AND")){
        /*Exp->Exp AND Exp*/
        Node* exp1 = exp->children;
        Node* exp2 = exp->children->next->next;
        char* label1 = new_label();
        translate_cond(exp1, label1, label_false);

        IrStmt* p = new_ir_stmt(2, IR_LABEL, label1);
        ADD_IR_STMT(p);

        translate_cond(exp2, label_true, label_false);
    }
    else if(exp->children->next != NULL && STREQ(exp->children->next->name, "OR")){
        /*Exp->Exp OR Exp*/
        Node* exp1 = exp->children;
        Node* exp2 = exp->children->next->next;
        char* label1 = new_label();
        translate_cond(exp1, label_true, label1);

        IrStmt* p = new_ir_stmt(2, IR_LABEL, label1);
        ADD_IR_STMT(p);

        translate_cond(exp2, label_true, label_false);
    }
    else if(exp->children->next != NULL && STREQ(exp->children->next->name, "RELOP")){
        /*Exp->Exp RELOP Exp*/
        Node* exp1 = exp->children;
        Node* exp2 = exp->children->next->next;
        Node* relop_node = exp->children->next;

        char* relop = relop_node->u.lex_val;
        IrNode* vname1 = translate_exp(exp1);
        IrNode* vname2 = translate_exp(exp2);
        IrNode* op1 = vname1, *op2 = vname2;
        IrNode* temp1 = NULL, *temp2 = NULL;

        if(vname1->if_addr == 1){
            temp1 = new_irn(new_temp_name(), BASIC_T, NULL, NULL);
            IrStmt* p1 = new_ir_stmt(3, IR_VP_ASSIGNOP, temp1->name, vname1->name);
            ADD_IR_STMT(p1);
            op1 = temp1;
        }
        if(vname2->if_addr == 1){
            temp2 = new_irn(new_temp_name(), BASIC_T, NULL, NULL);
            IrStmt* p2 = new_ir_stmt(3, IR_VP_ASSIGNOP, temp2->name, vname2->name);
            ADD_IR_STMT(p2);
            op2 = temp2;
        }
        
        IrStmt* p1 = new_ir_stmt(5, IR_IF_GOTO, op1->name, relop, op2->name, label_true);
        ADD_IR_STMT(p1);

        IrStmt* p2 = new_ir_stmt(2, IR_GOTO, label_false);
        ADD_IR_STMT(p2);
    }
    else{
        /*Other~~*/
        IrNode* vname1 = translate_exp(exp);
        IrNode* op1 = vname1;
        IrNode* temp1 = NULL;
        if(vname1->if_addr == 1){
            temp1 = new_irn(new_temp_name(), BASIC_T, NULL, NULL);
            IrStmt* p1 = new_ir_stmt(3, IR_VP_ASSIGNOP, temp1->name, vname1->name);
            ADD_IR_STMT(p1);
            op1 = temp1;
        }

        IrStmt* p1 = new_ir_stmt(5, IR_IF_GOTO, op1->name, "!=", "#0", label_true);
        ADD_IR_STMT(p1);

        IrStmt* p2 = new_ir_stmt(2, IR_GOTO, label_false);
        ADD_IR_STMT(p2);
    }
}

void translate_args(Node* args){
    /*
    Args->Exp COMMA Args
        | Exp;
    */
    if(args == NULL) return;
    assert(STREQ(args->name, "Args"));
    
    Node* exp = args->children;

    if(args->children->next == NULL) args = NULL;
    else args = args->children->next->next;

    translate_args(args);

    IrNode* vname = translate_exp(exp);

    IrStmt* p = new_ir_stmt(2, IR_ARG, vname->name);
    ADD_IR_STMT(p);
}

