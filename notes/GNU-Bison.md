# GNU Bison
- 编写描述语法规则的文件：
    - 文件后缀名可为任意后缀，如:syntax.y...
- 执行命令`bison <file_name>`利用Bison对语法规则源代码进行编译，生成`<file_name>.yy.c`的C语言源代码：
    - e.g. bison syntax.y
- `<file_name>.yy.c`C语言源文件说明：
    - 提供了函数`yyparse()`，其用于对输入文件进行语法分析，如果分析成功没有错误则返回0，否则返回非0。
        - 该函数的运转需要Flex提供的`yylex()`函数提供词法单元;
        - 使用Flex提供的此法但也接口，需要在Bison源代码文件中添加引用:`#include"lex.yy.c"`,并且在编译源代码时追加`-d`参数：`bison -d syntax.y`;(该参数含义：将编译结果拆分为`syntax.tab.c`和`syntax.tab.h`两个文件，`.h`文件包含一些此法单元的类型定义之类的内容)
    - 获得`.h`文件后，需要修改Flex源代码文件增加对`syntax.tab.h`的引用，让Flex源代码中规则部分的每一条action都返回相应的词法单元：
        ```
        %{
            #include "syntax.tab.h"
            ...
        }%
        ...
        %%
        "+" {return PLUS;}
        "-" {return SUB;}
        "&&" {return AND;}
        ...
        ```
    其中，返回值PLUS和SUB等都是在Bison源代码中定义过的词法单元（定义方法后文会提及）
- 最后拥有三个C语言源代码文件：`main.c`，`lex.yy.c`，`syntax.tab.c`,其中`lex.yy.c`已经被`syntax.tab.c`引用，最后要做的就是将`main.c`和`syntax.tab.c`放到一起进行编译：
    `gcc main.c syntax.tab.c -lfl -ly -o parser`
> 其中,`-lfl`、`-ly`不要省略，否则GCC会因缺少库函数而报错

- 现在我们就可以使用编译出的`parser`程序进行语法分析了，例如：
    `./parser test.cmm`对名为test.cmm的文件进行语法分析。


