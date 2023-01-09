#pragma once
#include"../data_struct/type_node.h"

typedef struct IrNode{
    enum TypeType ntype;
    char* name;
    int if_addr;    // 1-地址；0-值
    union{
        SymNode* struct_def;
        TypeNode* array_def;
    }u;
}IrNode;

/*
Program : ExtDefList;*/
void program_ir_anls(Node* program);

/*
ExtDefList : ExtDef ExtDefList | epsilon;*/
void extdeflist_ir_anls(Node* extdeflist);

/*
ExtDef->Specifier SEMI              [x]
    | Specifier ExtDecList SEMI     [x]
    | Specifier FunDec CompSt       [v]*/
void extdef_ir_anls(Node* extdef);

/*
FunDec : ID LP VarList RP | ID LP RP;*/
void translate_fundec(Node* fundec);

/*
CompSt : LC DefList StmtList RC;*/
void translate_compst(Node* compst);

/*
DefList->Def DefList | epsilon;*/
void deflist_ir_anls(Node* deflist);

/*
Def->Specifier DecList SEMI;*/
void def_ir_anls(Node* def);

/*
DecList->Dec | Dec COMMA DecList;*/
void declist_ir_anls(Node* declist);

/*
Dec->VarDec | VarDec ASSIGNOP Exp*/
void translate_dec(Node* dec);

/*
StmtList : Stmt StmtList | epsilon;*/
void stmtlist_ir_anls(Node* stmtlist);

/*
Stmt : Exp SEMI
    | CompSt
    | RETURN Exp SEMI
    | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE
    | IF LP Exp RP Stmt ELSE Stmt
    | WHILE LP Exp RP Stmt;*/
void translate_stmt(Node* stmt);


/*
Exp : ID
    | INT
    | FLOAT
    | MINUS Exp
    | NOT Exp
    | Exp AND Exp
    | Exp OR Exp
    | Exp RELOP Exp
    | Exp ASSIGNOP Exp
    | Exp PLUS Exp
    | Exp MINUS Exp
    | Exp STAR Exp
    | Exp DIV Exp
    | LP Exp RP
    | Exp DOT ID
    | ID LP RP
    | ID LP Args RP
    | Exp LB Exp RB;*/
IrNode* translate_exp(Node* exp);
void translate_cond(Node* exp, char* label_true, char* label_false);

/*
Args : Exp COMMA Args
    | Exp;*/
void translate_args(Node* args);

