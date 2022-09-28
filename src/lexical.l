INT10 ([0-9])|([1-9][0-9]+)
INT8 [0][1-7][0-7]*
INT16 ((0x)|(0X))[0-9a-fA-F]*
INT {INT10}|{INT8}|{INT16}
NOR_FLOAT {INT}\.([0-9]+)
SCI_FLOAT {NOR_FLOAT}[e,E][\+, \-]?{INT10}
FLOAT {SCI_FLOAT}|{NOR_FLOAT}
ID [_a-zA-Z][_a-zA-Z0-9]*
SEMT \;
COMMA \,
ASSIGNOP =
RELOP (>|<|>=|<=|==|!=)
PLUS \+
MINUS \-
STAR \*
DIV /
AND &&
OR \|\|
DOT \.
NOT !
TYPE (int)|(float)
LP \(
RP \)
LB \[
RB \]
LC \{
RC \}
STRUCT struct
RETURN return
IF if
ELSE else
WHILE while
%%
%%
