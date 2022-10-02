# One step: GNU-Flex generate lexical Analysis
flex: lexical.l
	flex lexical.l

bison-d: lexical.l syntax.y
	bison -d syntax.y

exec: main.c syntax.tab.c
	gcc main.c syntax.tab.c -lfl -ly -o parser

parser: main.c lexical.l syntax.y
	make clean
	make bison-d
	make flex
	gcc main.c syntax.tab.c sat_gen.c -lfl -ly -o parser

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
