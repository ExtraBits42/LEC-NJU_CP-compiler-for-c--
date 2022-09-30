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
SEMT ";"
COMMA ","
DOT "."
%%
{INT10} {printf("\033[36mINT10:\033[0m%d", atoi(yytext));}
{INT8} {printf("\033[36mINT8:\033[0m%s", yytext);}
{INT16} {printf("\033[36mINT16:\033[0m%s", yytext);}

{FLOAT} {printf("\033[36mFLOAT:\033[0m%s", yytext);}
{FLOATE} {printf("\033[36mFLOATE:\033[0m%s", yytext);}

{TYPE}|{STRUCT}|{RETURN}|{IF}|{ELSE}|{WHILE} {printf("\033[36mKEY_WORD:\033[0m%s", yytext);}

{ID} {printf("\033[36mID:\033[0m%s", yytext);}

{AND}|{OR}|{NOT} {printf("\033[36mLOGIC Op:\033[0m%s", yytext);}
{PLUS}|{MINUS}|{STAR}|{DIV} {printf("\033[36mMATH Op:\033[0m%s", yytext);}

{RELOP} {printf("\033[36mCOMPARE Op:\033[0m%s", yytext);}

{ASSIGNOP} {printf("\033[36mASSIGN Op:\033[0m%s", yytext);}

{LP}|{RP}|{LB}|{RB}|{LC}|{RC} {printf("\033[36mPARENTH:\033[0m%s", yytext);}

{SEMT}|{COMMA}|{DOT} {printf("\033[36mOTHER:\033[0m%s", yytext);}

[" ", \n]* {printf("%s", yytext);}

. {
    /*Type A: fault report*/
    printf("Error type A at Line %d: Mysterious characters \'%s\'\n", yylineno, yytext);
    return -1;
}
%%

