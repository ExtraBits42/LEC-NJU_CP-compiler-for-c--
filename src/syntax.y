%{
    #include<stdio.h>
    #include "lex.yy.c"
    #include "sat_gen.h"
    extern int pass;
    extern int yyerror(char* msg);
    extern Node* root;
    #define YYSTYPE Node*
%}

/*declared tokens*/
%token INT FLOAT
%token TYPE STRUCT RETURN IF ELSE WHILE
%token ID
%token ASSIGNOP
%token RELOP
%token PLUS MINUS STAR DIV
%token AND OR NOT
%token LP RP LB RB LC RC
%token SEMI COMMA DOT

/*declared associativity and priority*/
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT
%left LP RP LB RB DOT

%%
/*High-level Definitions*/
Program : ExtDefList                            {$$ = set_node("Program"); add_children(2, $$, $1); root = $$;}
    ;
ExtDefList : ExtDef ExtDefList                  {$$ = set_node("ExtDefList"); add_children(3, $$, $1, $2);}
    |                                           {$$ = set_node("ExtDefList");}
    ;
ExtDef : Specifier ExtDecList SEMI              {$$ = set_node("ExtDef"); add_children(4, $$, $1, $2, $3);}
    | Specifier SEMI                            {$$ = set_node("ExtDef"); add_children(3, $$, $1, $2);}
    | Specifier FunDec CompSt                   {$$ = set_node("ExtDef"); add_children(4, $$, $1, $2, $3);}
    | error SEMI
    ;
ExtDecList : VarDec                             {$$ = set_node("ExtDecList"); add_children(2, $$, $1);}
    | VarDec COMMA ExtDecList                   {$$ = set_node("ExtDecList"); add_children(4, $$, $1, $2, $3);}
    ;
/*Specifiers*/
Specifier : TYPE                                {$$ = set_node("Specifier"); add_children(2, $$, $1);}
    | StructSpecifier                           {$$ = set_node("Specifier"); add_children(2, $$, $1);}
    ;
StructSpecifier : STRUCT OptTag LC DefList RC   {$$ = set_node("StructSpecifier"); add_children(6, $$, $1, $2, $3, $4, $5);}
    | STRUCT Tag                                {$$ = set_node("StructSpecifier"); add_children(3, $$, $1, $2);}
    ;
OptTag : ID                                     {$$ = set_node("OptTag"); add_children(2, $$, $1);}
    |                                           {$$ = set_node("OptTag");}
    ;
Tag : ID                                        {$$ = set_node("Tag"); add_children(2, $$, $1);}
    ;
/*Declarators*/
VarDec : ID                                     {$$ = set_node("VarDec"); add_children(2, $$, $1);}
    | VarDec LB INT RB                          {$$ = set_node("VarDec"); add_children(5, $$, $1, $2, $3, $4);}
    ;
FunDec : ID LP VarList RP                       {$$ = set_node("FunDec"); add_children(5, $$, $1, $2, $3, $4);}
    | ID LP RP                                  {$$ = set_node("FunDec"); add_children(4, $$, $1, $2, $3);}
    ;
VarList : ParamDec COMMA VarList                {$$ = set_node("VarList"); add_children(4, $$, $1, $2, $3);}
    | ParamDec                                  {$$ = set_node("VarList"); add_children(2, $$, $1);}
    ;
ParamDec : Specifier VarDec                     {$$ = set_node("ParamDec"); add_children(3, $$, $1, $2);}
    ;
/*Statements*/
CompSt : LC DefList StmtList RC                 {$$ = set_node("CompSt"); add_children(5, $$, $1, $2, $3, $4);}
    ;
StmtList : Stmt StmtList                        {$$ = set_node("StmtList"); add_children(3, $$, $1, $2);}
    |                                           {$$ = set_node("StmtList");}
    ;
Stmt : Exp SEMI                                 {$$ = set_node("Stmt"); add_children(3, $$, $1, $2);}
    | CompSt                                    {$$ = set_node("Stmt"); add_children(2, $$, $1);}
    | RETURN Exp SEMI                           {$$ = set_node("Stmt"); add_children(4, $$, $1, $2, $3);}
    | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE   {$$ = set_node("Stmt"); add_children(6, $$, $1, $2, $3, $4, $5);}
    | IF LP Exp RP Stmt ELSE Stmt               {$$ = set_node("Stmt"); add_children(6, $$, $1, $2, $3, $4, $5, $6, $7);}
    | WHILE LP Exp RP Stmt                      {$$ = set_node("Stmt"); add_children(6, $$, $1, $2, $3, $4, $5);}
    | error SEMI
    ;
/*Local Definitions*/
DefList : Def DefList                           {$$ = set_node("DefList"); add_children(3, $$, $1, $2);}
    |                                           {$$ = set_node("DefList");}
    ;
Def : Specifier DecList SEMI                    {$$ = set_node("Def"); add_children(4, $$, $1, $2, $3);}
    ;
DecList : Dec                                   {$$ = set_node("DecList"); add_children(2, $$, $1);}
    | Dec COMMA DecList                         {$$ = set_node("DecList"); add_children(4, $$, $1, $2, $3);}
    ;
Dec : VarDec                                    {$$ = set_node("Dec"); add_children(2, $$, $1);}
    | VarDec ASSIGNOP Exp                       {$$ = set_node("Dec"); add_children(4, $$, $1, $2, $3);}
    ;
/*Expressions*/
Exp : Exp ASSIGNOP Exp                          {$$ = set_node("Exp"); add_children(4, $$, $1, $2, $3);}
    | Exp AND Exp                               {$$ = set_node("Exp"); add_children(4, $$, $1, $2, $3);}
    | Exp OR Exp                                {$$ = set_node("Exp"); add_children(4, $$, $1, $2, $3);}
    | Exp RELOP Exp                             {$$ = set_node("Exp"); add_children(4, $$, $1, $2, $3);}
    | Exp PLUS Exp                              {$$ = set_node("Exp"); add_children(4, $$, $1, $2, $3);}
    | Exp MINUS Exp                             {$$ = set_node("Exp"); add_children(4, $$, $1, $2, $3);}
    | Exp STAR Exp                              {$$ = set_node("Exp"); add_children(4, $$, $1, $2, $3);}
    | Exp DIV Exp                               {$$ = set_node("Exp"); add_children(4, $$, $1, $2, $3);}
    | LP Exp RP                                 {$$ = set_node("Exp"); add_children(4, $$, $1, $2, $3);}
    | MINUS Exp                                 {$$ = set_node("Exp"); add_children(3, $$, $1, $2);}
    | NOT Exp                                   {$$ = set_node("Exp"); add_children(3, $$, $1, $2);}
    | ID LP Args RP                             {$$ = set_node("Exp"); add_children(5, $$, $1, $2, $3, $4);}
    | ID LP RP                                  {$$ = set_node("Exp"); add_children(4, $$, $1, $2, $3);}
    | Exp LB Exp RB                             {$$ = set_node("Exp"); add_children(5, $$, $1, $2, $3, $4);}
    | Exp DOT ID                                {$$ = set_node("Exp"); add_children(4, $$, $1, $2, $3);}
    | ID                                        {$$ = set_node("Exp"); add_children(2, $$, $1);}
    | INT                                       {$$ = set_node("Exp"); add_children(2, $$, $1);}
    | FLOAT                                     {$$ = set_node("Exp"); add_children(2, $$, $1);}
    ;
Args : Exp COMMA Args                           {$$ = set_node("Args"); add_children(4, $$, $1, $2, $3);}
    | Exp                                       {$$ = set_node("Args"); add_children(2, $$, $1);}
    | error COMMA
    ;
%%

int yyerror(char* msg){
    pass = 0;
    fprintf(stderr, "Error type B at Line %d: Something Wrong! %s\n", yylineno, yytext);
}