# Experiment02 UpdateLog:
==2022/10/31==:


==2022/10/29==:
- Bug waiting for improve:
	- 符号表的作用域尾指针在弹出栈帧时的更新方法有待优化

==2022/10/28==:
- Design:
	

==2022/10/27==:
- Design:
	- 表示符号定义的节点类型: StructSpecifier(x, 修正：应当是OptTag), VarDec, FuncDec
- Debug:
	- [ ] 符号表节点进行free时，未递归将其指向的所有节点进行free

==2022/10/25==:
- Design:
	- 选定使用"哈希表+栈=十字链表"作为符号表的数据结构
- Debug:
	- lexical-part: .cmm文件中如果使用Tab制表符替换空格，则会出错。因此在词法分析部分追加了空白符-制表符的识别。现可支持.cmm中使用制表符。
	- 考虑到对重复定义的检测，如果使用目前确定的哈希表+栈=十字链表，那么会造成在当前作用域中进行搜索时需要O(n)的时间复杂度。能够设计更优秀的数据结构对其进行优化？(TODO：为每个符号节点追加一个栈深度标号，指明其所属栈帧，从而在限制作用域条件下查找时：仅需计算一次字符串哈希，而后对头部符合当前栈帧标号的节点一一比对即可)

==2022/10/24==:
None

# Reference:
==2022/10/31==:
- Ubuntu系统下不生成core文件的解决办法：
	- https://blog.csdn.net/qq_35621436/article/details/120870746
	- https://blog.csdn.net/xja31415/article/details/52777509
	- Useful: https://blog.csdn.net/weixin_41065922/article/details/121607389
	- Useful: https://blog.csdn.net/xja31415/article/details/52777509
- GDB相关：
	- 指令x，查看指定内存地址的内容：https://blog.csdn.net/haifeng_gu/article/details/73928545
	- 指令where，查看异常发生路径：https://blog.csdn.net/a936676463/article/details/8677797
	- gdb，使用core dump：https://blog.csdn.net/weixin_41065922/article/details/121607389
	- gdb快速定位段错误：https://blog.csdn.net/xja31415/article/details/52777509

==2022/10/29==:
- Debug:
	- 段错误定位方法：https://blog.csdn.net/wit_732/article/details/94353424
- Question:
	- 在不同文件中

==2022/10/28==:
- Makefile:
	- 编写makefile时使用'\\'进行换行，无论是依赖要求还是命令都可以拆分成多行
- C Language:
	- 关键字static、extern

==2022/10/27==:
- C Language:
	- strlen函数
	- 数字转字符
		https://www.dotcpp.com/course/555
		https://www.runoob.com/w3cnote/c-int2str.html
	- 字符串拼接http://c.biancheng.net/c/strcat.html
	- strcat函数
		https://www.dotcpp.com/course/590
		https://blog.csdn.net/u011733907/article/details/107667912

==2022/10/25==:
- C Language:
	- C赋值操作返回值：https://www.cnblogs.com/nangcr/p/return-value-of-assignment-expression.html
	- C malloc函数申请动态数组: https://blog.csdn.net/m0_52688669/article/details/116788103
	- C free函数使用方法：https://www.runoob.com/cprogramming/c-function-free.html
	- C库函数strcmp：https://www.runoob.com/cprogramming/c-function-strcmp.html
	- C库宏assert:https://www.runoob.com/cprogramming/c-macro-assert.html
- Data Structure:
	- 字符串哈希函数: https://blog.csdn.net/MyLinChi/article/details/79508112
	- 哈希表:
		- https://zhuanlan.zhihu.com/p/144296454
		- https://blog.csdn.net/lvyibin890/article/details/82219767
- Questions:
	- 语义分析是否可以视为仅仅只是一个检查阶段，经过检查后，该阶段的信息便可丢弃，不会被后续过程所使用？
		- 并非如此。语义分析所应当做的确实是进行语义层面的检查，但是在检查完成后所形成的符号表则是会被后续步骤所使用的。另外，其中的各类求值运算则是放入到代码生成部分，生成机器码，最后再交由机器使其根据机器码进行运算（而绝不是在语义分析过程中进行运算）。


==2022/10/24==:
- Github:
	- Github解决每次Push都要token: https://blog.csdn.net/LoveFHM/article/details/122923041
	- Github的ID解析: https://ping.chinaz.com/github.com
- Git:
	- Git的reset操作：https://www.runoob.com/git/git-reset.html
- Linux:
	- cp操作
		- https://www.runoob.com/git/git-reset.html
		- https://blog.csdn.net/weixin_43820866/article/details/90453951
		- https://www.runoob.com/linux/linux-comm-cp.html
	- 压缩操作
		- https://blog.csdn.net/songbinxu/article/details/80435665
		- https://www.phpernote.com/linux/1226.html
- C Language:
	- C语言头文件重复包含：https://blog.csdn.net/csdn_kou/article/details/80160031
	- C语言free函数：https://www.runoob.com/cprogramming/c-function-free.html
	- C语言带参宏定义：http://c.biancheng.net/view/1982.html
	- C语言可变参数：https://www.runoob.com/cprogramming/c-variable-arguments.html
- Others:
	- 编程常用命名缩写: https://blog.csdn.net/Rong_Toa/article/details/88366733

- Questions:
	- makefile怎么指定特定的工作目录(输入目录、输出目录)，不使用其所在目录？
	- makefile内部怎么进行流程控制？（循环、顺序、条件）
