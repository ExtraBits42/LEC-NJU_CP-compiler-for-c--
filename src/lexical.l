%{
    #include "syntax.tab.h"    
%}

%option yylineno

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
{INT10}|{INT8}|{INT16} {return INT;}

{FLOAT}|{FLOATE} {return FLOAT;}

{TYPE} {return TYPE;}
{STRUCT} {return STRUCT;}
{RETURN} {return RETURN;}
{IF} {return IF;}
{ELSE} {return ELSE;}
{WHILE} {return WHILE;}

{ID} {return ID;}

{AND} {return AND;}
{OR} {return OR;}
{NOT} {return NOT;}

{PLUS} {return PLUS;}
{MINUS} {return MINUS;}
{STAR} {return STAR;}
{DIV} {return DIV;}

{RELOP} {return RELOP;}

{ASSIGNOP} {return ASSIGNOP;}

{LP} {return LP;}
{RP} {return RP;}
{LB} {return LB;}
{RB} {return RB;}
{LC} {return LC;}
{RC} {return RC;} 

{SEMI} {return SEMI;}
{COMMA} {return COMMA;}
{DOT} {return DOT;}

" "
\n

. {
    /*Type A: fault report*/
    printf("Error type A at Line %d: Mysterious characters \'%s\'\n", yylineno, yytext);
}
%%
