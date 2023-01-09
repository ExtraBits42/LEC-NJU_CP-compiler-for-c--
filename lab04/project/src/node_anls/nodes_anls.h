#pragma once

#include"../data_struct/spt.h"

/*
Program : ExtDefList;*/
void program_anls(Node* program);

/*
ExtDefList : ExtDef ExtDefList | epsilon;*/
void extdeflist_anls(Node* extdeflist);

/*
ExtDef : Specifier ExtDecList SEMI
    | Specifier SEMI
    | Specifier FunDec CompSt;*/
void extdef_anls(Node* extdef);

/*
ExtDecList : VarDec | VarDec COMMA ExtDecList;*/
void extdeclist_anls(TypeNode* type, Node* extdeclist);

/*
Specifier : TYPE | StructSpecifier;*/
TypeNode* specifier_anls(Node* specifier);

/*
StructSpecifier : STRUCT OptTag LC DefList RC
    | STRUCT Tag;*/
TypeNode* structspecifier_anls(Node* structspecifier);

/*
OptTag : ID | epsilon;*/
char* opttag_anls(Node* opttag);

/*
Tag : ID;*/
char* tag_anls(Node* tag);

/*
VarDec : ID | VarDec LB INT RB;*/
SymNode* vardec_anls(TypeNode* type, Node* vardec, int if_mnt);

/*
FunDec : ID LP VarList RP | ID LP RP;*/
void fundec_anls(TypeNode* type, Node* fundec);

/*
VarList : ParamDec COMMA VarList
    | ParamDec;*/
void varlist_anls(Node* varlist, SymNode** params, int* params_num);


/*
ParamDec : Specifier VarDec;*/
SymNode* paramdec_anls(Node* paramdec);

/*
CompSt : LC DefList StmtList RC;*/
void compst_anls(Node* compst);


/*
StmtList : Stmt StmtList | epsilon;*/
void stmtlist_anls(Node* stmtlist);

/*
Stmt : Exp SEMI
    | CompSt
    | RETURN Exp SEMI
    | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE
    | IF LP Exp RP Stmt ELSE Stmt
    | WHILE LP Exp RP Stmt;*/
void stmt_anls(Node* stmt);

/*
DefList : Def DefList | epsilon;*/
void deflist_anls(Node* deflist, SymNode** sym_list, int* sym_num, int if_mnt);

/*
Def : Specifier DecList SEMI;*/
void def_anls(Node* def, SymNode** sym_list, int* sym_num, int if_mnt);

/*
DecList : Dec | Dec COMMA DecList;*/
void declist_anls(TypeNode* type, Node* declist, SymNode** sym_list, int* sym_num, int if_mnt);

/*
Dec : VarDec | VarDec ASSIGNOP Exp;*/
SymNode* dec_anls(TypeNode* type, Node* dec, int if_mnt);

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
    | Exp LB Exp RB
    ;*/
void exp_anls(Node* exp);

/*
Args : Exp COMMA Args
    | Exp
    ;
*/
void args_anls(Node* args);