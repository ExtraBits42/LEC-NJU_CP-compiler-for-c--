%{
    #include "syntax.tab.h"    
%}
/*integer data*/
INT10 ([0-9])|([1-9][0-9]+)
INT8 [0][1-7][0-7]+
INT16 ("0x"|"0X")[0-9a-fA-F]+
/*float data*/
FLOAT (([0-9]\.[0-9]+)|[1-9][0-9]+\.[0-9]+)
FLOATE ((\.[0-9]+)|({INT10}\.)|({INT10}\.[0-9]+))[eE][\+,-]?{INT10}
/*key words*/
TYPE ("int"|"float")
STRUCT ("struct")
RETURN ("return")
IF ("if")
ELSE ("else")
WHILE ("while")
/*id name, excluding key words*/
ID [_a-zA-Z][_0-9a-zA-Z]*
/*logic operators*/
AND "&&"
OR "||"
NOT "!"
/*calculate operators*/
PLUS "+"
MINUS "-"
STAR "*"
DIV "/"
/*compare operators*/
RELOP (">"|"<"|">="|"<="|"=="|"!=")
/*assignment operators*/
ASSIGNOP "="
/*parentheses*/
LP "("
RP ")"
LB "["
RB "]"
LC "{"
RC "}"
/*others*/
SEMI ";"
COMMA ","
DOT "."
%%
{INT10} {printf("\033[36m%d\033[0m", atoi(yytext)); return INT;}
{INT8} {printf("\033[36m%s\033[0m", yytext); return INT;}
{INT16} {printf("\033[36m%s\033[0m", yytext); return INT;}

{FLOAT} {printf("\033[36m%s\033[0m", yytext); return FLOAT;}
{FLOATE} {printf("\033[36m%s\033[0m", yytext); return FLOAT;}

{TYPE} {printf("\033[36m%s\033[0m", yytext); return TYPE;}
{STRUCT} {printf("\033[36m%s\033[0m", yytext); return STRUCT;}
{RETURN} {printf("\033[36m%s\033[0m", yytext); return RETURN;}
{IF} {printf("\033[36m%s\033[0m", yytext); return IF;}
{ELSE} {printf("\033[36m%s\033[0m", yytext); return ELSE;}
{WHILE} {printf("\033[36m%s\033[0m", yytext); return WHILE;}

{ID} {printf("\033[36m%s\033[0m", yytext); return ID;}

{AND} {printf("\033[36m%s\033[0m", yytext); return AND;}
{OR} {printf("\033[36m%s\033[0m", yytext); return OR;}
{NOT} {printf("\033[36m%s\033[0m", yytext); return NOT;}

{PLUS} {printf("\033[36m%s\033[0m", yytext); return PLUS;}
{MINUS} {printf("\033[36m%s\033[0m", yytext); return MINUS;}
{STAR} {printf("\033[36m%s\033[0m", yytext); return STAR;}
{DIV} {printf("\033[36m%s\033[0m", yytext); return DIV;}

{RELOP} {printf("\033[36m%s\033[0m", yytext); return RELOP;}

{ASSIGNOP} {printf("\033[36m%s\033[0m", yytext); return ASSIGNOP;}

{LP} {printf("\033[36m%s\033[0m", yytext); return LP;}
{RP} {printf("\033[36m%s\033[0m", yytext); return RP;}
{LB} {printf("\033[36m%s\033[0m", yytext); return LB;}
{RB} {printf("\033[36m%s\033[0m", yytext); return RB;}
{LC} {printf("\033[36m%s\033[0m", yytext); return LC;}
{RC} {printf("\033[36m%s\033[0m", yytext); return RC;} 

{SEMI} {printf("\033[36m%s\033[0m", yytext); return SEMI;}
{COMMA} {printf("\033[36m%s\033[0m", yytext); return COMMA;}
{DOT} {printf("\033[36m%s\033[0m", yytext); return DOT;}

[" ", \n]* {printf("%s", yytext);}

. {
    /*Type A: fault report*/
    printf("Error type A at Line %d: Mysterious characters \'%s\'\n", yylineno, yytext);
}
%%

