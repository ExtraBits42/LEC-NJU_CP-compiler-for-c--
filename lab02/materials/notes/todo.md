# 语法规约
- 运算部分:
    - 整型（int）变量不能与浮点型（float）变量相互赋值或者相互运算。
    - 仅有int型变量才能进行逻辑运算或者作为if和while语句的条件；仅有int型和float型变量才能参与算术运算。
- 函数部分:
    - 任何函数只进行一次定义，无法进行函数声明。==Expand01: 函数可进行声明==
    - 函数无法进行嵌套定义。
- 变量部分:
    - 所有变量（包括函数的形参）的作用域都是全局的，即程序中所有变量均不能重名。==Expand02: 变量的定义受可嵌套作用域的影响==
    - 结构体间的类型等价机制采用名等价（Name Equivalence）的方式。==Expand03: 结构体间的类型等价机制更改为**结构等价**==
    - 结构体中的域不与变量重名，并且不同结构体中的域互不重名。

# 错误类型
- 变量部分:
    - [x]Type:1 变量在使用时未经定义。
    - [x]Type:3 变量出现重复定义，或变量与前面定义过的结构体名字重复。
    - [x]Type:17 直接使用未定义过的结构体来定义变量。

- 函数部分:
    - [x]Type:2 函数在调用时未经定义。
    - [x]Type:4 函数出现重复定义（即同样的函数名出现了不止一次定义）。
    - [x]Type:8 return语句的返回类型与函数定义的返回类型不匹配。
    - [x]Type:9 函数调用时实参与形参的数目或类型不匹配。

    Expand01:
        - [x]Type:18 函数进行了声明，但没有被定义。
        - [ ]Type:19 函数的多次声明互相冲突（即函数名一致，但返回类型、形参数量或者形参类型不一致），或者声明与定义之间互相冲突。

- 运算部分:
    - [x]Type:5 赋值号两边的表达式类型不匹配。
    - [x]Type:6 赋值号左边出现一个只有右值的表达式。
    - [x]Type:7 操作数类型不匹配或操作数类型与操作符不匹配（例如整型变量与数组变量相加减，或数组（或结构体）变量与数组（或结构体）变量相加减）。

    - [x]Type:10 对非数组型变量使用“[…]”（数组访问）操作符。
    - [x]Type:11 对普通变量使用“(…)”或“()”（函数调用）操作符。
    - [x]Type:12 数组访问操作符“[…]”中出现非整数（例如a[1.5]）。
    - [x]Type:13 对非结构体型变量使用“.”操作符。

- 结构体部分:
    - [x]Type:16 结构体的名字与前面定义过的结构体或变量的名字重复。
    - [x]Type:15 结构体中域名重复定义（指同一结构体中），或在定义时对域进行初始化（例如struct A { int a = 0; }）。
    - [x]Type:14 访问结构体中未定义过的域。

# 阶段划分
- 第一次DFS分析: 填充符号表 + 分析OP操作是否合法
    - Others

- 第二次DFS分析: 针对函数定义-声明进行分析
    - [x]Type:18 函数进行了声明，但没有被定义
    - [x]Type:19 函数的多次声明互相冲突（即函数名一致，但返回类型、形参数量或者形参类型不一致），或者声明与定义之间互相冲突。
    - [x] 结构体定义间使用类型等价





/*7.1.1 declared tokens*/
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
/*7.1.2 High-level Definitions*/
[x] Program : ExtDefList
[x]     ;
[x] ExtDefList : ExtDef ExtDefList
[x]     |
[x]     ;
[x] ExtDef : Specifier ExtDecList SEMI
[x]     | Specifier SEMI
[x]     | Specifier FunDec CompSt
[x]     | Specifier FunDec SEMI
[x]     ;
[x] ExtDecList : VarDec
[x]     | VarDec COMMA ExtDecList
[x]     ;
[x] 
[x] /*7.1.3 Specifiers*/
[x] Specifier : TYPE
[x]     | StructSpecifier
[x]     ;
[x] StructSpecifier : STRUCT OptTag LC DefList RC
[x]     | STRUCT Tag
[x]     ;
[x] OptTag : ID
[x]     |
[x]     ;
[x] Tag : ID
[x]     ;
[x] /*7.1.4 Declarators*/
[x] VarDec : ID
[x]     | VarDec LB INT RB
[x]     ;
[x] FunDec : ID LP VarList RP
[x]     | ID LP RP
[x]     ;
[x] VarList : ParamDec COMMA VarList
[x]     | ParamDec
[x]     ;
[x] ParamDec : Specifier VarDec
[x]     ;
[x] /*7.1.5 Statements*/
[x] CompSt : LC DefList StmtList RC
[x]     ;
[x] StmtList : Stmt StmtList
[x]     |
[x]     ;
[x] Stmt : Exp SEMI
[x]     | CompSt
[x]     | RETURN Exp SEMI
[x]     | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE
[x]     | IF LP Exp RP Stmt ELSE Stmt
[x]     | WHILE LP Exp RP Stmt
[x]     ;
[x] /*7.1.6 Local Definitions*/
[x] DefList : Def DefList
[x]     |
[x]     ;
[x] Def : Specifier DecList SEMI
[x]     ;
[x] DecList : Dec
[x]     | Dec COMMA DecList
[x]     ;
[x] Dec : VarDec
[x]     | VarDec ASSIGNOP Exp
[x]     ;
[x] /*7.1.7 Expressions*/
[x] Exp : Exp ASSIGNOP Exp
[x]     | Exp AND Exp
[x]     | Exp OR Exp
[x]     | Exp RELOP Exp
[x]     | Exp PLUS Exp
[x]     | Exp MINUS Exp
[x]     | Exp STAR Exp
[x]     | Exp DIV Exp
[x]     | LP Exp RP
[x]     | MINUS Exp
[x]     | NOT Exp
[x]     | ID LP Args RP
[x]     | ID LP RP
[x]     | Exp LB Exp RB
[x]     | Exp DOT ID
[x]     | ID
[x]     | INT
[x]     | FLOAT
[x]     ;
[x] Args : Exp COMMA Args
[x]     | Exp
[x]     ;
%%
