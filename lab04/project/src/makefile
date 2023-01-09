# multi steps
# generate lexical analyzer and syntax analyzer
parser: main.c lexical.l syntax.y general.h \
		utils/utils.h utils/utils.c \
 		data_struct/spt.h data_struct/spt.c \
		data_struct/type_node.h data_struct/type_node.c data_struct/sym_node.h data_struct/sym_node.c data_struct/sym_table.h data_struct/sym_table.c \
		data_struct/ir_list.h data_struct/ir_list.c \
		node_anls/nodes_anls.h node_anls/nodes_anls.c node_anls/semantic_anls.h node_anls/semantic_anls.c \
		node_anls/semantic_checker.h node_anls/semantic_checker.c \
		mips_gen/mips_gen.h mips_gen/mips_gen.c \
		ir_gen/ir_gen.h ir_gen/ir_gen.c ir_gen/ir_spt_anls.h ir_gen/ir_spt_anls.c
		
	make clean
	flex lexical.l
	bison -d syntax.y
	flex lexical.l

	gcc main.c syntax.tab.c \
	utils/utils.c \
	data_struct/spt.c \
	data_struct/type_node.c data_struct/sym_node.c data_struct/sym_table.c \
	node_anls/nodes_anls.c node_anls/semantic_anls.c node_anls/semantic_checker.c \
	data_struct/ir_list.c \
	ir_gen/ir_gen.c ir_gen/ir_spt_anls.c \
	mips_gen/mips_gen.c \
	-lfl -ly -g -o parser

	rm -rf lex.yy.c syntax.tab.c syntax.tab.h

# clean up all middle source files
clean:
	rm -rf lex.yy.c parser syntax.tab.c syntax.tab.h

# debug
test: parser ../testcases/test.cmm
	./parser ../testcases/test.cmm ./test.s
