# One step: GNU-Flex generate lexical Analysis
flex: lexical.l
	flex lexical.l
# multisteps:get lexical scanner
flex-t: main.c lexical.l test.cmm
	flex lexical.l
	gcc main.c lex.yy.c -lfl -o scanner
	./scanner test.cmm

# clean unnecessary middle files
clean:
	rm -rf scanner lex.yy.c