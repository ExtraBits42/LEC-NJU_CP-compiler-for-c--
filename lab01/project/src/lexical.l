%option yylineno

%{
    #include<stdio.h>
    #include "general.h"
    #define YYSTYPE Node*
    #include "syntax.tab.h"
    
    extern int pass;
    int yycolumn = 1;

    #define TOKENINIT(t, v) yylval = build_lexical_node(t, v);
    #define TOKENINIT_TVSAME(n) yylval = build_lexical_node(n, n);
    #define YY_USER_ACTION \
        yylloc.first_line = yylloc.last_line = yylineno; \
        yylloc.first_column = yycolumn; \
        yylloc.last_column = yycolumn + yyleng - 1; \
        yycolumn += yyleng;
%}

/*integer data*/
INT10 ([0-9])|([1-9][0-9]+)
INT8 [0][0-7]+
INT16 ("0x"|"0X")[0-9a-fA-F]+
/*float data*/
FLOAT (([0-9]"."[0-9]+)|[1-9][0-9]+"."[0-9]+)
FLOATE (([0-9]+".")|("."[0-9]+)|([0-9]+"."[0-9]+))("e"|"E")("+"|"-")?(([0-9])+)
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

/*annotation*/
SCOMMENT "//"
MCOMMENT "/*"
%%


{INT10} { TOKENINIT("INT10", yytext); return INT;}
{INT8} { TOKENINIT("INT8", yytext); return INT;}
{INT16} { TOKENINIT("INT16", yytext); return INT;}

{FLOATE}|{FLOAT} { TOKENINIT("FLOAT", yytext); return FLOAT;}

{TYPE} { TOKENINIT("TYPE", yytext); return TYPE;}
{STRUCT} { TOKENINIT_TVSAME("STRUCT"); return STRUCT;}
{RETURN} { TOKENINIT_TVSAME("RETURN"); return RETURN;}
{IF} { TOKENINIT_TVSAME("IF"); return IF;}
{ELSE} { TOKENINIT_TVSAME("ELSE"); return ELSE;}
{WHILE} { TOKENINIT_TVSAME("WHILE"); return WHILE;}

{ID} { TOKENINIT("ID", yytext); return ID;}

{AND} { TOKENINIT_TVSAME("AND"); return AND;}
{OR} { TOKENINIT_TVSAME("OR"); return OR;}
{NOT} { TOKENINIT_TVSAME("NOT"); return NOT;}

{PLUS} { TOKENINIT_TVSAME("PLUS"); return PLUS;}
{MINUS} { TOKENINIT_TVSAME("MINUS"); return MINUS;}
{STAR} { TOKENINIT_TVSAME("STAR"); return STAR;}
{DIV} { TOKENINIT_TVSAME("DIV"); return DIV;}

{RELOP} { TOKENINIT_TVSAME("RELOP"); return RELOP;}

{ASSIGNOP} { TOKENINIT_TVSAME("ASSIGNOP"); return ASSIGNOP;}


{LP} { TOKENINIT_TVSAME("LP"); return LP;}
{RP} { TOKENINIT_TVSAME("RP"); return RP;}
{LB} { TOKENINIT_TVSAME("LB"); return LB;}
{RB} { TOKENINIT_TVSAME("RB"); return RB;}
{LC} { TOKENINIT_TVSAME("LC"); return LC;}
{RC} { TOKENINIT_TVSAME("RC"); return RC;} 

{SEMI} { TOKENINIT_TVSAME("SEMI"); return SEMI;}
{COMMA} { TOKENINIT_TVSAME("COMMA"); return COMMA;}
{DOT} { TOKENINIT_TVSAME("DOT"); return DOT;}


{SCOMMENT} {
    char c = input();
    while(c != '\n') c = input();
}

{MCOMMENT} {
    int state = 0;
    char c;
    do{
        c = input();
        if(c == '*') state = 1;
        else if(state == 1 && c == '/') state = 2;
        else state = 0;
    } while(state != 2);
}

" " {}
\n { yycolumn = 1;}

. {
    
    /*Type A: fault report*/
    pass = 0;
    printf("Error type A at Line %d:blablabla... %s\n", yylineno, yytext);
}
%%
