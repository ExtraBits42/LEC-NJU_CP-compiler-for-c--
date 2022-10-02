%{
    #include "sat_gen.h"
    #define YYSTYPE Node*
    #include "syntax.tab.h"
    #include<stdio.h>
    extern int pass;
    #define TOKENINIT(n) yylval = build_lexical_node(n, n);

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


{INT10}|{INT8}|{INT16} {yylval = build_lexical_node("INT", yytext); return INT;}

{FLOAT}|{FLOATE} {yylval = build_lexical_node("FLOAT", yytext); return FLOAT;}

{TYPE} {TOKENINIT("TYPE"); return TYPE;}
{STRUCT} {TOKENINIT("STRUCT"); return STRUCT;}
{RETURN} {TOKENINIT("RETURN"); return RETURN;}
{IF} {TOKENINIT("IF"); return IF;}
{ELSE} {TOKENINIT("ELSE"); return ELSE;}
{WHILE} {TOKENINIT("WHILE"); return WHILE;}

{ID} {TOKENINIT("ID"); return ID;}

{AND} {TOKENINIT("AND"); return AND;}
{OR} {TOKENINIT("OR"); return OR;}
{NOT} {TOKENINIT("NOT"); return NOT;}

{PLUS} {TOKENINIT("PLUS"); return PLUS;}
{MINUS} {TOKENINIT("MINUS"); return MINUS;}
{STAR} {TOKENINIT("STAR"); return STAR;}
{DIV} {TOKENINIT("DIV"); return DIV;}

{RELOP} {TOKENINIT("RELOP"); return RELOP;}

{ASSIGNOP} {TOKENINIT("ASSIGNOP"); return ASSIGNOP;}


{LP} {TOKENINIT("LP"); return LP;}
{RP} {TOKENINIT("RP"); return RP;}
{LB} {TOKENINIT("LB"); return LB;}
{RB} {TOKENINIT("RB"); return RB;}
{LC} {TOKENINIT("LC"); return LC;}
{RC} {TOKENINIT("RC"); return RC;} 

{SEMI} {TOKENINIT("SEMI"); return SEMI;}
{COMMA} {TOKENINIT("COMMA"); return COMMA;}
{DOT} {TOKENINIT("DOT"); return DOT;}

" "
\n

. {
    /*Type A: fault report*/
    pass = 0;
    printf("Error type A at Line %d: Mysterious characters \'%s\'\n", yylineno, yytext);
}
%%
