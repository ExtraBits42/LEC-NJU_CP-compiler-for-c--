# include<stdio.h>
# include<stdlib.h>

/*declarations*/
extern int yyleng;
extern char* yytext;
extern FILE* yyin;
extern int yylex(void);
extern int yyrestart(FILE* f);
extern int yyparse();

/*global variables*/
int pass = 1;

/*main function*/
int main(int argc, char** argv){
    pass = 1;

    if(argc <= 1) return 1;
    FILE* f = fopen(argv[1], "r");
    if(!f){
        perror(argv[1]);
        return 1;
    }
    yyrestart(f);
    yyparse();
    if(pass == 1) printf("\033[32mPassed!\n\033[0m");
    else printf("\033[31mFault!\n\033[0m");
    return 0;
}