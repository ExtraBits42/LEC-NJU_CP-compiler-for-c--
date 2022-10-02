%{
    #include "sat_gen.h"
    #define YYSTYPE Node*
    #include "syntax.tab.h"
    #include<stdio.h>
    extern int pass;

    #define TOKENINIT yylval = (Node *)malloc(sizeof(Node)); \
                      yylval->ntype = LEXICAL; \
                      yylval->u.lex_val = yytext; \
                      yylval->child = NULL; \
                      yylval->tail = NULL; \
                      yylval->next = NULL;
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


{INT10}|{INT8}|{INT16} {TOKENINIT; yylval->name = "INT"; return INT;}

{FLOAT}|{FLOATE} {TOKENINIT; yylval->name = "FLOAT"; return FLOAT;}

{TYPE} {TOKENINIT; yylval->name = "TYPE"; return TYPE;}
{STRUCT}|{RETURN}|{IF}|{ELSE}|{WHILE} {
    TOKENINIT;
    yylval->name = "KEY_WORD";
    REJECT;
}
{STRUCT} {return STRUCT;}
{RETURN} {return RETURN;}
{IF} {return IF;}
{ELSE} {return ELSE;}
{WHILE} {return WHILE;}

{ID} {TOKENINIT; yylval->name = "ID"; return ID;}

{AND}|{OR}|{NOT}|{PLUS}|{MINUS}|{STAR}|{DIV}|{RELOP}|{ASSIGNOP} {
    TOKENINIT;
    yylval->name = "OP";
    REJECT;
}
{AND} {return AND;}
{OR} {return OR;}
{NOT} {return NOT;}

{PLUS} {return PLUS;}
{MINUS} {return MINUS;}
{STAR} {return STAR;}
{DIV} {return DIV;}

{RELOP} {return RELOP;}

{ASSIGNOP} {return ASSIGNOP;}


{LP} {TOKENINIT; yylval->name = "LP"; return LP;}
{RP} {TOKENINIT; yylval->name = "RP"; return RP;}
{LB} {TOKENINIT; yylval->name = "LB"; return LB;}
{RB} {TOKENINIT; yylval->name = "RB"; return RB;}
{LC} {TOKENINIT; yylval->name = "LC"; return LC;}
{RC} {TOKENINIT; yylval->name = "RC"; return RC;} 

{SEMI} {TOKENINIT; yylval->name = "SEMI"; return SEMI;}
{COMMA} {TOKENINIT; yylval->name = "COMMA"; return COMMA;}
{DOT} {TOKENINIT; yylval->name = "DOT"; return DOT;}

" "
\n

. {
    /*Type A: fault report*/
    pass = 0;
    printf("Error type A at Line %d: Mysterious characters \'%s\'\n", yylineno, yytext);
}
%%
