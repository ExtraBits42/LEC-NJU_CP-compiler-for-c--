# multi steps
# generate lexical analyzer and syntax analyzer
parser: main.c lexical.l syntax.y general.h \
		utils/utils.h utils/utils.c \
 		data_struct/spt.h data_struct/spt.c data_struct/sym_table.h data_struct/sym_table.c data_struct/sym_node.c data_struct/type_node.c \
		node_anls/vdec_check.c node_anls/sdec_check.c node_anls/fdec_check.c node_anls/stmt_check.c node_anls/spt_anls.c
	make clean
	flex lexical.l
	bison -d syntax.y
	flex lexical.l
	gcc main.c syntax.tab.c \
	utils/utils.c \
	data_struct/spt.c data_struct/sym_table.c data_struct/sym_node.c data_struct/type_node.c \
	node_anls/spt_anls.c node_anls/vdec_check.c node_anls/sdec_check.c node_anls/fdec_check.c node_anls/stmt_check.c\
	 -lfl -ly -g -o parser
	rm -rf lex.yy.c syntax.tab.c syntax.tab.h

# clean up all middle source files
clean:
	rm -rf lex.yy.c parser syntax.tab.c syntax.tab.h

# debug
test: parser ../testcases/test.cmm
	./parser ../testcases/test.cmm

# run all basic testcases
test-basic: parser
	./parser ../testcases/basic_tcs/basic_tc01.cmm
	./parser ../testcases/basic_tcs/basic_tc02.cmm
	./parser ../testcases/basic_tcs/basic_tc03.cmm
	./parser ../testcases/basic_tcs/basic_tc04.cmm
	./parser ../testcases/basic_tcs/basic_tc05.cmm
	./parser ../testcases/basic_tcs/basic_tc06.cmm
	./parser ../testcases/basic_tcs/basic_tc07.cmm
	./parser ../testcases/basic_tcs/basic_tc08.cmm
	./parser ../testcases/basic_tcs/basic_tc09.cmm
	./parser ../testcases/basic_tcs/basic_tc10.cmm
	./parser ../testcases/basic_tcs/basic_tc11.cmm
	./parser ../testcases/basic_tcs/basic_tc12.cmm
	./parser ../testcases/basic_tcs/basic_tc13.cmm
	./parser ../testcases/basic_tcs/basic_tc14.cmm
	./parser ../testcases/basic_tcs/basic_tc15.cmm
	./parser ../testcases/basic_tcs/basic_tc16.cmm
	./parser ../testcases/basic_tcs/basic_tc17.cmm

test-expand: parser
	./parser ../testcases/exp_tcs/exp_tc01.cmm
	./parser ../testcases/exp_tcs/exp_tc02.cmm
	./parser ../testcases/exp_tcs/exp_tc03.cmm
	./parser ../testcases/exp_tcs/exp_tc04.cmm
	./parser ../testcases/exp_tcs/exp_tc05.cmm
	./parser ../testcases/exp_tcs/exp_tc06.cmm
