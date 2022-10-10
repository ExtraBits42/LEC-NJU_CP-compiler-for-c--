# multi steps
# generate lexical analyzer and syntax analyzer
parser: main.c lexical.l syntax.y general.h sat_gen.c ./utils/utils.h ./utils/utils.c
	make clean
	flex lexical.l
	bison -d syntax.y
	flex lexical.l
	gcc main.c syntax.tab.c sat_gen.c ./utils/utils.c -lfl -ly -o parser
# clean up all middle source files
clean:
	rm -rf scanner lex.yy.c parser syntax.tab.c syntax.tab.h

# single step
bison-d : lexical.l syntax.y
	flex lexical.l
	bison -d syntax.y


# debug
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
