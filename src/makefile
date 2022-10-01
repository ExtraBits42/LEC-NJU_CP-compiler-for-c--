# One step: GNU-Flex generate lexical Analysis
flex: lexical.l
	flex lexical.l
# multisteps:get lexical scanner
flex-t: main.c lexical.l test.cmm
	flex lexical.l
	gcc main.c lex.yy.c -lfl -o scanner
	./scanner test.cmm

bison-d: lexical.l syntax.y
	flex lexical.l
	bison -d syntax.y

all: main.c lexical.l syntax.y
	make clean
	make bison-d
	make flex
	gcc main.c syntax.tab.c -lfl -ly -o parser

test: parser test.cmm
	./parser ./test.cmm

all-t:
	./parser ../testcases/basic-testcases/tc01.cmm
	./parser ../testcases/basic-testcases/tc02.cmm
	./parser ../testcases/basic-testcases/tc03.cmm
	./parser ../testcases/basic-testcases/tc04.cmm

all-extt:
	./parser ../testcases/ext-testcases/tc01.cmm
	./parser ../testcases/ext-testcases/tc02.cmm
	./parser ../testcases/ext-testcases/tc03.cmm
	./parser ../testcases/ext-testcases/tc04.cmm
	./parser ../testcases/ext-testcases/tc05.cmm
	./parser ../testcases/ext-testcases/tc06.cmm

# clean unnecessary middle files
clean:
	rm -rf scanner lex.yy.c parser syntax.tab.c syntax.tab.h
