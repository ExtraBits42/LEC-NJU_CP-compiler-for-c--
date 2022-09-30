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
{INT10} {printf("\033[36mINT10:\033[0m%d", atoi(yytext)); return INT;}
{INT8} {printf("\033[36mINT8:\033[0m%s", yytext); return INT;}
{INT16} {printf("\033[36mINT16:\033[0m%s", yytext); return INT;}

{FLOAT} {printf("\033[36mFLOAT:\033[0m%s", yytext); return FLOAT;}
{FLOATE} {printf("\033[36mFLOATE:\033[0m%s", yytext); return FLOAT;}

{TYPE} {printf("\033[36mKEY_WORD:\033[0m%s", yytext); return TYPE;}
{STRUCT} {printf("\033[36mKEY_WORD:\033[0m%s", yytext); return STRUCT;}
{RETURN} {printf("\033[36mKEY_WORD:\033[0m%s", yytext); return RETURN;}
{IF} {printf("\033[36mKEY_WORD:\033[0m%s", yytext); return IF;}
{ELSE} {printf("\033[36mKEY_WORD:\033[0m%s", yytext); return ELSE;}
{WHILE} {printf("\033[36mKEY_WORD:\033[0m%s", yytext); return WHILE;}

{ID} {printf("\033[36mID:\033[0m%s", yytext); return ID;}

{AND} {printf("\033[36mLOGIC Op:\033[0m%s", yytext); return AND;}
{OR} {printf("\033[36mLOGIC Op:\033[0m%s", yytext); return OR;}
{NOT} {printf("\033[36mLOGIC Op:\033[0m%s", yytext); return NOT;}

{PLUS} {printf("\033[36mMATH Op:\033[0m%s", yytext); return PLUS;}
{MINUS} {printf("\033[36mMATH Op:\033[0m%s", yytext); return MINUS;}
{STAR} {printf("\033[36mMATH Op:\033[0m%s", yytext); return STAR;}
{DIV} {printf("\033[36mMATH Op:\033[0m%s", yytext); return DIV;}

{RELOP} {printf("\033[36mCOMPARE Op:\033[0m%s", yytext); return RELOP;}

{ASSIGNOP} {printf("\033[36mASSIGN Op:\033[0m%s", yytext); return ASSIGNOP;}

{LP} {printf("\033[36mPARENTH:\033[0m%s", yytext); return LP;}
{RP} {printf("\033[36mPARENTH:\033[0m%s", yytext); return RP;}
{LB} {printf("\033[36mPARENTH:\033[0m%s", yytext); return LB;}
{RB} {printf("\033[36mPARENTH:\033[0m%s", yytext); return RB;}
{LC} {printf("\033[36mPARENTH:\033[0m%s", yytext); return LC;}
{RC} {printf("\033[36mPARENTH:\033[0m%s", yytext); return RC;} 

{SEMI} {printf("\033[36mOTHER:\033[0m%s", yytext); return SEMI;}
{COMMA} {printf("\033[36mOTHER:\033[0m%s", yytext); return COMMA;}
{DOT} {printf("\033[36mOTHER:\033[0m%s", yytext); return DOT;}

[" "]* {printf("%s", yytext);}
[\n] {printf("%s", yytext); yycolumn = 1;}

. {
    /*Type A: fault report*/
    printf("Error type A at Line %d: Mysterious characters \'%s\'\n", yylineno, yytext);
}
%%

