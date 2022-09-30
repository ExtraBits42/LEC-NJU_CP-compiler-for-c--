clean:
	rm -rf lex.yy.c syntax.tab.c syntax.tab.h parser
flex:
	flex lex.l
bison:
	bison syntax.y
bison-d:
	bison -d syntax.y
all: main.c syntax.tab.c
	gcc main.c syntax.tab.c -lfl -ly -o parser
test: parser test.cmm
	./parser test.cmm