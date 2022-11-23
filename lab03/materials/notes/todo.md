# Just For Fun
- 遇到Bug没法de出来怎么办？直接摆烂一段时间，心情好了再回来看，一眼就能找出bug。


- [ ] 设计程序的顶层对外接口
- [ ] 函数符号表初始化进行特殊处理(预填入WRITE,READ函数)
- [ ] 锚定技术选型:
    - 中间代码表示类型：线形表示
    - 中间代码数据结构：
    - 数据流分析&控制流分析：



# Syntax:
------
Program : ExtDefList
    ;
ExtDefList : ExtDef ExtDefList
    |
    ;
ExtDef : Specifier ExtDecList SEMI
    | Specifier SEMI
    | Specifier FunDec CompSt
    ;
ExtDecList : VarDec
    | VarDec COMMA ExtDecList
    ;

/*7.1.3 Specifiers*/
Specifier : TYPE
    | StructSpecifier
    ;
StructSpecifier : STRUCT OptTag LC DefList RC
    | STRUCT Tag
    ;
OptTag : ID
    |
    ;
Tag : ID
    ;
/*7.1.4 Declarators*/
VarDec : ID
    | VarDec LB INT RB
    ;
FunDec : ID LP VarList RP
    | ID LP RP
    ;
VarList : ParamDec COMMA VarList
    | ParamDec
    ;
ParamDec : Specifier VarDec
    ;
/*7.1.5 Statements*/
CompSt : LC DefList StmtList RC
    ;
StmtList : Stmt StmtList
    |
    ;
Stmt : Exp SEMI
    | CompSt
    | RETURN Exp SEMI
    | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE
    | IF LP Exp RP Stmt ELSE Stmt
    | WHILE LP Exp RP Stmt
    ;
/*7.1.6 Local Definitions*/
DefList : Def DefList
    |
    ;
Def : Specifier DecList SEMI
    ;
DecList : Dec
    | Dec COMMA DecList
    ;
Dec : VarDec
    | VarDec ASSIGNOP Exp
    ;
/*7.1.7 Expressions*/
Exp : INT
	| ID
	| Exp ASSIGNOP Exp
    | Exp PLUS Exp
    | Exp MINUS Exp
    | Exp RELOP Exp
    | NOT Exp
    | Exp AND Exp
    | Exp OR Exp
    | Exp STAR Exp
    | Exp DIV Exp
    | LP Exp RP
    | MINUS Exp
    | ID LP Args RP
    | ID LP RP
    | Exp LB Exp RB
    | Exp DOT ID
    | FLOAT
    ;
Args : Exp COMMA Args
    | Exp
    ;


Lab02重写工作:
- [x] 变量在使用时未经定义。
	- 1. Error type 1 at Line 4: Undefined variable "j".
- [ ] 函数在调用时未经定义。
	- 2. Error type 2 at Line 4: Undefined function "inc".
- [x] 变量出现重复定义，或变量与前面定义过的结构体名字重复。
	- Error type 3 at Line 4: Redefined variable "i".
- [x] 函数出现重复定义（即同样的函数名出现了不止一次定义）。
	- Error type 4 at Line 6: Redefined function "func".
- [ ] 赋值号两边的表达式类型不匹配。
	- Error type 5 at Line 4: Type mismatched for assignment.
- [ ] 赋值号左边出现一个只有右值的表达式。
	- Error type 6 at Line 4: The left-hand side of an assignment must be a variable.
- [ ] 操作数类型不匹配或操作数类型与操作符不匹配（例如整型变量与数组变量相加减，或数组（或结构体）变量与数组（或结构体）变量相加减）。
	- Error type 7 at Line 4: Type mismatched for operands.
- [ ] return语句的返回类型与函数定义的返回类型不匹配。
	- Error type 8 at Line 4: Type mismatched for return.
- [ ] 函数调用时实参与形参的数目或类型不匹配。
	- Error type 9 at Line 8: Function "func(int)" is not applicable for arguments"(int, int)".
- [ ] 对非数组型变量使用“[…]”（数组访问）操作符。
	- Error type 10 at Line 4: "i" is not an array.
- [ ] 对普通变量使用“(…)”或“()”（函数调用）操作符。
	- Error type 11 at Line 4: "i" is not a function.
- [ ] 数组访问操作符“[…]”中出现非整数（例如a[1.5]）。
	- Error type 12 at Line 4: "1.5" is not an integer.
- [ ] 对非结构体型变量使用“.”操作符。
	- Error type 13 at Line 9: Illegal use of ".".
- [ ] 访问结构体中未定义过的域。
	- Error type 14 at Line 9: Non-existent field "n".
- [ ] 结构体中域名重复定义（指同一结构体中），或在定义时对域进行初始化（例如struct A { int a = 0; }）。
	- Error type 15 at Line 4: Redefined field "x".
- [x] 结构体的名字与前面定义过的结构体或变量的名字重复。
	- Error type 16 at Line 6: Duplicated name "Position".
- [x] 直接使用未定义过的结构体来定义变量。
	- Error type 17 at Line 3: Undefined structure "Position".

