#include"../general.h"
#include"../utils/utils.h"

#include"../data_struct/type_node.h"
#include"../data_struct/sym_node.h"
#include"../data_struct/sym_table.h"

#include"nodes_anls.h"
#include"semantic_checker.h"

extern int pass;
extern SymTable* table;

int annoy_num = 0;

void program_anls(Node* program){
    /*Program->ExtDefList*/
    assert(STREQ(program->name, "Program"));
    Node* extdeflist = program->children;
    extdeflist_anls(extdeflist);
}

void extdeflist_anls(Node* extdeflist){
    /*ExtDefList->ExtDef ExtDefList | epsilon*/
    assert(STREQ(extdeflist->name, "ExtDefList"));
    while(1){
        if(extdeflist->children == NULL) break;
        Node* extdef = extdeflist->children;
        extdef_anls(extdef);
        extdeflist = extdeflist->children->next;
    }
}

void extdef_anls(Node* extdef){
    /*
    ExtDef->Specifier SEMI
        | Specifier ExtDecList SEMI
        | Specifier FunDec CompSt
    */
    assert(STREQ(extdef->name, "ExtDef"));
    if(STREQ(extdef->children->next->name, "SEMI")){
        /*ExtDef->Specifier SEMI*/
        Node* specifier = extdef->children;
        specifier_anls(specifier);
    }
    else if(STREQ(extdef->children->next->next->name, "SEMI")){
        /*ExtDef->Specifier ExtDecList SEMI*/
        Node* specifier = extdef->children;
        Node* extdeclist = extdef->children->next;

        TypeNode* type = specifier_anls(specifier);
        extdeclist_anls(type, extdeclist);

    }
    else if(STREQ(extdef->children->next->next->name, "CompSt")){
        /*ExtDef->Specifier FunDec CompSt*/
        Node* specifier = extdef->children;
        Node* fundec = extdef->children->next;
        Node* compst = fundec->next;
        TypeNode* type = specifier_anls(specifier);
        fundec_anls(type, fundec);
        compst_anls(compst);
    }
    else assert(0);
}

TypeNode* specifier_anls(Node* specifier){
    /*
    Specifier->TYPE
        | StructSpecifier
    */
    assert(STREQ(specifier->name, "Specifier"));
    if(STREQ(specifier->children->name, "TYPE")){
        /*Specifier->TYPE*/
        Node* type = specifier->children;
        if(STREQ(type->u.lex_val, BASIC_TYPE_INT)){
            TypeNode* rst = new_basic_type_node(BASIC_TYPE_INT, VARIABLE);
            return rst;
        }
        else if(STREQ(type->u.lex_val, BASIC_TYPE_FLOAT)){
            TypeNode* rst = new_basic_type_node(BASIC_TYPE_INT, VARIABLE);
            return rst;
        }
        else assert(0);
    }
    else if(STREQ(specifier->children->name, "StructSpecifier")){
        /*Specifier->StructSpecifier*/
        Node* structspecifier = specifier->children;
        return structspecifier_anls(structspecifier);
    }
    else assert(0);
}

TypeNode* structspecifier_anls(Node* structspecifier){
    /*
    StructSpecifier->STRUCT Tag
        | STRUCT OptTag LC DefList RC
    */
    assert(STREQ(structspecifier->name, "StructSpecifier"));
    if(STREQ(structspecifier->children->next->name, "Tag")){
        /*StructSpecifier->STRUCT Tag*/
        Node* tag = structspecifier->children->next;
        char* name = tag_anls(tag);
        SymNode* struct_def = hash_find(name, table);
        return new_struct_type_node(struct_def, 1);
    }
    else if(STREQ(structspecifier->children->next->name, "OptTag")){
        /*StructSpecifier->STRUCT OptTag LC DefList RC*/
        Node* opttag = structspecifier->children->next;
        Node* deflist = opttag->next->next;

        char* struct_name = opttag_anls(opttag);

        // 检查错误号 : 16 - 结构体名字与前面定义过的结构体或变量的名字重复
        int checker1 = check_semantic_error_16(struct_name, opttag);

        SymNode* members = NULL;
        int mem_num = 0;
        deflist_anls(deflist, &members, &mem_num, 1);

        SymNode* new_sym = new_struct_node(struct_name, mem_num, members);
        hash_insert(new_sym, table);
        return new_struct_type_node(new_sym, 1);
    }
    else assert(0);
}

char* tag_anls(Node* tag){
    /*Tag->ID*/
    assert(STREQ(tag->name, "Tag"));
    char* rst = NULL;
    STRCP(rst, tag->children->u.lex_val);
    return rst;
}

char* opttag_anls(Node* opttag){
    /*OptTag->ID | epsilon*/
    assert(STREQ(opttag->name, "OptTag"));
    char* rst = NULL;
    if(opttag->children == NULL){
        /*OptTag->epsilon*/
        rst = new_name_xxx(annoy_num, "Annoy");
        // char* num = int2str(annoy_num);
        // char* rst = (char*)malloc(sizeof(char) * (strlen("Annoy") + strlen(num) + 1));
        // strcpy(rst, "Annoy");
        // strcat(rst, num);
        // free(num);
        annoy_num++;
    }
    else if(STREQ(opttag->children->name, "ID")){
        /*OptTag->ID*/
        Node* id = opttag->children;
        STRCP(rst, id->u.lex_val);
    }
    else assert(0);
    return rst;
}

void deflist_anls(Node* deflist, SymNode** sym_list, int* sym_num, int if_mnt){
    /*
    DefList->Def DefList
        | epsilon
    */
    assert(STREQ(deflist->name, "DefList"));
    if(if_mnt == 1){
        (*sym_list) = NULL;
        (*sym_num) = 0;
    }
    while(1){
        if(deflist->children == NULL) break;
        Node* def = deflist->children;
        if(if_mnt == 1){
            SymNode* pieces = NULL;
            int piece_len = 0;
            def_anls(def, &pieces, &piece_len, 1);
            syms_pieces_cat(sym_list, (*sym_num), &pieces, piece_len);
            (*sym_num) += piece_len;
        }
        else def_anls(def, NULL, NULL, 0);

        deflist = deflist->children->next;
    }
}

void def_anls(Node* def, SymNode** sym_list, int* sym_num, int if_mnt){
   /*
   Def->Specifier DecList SEMI
   */
    assert(STREQ(def->name, "Def"));
    Node* specifier = def->children;
    Node* declist = def->children->next;
    TypeNode* type = specifier_anls(specifier);
    declist_anls(type, declist, sym_list, sym_num, if_mnt);
}

void declist_anls(TypeNode* type, Node* declist, SymNode** sym_list, int* sym_num, int if_mnt){
    /*
    DecList->Dec
        | Dec COMMA DecList
    */
    assert(STREQ(declist->name, "DecList"));

    // 检查错误号 : 17 - 直接使用未定义过的结构体来定义变量
    int checker1 = check_semantic_error_17(type, declist);

    if(if_mnt == 1){
        while(1){
            if(declist == NULL) break;

            Node* dec = declist->children;
            SymNode* sym = dec_anls(type, dec, 1);
            syms_pieces_cat(sym_list, (*sym_num), &sym, 1);
            (*sym_num) += 1;

            if(declist->children->next == NULL) declist = NULL;
            else declist = declist->children->next->next;
        }
    }
    else{
        while(1){
            if(declist == NULL) break;

            Node* dec = declist->children;
            dec_anls(type, dec, 0);

            if(declist->children->next == NULL) declist = NULL;
            else declist = declist->children->next->next;
        }
    }
}

SymNode* dec_anls(TypeNode* type, Node* dec, int if_mnt){
    /*
    Dec->VarDec
        | VarDec ASSIGNOP Exp
    */
    assert(STREQ(dec->name, "Dec"));
    if(dec->children->next == NULL){
        /*Dec->VarDec*/
        Node* vardec = dec->children;
        return vardec_anls(type, vardec, if_mnt);
    }
    else if(STREQ(dec->children->next->next->name, "Exp")){
        /*Dec->VarDec ASSIGNOP Exp*/
        Node* vardec = dec->children;
        Node* exp = dec->children->next->next;
        vardec_anls(type, vardec, if_mnt);
        //TODO
    }
    else assert(0);
}

SymNode* vardec_anls(TypeNode* type, Node* vardec, int if_mnt){
    /*
    VarDec->ID
        | VarDec LB INT RB
    */
   assert(STREQ(vardec->name, "VarDec"));
    TypeNode* var_type = clone_type_node(type);
    while(vardec->children->next != NULL){
        Node* dimension = vardec->children->next->next;
        int arr_size = atoi(dimension->u.lex_val);
        var_type = new_array_type_node(var_type, arr_size, VARIABLE);
        vardec = vardec->children;
    }
    Node* id = vardec->children;

    // 检查错误号 : 3 - 变量重复定义OR变量与已定义的结构体名字重复
    int checker1 = check_semantic_error_3(id);
    if(checker1 == 0) return NULL;

    SymNode* new_sym = new_var_sym_node(id->u.lex_val, var_type);
    hash_insert(new_sym, table);

    if(if_mnt == 0) return NULL;
    else return clone_sym_node(new_sym);
}

void extdeclist_anls(TypeNode* type, Node* extdeclist){
    /*
    ExtDecList->VarDec
        | VarDec COMMA ExtDecList
    */
    assert(STREQ(extdeclist->name, "ExtDecList"));

    // 检查错误号 : 17 - 直接使用未定义过的结构体来定义变量
    int checker1 = check_semantic_error_17(type, extdeclist);

    while(extdeclist != NULL){
        Node* vardec = extdeclist->children;
        vardec_anls(type, vardec, 0);
        if(extdeclist->children->next == NULL) extdeclist = NULL;
        else extdeclist = extdeclist->children->next->next;
    }
}

void fundec_anls(TypeNode* type, Node* fundec){
    /*
    FunDec->ID LP VarList RP
        | ID LP RP
    */
    assert(STREQ(fundec->name, "FunDec"));
    type->if_const = CONSTANT;
    Node* id = fundec->children;
    
    // 检查错误号 : 4 - 函数出现重复定义
    int checker1 = check_semantic_error_4(id, fundec);

    if(STREQ(fundec->children->next->next->name, "RP")){
        /*FunDec->ID LP RP*/
        
        SymNode* new_sym = new_func_node(id->u.lex_val, type, 0, NULL);
        hash_insert(new_sym, table);
    }
    else if(STREQ(fundec->children->next->next->name, "VarList")){
        /*FunDec->ID LP VarList RP*/
        Node* varlist = fundec->children->next->next;

        SymNode* params = NULL;
        int params_num = 0;
        varlist_anls(varlist, &params, &params_num);

        SymNode* new_sym = new_func_node(id->u.lex_val, type, params_num, params);
        hash_insert(new_sym, table);
    }
    else assert(0);
}

void varlist_anls(Node* varlist, SymNode** params, int* params_num){
    /*
    VarList->ParamDec COMMA VarList
        | ParamDec
    */
    while(1){
        if(varlist == NULL) break;
        Node* paramdec = varlist->children;
        SymNode* param = paramdec_anls(paramdec);
        syms_pieces_cat(params, (*params_num), &param, 1);
        (*params_num) += 1;

        if(varlist->children->next == NULL) varlist = NULL;
        else varlist = varlist->children->next->next;
    }
}

SymNode* paramdec_anls(Node* paramdec){
    /*ParamDec->Specifier VarDec*/
    assert(STREQ(paramdec->name, "ParamDec"));
    Node* specifier = paramdec->children;
    Node* vardec = paramdec->children->next;
    TypeNode* type = specifier_anls(specifier);
    return vardec_anls(type, vardec, 1);
}

void compst_anls(Node* compst){
    /*CompSt->LC DefList StmtList RC*/
    assert(STREQ(compst->name, "CompSt"));
    Node* deflist = compst->children->next;
    Node* stmtlist = compst->children->next->next;
    deflist_anls(deflist, NULL, NULL, 0);
    stmtlist_anls(stmtlist);
}

void stmtlist_anls(Node* stmtlist){
    /*StmtList->Stmt StmtList | epsilon*/
    assert(STREQ(stmtlist->name, "StmtList"));
    while(1){
        if(stmtlist->children == NULL) break;
        Node* stmt = stmtlist->children;
        stmt_anls(stmt);
        stmtlist = stmtlist->children->next;
    }
}

void stmt_anls(Node* stmt){
    /*
    Stmt->Exp SEMI
        | CompSt
        | RETURN Exp SEMI
        | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE
        | IF LP Exp RP Stmt ELSE Stmt
        | WHILE LP Exp RP Stmt
    */
    assert(STREQ(stmt->name, "Stmt"));
    if(STREQ(stmt->children->name, "CompSt")){
        /*Stmt->CompSt*/
        Node* compst = stmt->children;
        compst_anls(compst);
    }
    else if(STREQ(stmt->children->name, "Exp")){
        /*Stmt->Exp SEMI*/
        Node* exp = stmt->children;
        exp_anls(exp);
    }
    else if(STREQ(stmt->children->name, "WHILE")){
        /*Stmt->WHILE LP Exp RP Stmt*/
        Node* exp = stmt->children->next->next;
        Node* stmt1 = exp->next->next;
        exp_anls(exp);
        stmt_anls(stmt1);
    }
    else if(STREQ(stmt->children->name, "RETURN")){
        /*Stmt->RETURN Exp SEMI*/
        Node* exp = stmt->children->next;
        exp_anls(exp);
    }
    else{
        Node* stmt1 = stmt->children->next->next->next->next;
        if(stmt1->next == NULL){
            /*Stmt->IF LP Exp RP Stmt*/
            Node* exp = stmt->children->next->next;
            Node* stmt1 = exp->next->next;
            exp_anls(exp);
            stmt_anls(stmt1);
        }
        else if(STREQ(stmt1->next->name, "ELSE")){
            /*Stmt->IF LP Exp RP Stmt ELSE Stmt*/
            Node* exp = stmt->children->next->next;
            Node* stmt1 = exp->next->next;
            Node* stmt2 = stmt1->next->next;
            exp_anls(exp);
            stmt_anls(stmt1);
            stmt_anls(stmt2);
        }
        else assert(0);
    }
}

TypeNode* exp_anls(Node* exp){
    /*
    Exp : ID
        | INT
        | FLOAT
        | MINUS Exp
        | NOT Exp
        | Exp ASSIGNOP Exp
        | Exp AND Exp
        | Exp OR Exp
        | Exp RELOP Exp
        | Exp PLUS Exp
        | Exp MINUS Exp
        | Exp STAR Exp
        | Exp DIV Exp
        | LP Exp RP
        | Exp DOT ID
        | ID LP RP
        | ID LP Args RP
        | Exp LB Exp RB;*/
    assert(STREQ(exp->name, "Exp"));
    if(STREQ(exp->children->name, "ID")){
        //Exp->ID
        Node* id = exp->children;

        // 检查错误号 : 1 - 变量在使用时未经定义
        int checker1 = check_semantic_error_1(id);
        if(checker1 == 0) return NULL;
        else{
            SymNode* found = hash_find(id->u.lex_val, table);
            if(found->ntype == VAR_SYM) return found->var_v.vtype;
            else if(found->ntype == FUNC_SYM){
                TypeNode* rst = clone_type_node(found->func_v.rtype);
                rst->if_const = CONSTANT;
                return rst;
            }
            else if(found->ntype == STRUCT_SYM){
                TypeNode* rst = new_struct_type_node(found, VARIABLE);
                return rst;
            }
        }
    }
    else if(STREQ(exp->children->name, "INT")){
        //Exp->INT
        Node* _int = exp->children;
        return new_basic_type_node(BASIC_TYPE_INT, CONSTANT);
    }
    else if(STREQ(exp->children->name, "FLOAT")){
        //Exp->FLOAT
        Node* _float = exp->children;
        return new_basic_type_node(BASIC_TYPE_FLOAT, CONSTANT);
    }
    else if(STREQ(exp->children->name, "MINUS")){
        //Exp->MINUS Exp
        Node* exp1 = exp->children->next;
        exp_anls(exp1);
    }
    else if(STREQ(exp->children->name, "NOT")){
        //Exp->NOT Exp
        Node* exp1 = exp->children->next;
        exp_anls(exp1);
    }
    else if(STREQ(exp->children->next->name, "ASSIGNOP")){
        //Exp->Exp ASSIGNOP Exp
        Node* exp1 = exp->children;
        Node* exp2 = exp->children->next->next;
        exp_anls(exp1);
        exp_anls(exp2);
    }
    else if(STREQ(exp->children->next->name, "AND")){
        //Exp->Exp AND Exp
        Node* exp1 = exp->children;
        Node* exp2 = exp->children->next->next;
        exp_anls(exp1);
        exp_anls(exp2);
    }
    else if(STREQ(exp->children->next->name, "OR")){
        //Exp->Exp OR Exp
        Node* exp1 = exp->children;
        Node* exp2 = exp->children->next->next;
        exp_anls(exp1);
        exp_anls(exp2);
    }
    else if(STREQ(exp->children->next->name, "RELOP")){
        //Exp->Exp RELOP Exp
        Node* exp1 = exp->children;
        Node* exp2 = exp->children->next->next;
        exp_anls(exp1);
        exp_anls(exp2);
    }
    else if(STREQ(exp->children->next->name, "PLUS")){
        // Exp->Exp PLUS Exp
        Node* exp1 = exp->children;
        Node* exp2 = exp->children->next->next;
        exp_anls(exp1);
        exp_anls(exp2);
    }
    else if(STREQ(exp->children->next->name, "MINUS")){
        // Exp->Exp MINUS Exp
        Node* exp1 = exp->children;
        Node* exp2 = exp->children->next->next;
        exp_anls(exp1);
        exp_anls(exp2);
    }
    else if(STREQ(exp->children->next->name, "STAR")){
        // Exp->Exp STAR Exp
        Node* exp1 = exp->children;
        Node* exp2 = exp->children->next->next;
        exp_anls(exp1);
        exp_anls(exp2);
    }
    else if(STREQ(exp->children->next->name, "DIV")){
        // Exp->Exp DIV Exp
        Node* exp1 = exp->children;
        Node* exp2 = exp->children->next->next;
        exp_anls(exp1);
        exp_anls(exp2);
    }
    else if(STREQ(exp->children->next->name, "Exp")){
        // Exp->LP Exp RP
        Node* exp1 = exp->children->next;
        exp_anls(exp1);
    }
    else if(STREQ(exp->children->next->name, "DOT")){
        // Exp->Exp DOT ID
        Node* exp1 = exp->children;
        exp_anls(exp1);
    }
    else if(STREQ(exp->children->next->next->name, "RP")){
        // Exp->ID LP RP
        Node* id = exp->children;
    }
    else if(STREQ(exp->children->next->next->name, "Args")){
        // Exp->ID LP Args RP
        Node* id = exp->children;
        Node* args = exp->children->next->next;
    }
    else if(STREQ(exp->children->next->next->name, "Exp")){
        // Exp->Exp LB Exp RB
        Node* exp1 = exp->children;
        Node* exp2 = exp->children->next->next;
        exp_anls(exp1);
        exp_anls(exp2);
    }
    else assert(0);
}


void args_anls(Node* args){
    /*Args->Exp COMMA Args | Exp;*/
    assert(STREQ(args->name, "Args"));
    if(args->children->next == NULL){
        /*Args->Exp*/
        Node* exp1 = args->children;
        exp_anls(exp1);
    }
    else if(STREQ(args->children->next->name, "COMMA")){
        /*Args->Exp COMMA Args*/
        while(1){
            if(args == NULL) break;
            Node* exp = args->children;
            exp_anls(exp);
            if(args->children->next == NULL) args = NULL;
            else args = args->children->next->next;
        }
    }
    else assert(0);
}