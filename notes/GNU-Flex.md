# GNU Flex

简介：
- GNU Flex是为了帮助GCC进行词法分析而被开发出来。
- 我们只需要为工具提供词法规范的正则表达式，后续自动机转换将由工具自动完成。

## GNU Flex使用流程
- 编写描述词法规则的文件：
    - 文件后缀名可为任意后缀，如:lexical.l(建议使用.l后缀), lexical.cbd...

- 执行命令`flex <file_name>`，利用Flex对描述词法规则的文件进行编译，生成`<file_name>.yy.c`的C语言源代码：
    - e.g. flex lexical.l

- `<file_name>.yy.c`C语言源文件说明：
    - 提供了函数`int yylex(void)`，其作用是读取输入流中的一个词法单元，在其它文件中使用它时需要进行声明；
    - Flex内部还提供了一个变量`yyin`，表示输入文件的文件指针，在其他源文件中使用时需要声明为外部变量：`extern FILE* yyin`;
    - 如果不设置文件指针，则Flex将`yyin`自动设置为stdin(标准输入，通常连接到键盘)

- 生成的.yy.c源文件提供了词法处理的接口`yylex`，我们可以将其与main函数中的其他逻辑组合进行编译：
`gcc main.c lex.yy.c -lfl -o <output_filename>`
> 编译命令中的'-lfl'参数不可缺少，否则GCC会因为缺少库函数报错

- 最后我们可以用生成的可执行文件来进行词法分析。

## GNU Flex正则表达式
...P25-27

## GNU Flex词法规则-源代码编写
整体格式：
```
{definitions}
%%
{rules}
%%
{user subroutines}
```
- 定义部分：给后面可能经常用到的正则表达所取一个别名，从而简化词法规则的书写；
- 规则部分：由正则表达式和相应的响应函数组成，格式为：`pattern {action}`;
- 用户自定义代码部分：该部分代码会原封不动地拷贝到编译生成的`<file_name>.yy.c`当中，以方便用户自定义所需要执行的函数；
    - 如果用户想要对这部分所用到的变量、函数或者头文件进行声明，可以在前面的定义部分（即Flex源代码文件的第一部分）之前使用“%{”和“%}”符号将要声明的内容添加进去。被“%{”和“%}”所包围的内容也会一并拷贝到lex.yy.c的最前面。

实例：
- yyleng是Flex为我们提供的变量，可理解为strlen(yytext)；
- yyrestart(FILE* f)是Flex提供的库函数，其可让Flex将其输入文件的文件指针yyin设置为f；
```
%{
    /* 此处省略#include部分 */
    int chars = 0;
    int words = 0;
    int lines = 0;
%}
letter [a-zA-Z]
%%
{letter}+ { words++; chars+= yyleng; }
\n { chars++; lines++; }
. { chars++; }
%%
int main(int argc, char** argv) {
    if (argc > 1) {
        if (!(yyin = fopen(argv[1], "r"))) {
        perror(argv[1]);
        return 1;
        }
    }
    yylex();
    printf("%8d%8d%8d\n", lines, words, chars);
    return 0;
}
```

## GNU Flex高级特性
...P27-29
- yylineno选项：记录已读行数
- 输入缓冲区：一次从文件中读入大段字符，减少磁盘操作（实现多文件）
- 库函数input：从当前输入文件中读入一个字符（实现注释）
- 库函数unput：将指定字符放回输入缓冲区（实现宏）
- 库函数yyless和yymore：缓冲区放回与当前词素等待（实现双引号内的转义引号）
- 宏REJECT：识别相互重叠的样式