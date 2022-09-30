%{
    #include"syntax.tab.h"    
%}
digit [0-9]
%%
{digit}* {
    yylval = atoi(yytext);
    return INT;
}
"+" {return ADD;}
"-" {return SUB;}
"*" {return MUL;}
"/" {return DIV;}
%%