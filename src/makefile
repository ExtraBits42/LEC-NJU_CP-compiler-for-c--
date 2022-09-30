# One step: GNU-Flex generate lexical Analysis
flex: lexical.l
	flex lexical.l
# multisteps:get lexical scanner
flex-t: main.c lexical.l test.cmm
	flex lexical.l
	gcc main.c lex.yy.c -lfl -o scanner
	./scanner test.cmm

bison-d: lexical.l syntax.y
	bison -d syntax.y

all: main.c syntax.tab.c
	gcc main.c syntax.tab.c -lfl -ly -o parser

all-t:
	./parser ./basic-testcases/tc01.cmm
	./parser ./basic-testcases/tc02.cmm
	./parser ./basic-testcases/tc03.cmm
	./parser ./basic-testcases/tc04.cmm

all-extt:
	./parser ./ext-testcases/tc01.cmm
	./parser ./ext-testcases/tc02.cmm
	./parser ./ext-testcases/tc03.cmm
	./parser ./ext-testcases/tc04.cmm
	./parser ./ext-testcases/tc05.cmm
	./parser ./ext-testcases/tc06.cmm

# clean unnecessary middle files
clean:
	rm -rf scanner lex.yy.c parser syntax.tab.c syntax.tab.h
