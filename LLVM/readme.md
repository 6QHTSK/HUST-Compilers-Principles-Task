由于本人能力有限，以下内容难免有疏漏，如有错误的地方，敬请指出（QQ/issue）

也欢迎用PR来帮助我完善这个页面

# Hint

## T1

简单的中间代码的生成

IR 中，请使用分号作为注释符号。总体结构类似与汇编，但是，load得到的变量只能使用一次，如需继续使用需要重新load。

用于br（类似与goto）的标记可以使用纯数字作为标记

写完的代码在安装完LLVM套件可以用lli指令检查 例，运行1.txt中的IR

```bash
lli 1.txt
```

## T2

依葫芦画瓢

注意一下给出的样例程序，对应部分如下：

```C
#include<stdio.h>   // 不翻译（或者是82～88行）
int k;              // 90～94行

int inc(){          // 96～122行 涉及函数的建立（返回值和参数设置）
                    // 新建基本块和插入点

    int a=1;        // 126～131行，先createAlloca，然后直接Store
                    // Alloca出来的变量。注意，给出的常量1，也需要经过
                    // ConstantInt定义，变成一个Value*
                    // CreateStore 接受一个Value*,把它存到AllocaInst*
                    // 这个地方。Value*的值可以理解为临时变量，可以直接用
                    // AllocaInst* 可以理解为稳定的变量，有存储空间，
                    // 不过使用前需要load出一个Value* 的副本

    int b=3;        // 134~139行，同上

    a = a + b;      // 141~149行，拥有Assignment和BinaryOperator
                    // 对于每一个binaryOperator，首先得到
                    // lhs(左语法树)和rhs（右语法树）的Value*，然后用
                    // LLVM提供的接口CreateAdd（其他运算也有其他接口，
                    // 具体可以去谷歌或者找文档）获得。结果值为一个Value*
                    //（这个值可以作为前面lhs和rhs的Value*继续参与运算）
                    // Assignment 就是把rhs（右边式子）的Value*复制给
                    // lhs（左边变量）的变量，用CreateStore就行了

    if(a>0){        // 151～168,169～170,182行，分支结构
                    // 这是一个仅if的分支结构，涉及两个块，
                    // thenb (then block)块，就是if里面的那个语句块，
                    // ifcontb(if continue block)，if语句右花括号后面
                    // 154～160 是a>0的翻译，其翻译结果保存到了
                    // compare_a_0。162～163是if语句的翻译部分，
                    // 将compare_a_0的值与该类型的空初始化（NULL）比较
                    // 注意这里涉及到了上课知识，为真跳到thenb块，为假
                    // 跳到ifcontb块。记得在翻译thenb块中的内容前，
                    // CreateInsertPoint(thenb)，让程序找到跳转到的点
                    // thenb块翻译结束后，还要跳出thenb块到ifcontb块
                    // 别忘记给ifcont块设置InsertPoint哦！

        a = a+'0';  // 172~178行，同上面的a = a + b
    }
    putchar(a);     // 186～199行。调用函数，先从函数表找到函数，再用
                    // vector存参数（其使用方法可以去cppreference找）
                    // 存的参数是Load出来的Value*
                    // 然后CreateCall就行了。注意，返回值存在了
                    // callputchar 这个Value* 变量里里。

    return 0;       // 201~202 return部分的翻译后面不会涉及
                    // 这题也会给出，不说了
}
```

本题T1、T2两个子任务的生成代码就是第一关的T1和T2，可以照着写

### T1

可以参考样例代码的下列语句进行翻译
```
int a=1;
a = a+b;
putchar(a);
```

### T2
与样例不同，这个还有一个elseb语句块，不过样例的仅if语句块可以参考

thenb 和 elseb 最后都需要br到ifcontb语句块哦

### 输出结果

可以借助makefile生成程序，生成结果的值输出就是T1的中间代码。

## T3~T5

~~你已经基本学会了LLVM C++API的基本用法了，让我们写一个C编译器吧~~

本次编译原理实验最难三关！T3～T5相当于一个任务

写这个任务之前先去把评测脚本PULL下来，避免大量摸索，评测任务就是一个脚本，部分题目的输入写在里面了，可以先去看看。

具体的任务描述起来大概就是，你要DFS遍历语法树（这个建语法树上次实验T4的任务），你在遍历过程中，经过的所有与LLVM C++API相关的代码如果一行一行打印出来，要和你按照T2的要求，手工写的LLVM C++API代码一模一样。

题目给了一个800行左右的程序，有用的部分是后面的codegen。

可以先试着把第一个测试点的程序，喂给上一次的T4生成语法树（图略，也可以把main.cpp注释掉的parse()函数取消注释，后面的部分注释上来看语法树）。题目帮你把最简单的程序实现了，你只需要照着语法树的遍历，从``Program::codegen()``函数开始遍历，什么没写就写什么。内容可以回去看T2的样例。例如子任务``int b;``可以参考样例的``int a=1;``

值得注意的是，变量表中，``namedValues``疑似局部变量，``curNamedValues``疑似全局变量。注意添加的情况。

### T3-1

最简单也是最难的测试点，努力让你的代码的输出结果和最后的一样。

如果报错段错误，可以去看看是不是哪里返回了一个nullptr

### T3-2

含错误类型1。检查一下变量在不在变量表里面

错误类型的定义可以去``NExtDecFunDef::codegen()``里找找代码，然后模拟一份

注意，C++的map类型，检查是否存在该元素的做法建议去网上找。不要用中括号访问！

该错误类型需要在``NAssignment::codegen()`` ``NIdentifier::codegen()``检查

### T3-3

含错误类型2。检查一下``getFunction()``函数的返回值就可以了，nullptr就是没找到这个函数

该错误类型需要在``NMethodCall::codegen()`` 中实现。

### T3-4

含错误类型3。该错误类型需要在``NDef::codegen()`` 中实现。

### T4-1

这里需要额外实现一下函数的调用部分，其他部分T3和初始程序已经实现了。

### T4-2

含错误类型4。题目已经帮忙实现了。不用管

### T4-3

含错误类型5。类型匹配。有点复杂，建议直接去看我的代码或者stacksOverflow上的讨论（谷歌一下（用英语搜索）StacksOverflow第一个就是）

### T4-4及其他

在我的代码里搜索 ``printSemanticError(n,`` (n为错误号)，就可以看到解决方案了。

return值的类型匹配谷歌上没找到，看我的代码就行。在``NReturnStmt::codegen()``里面。

~~比较类似就摸了~~

### T5-1

新增if-else语句块。可以去参考T2-2的代码生成

别忘记往``NBinaryOperator::codegen()``添加新的operator的翻译语句

涉及的``NCompStStmt::codegen()``函数比较复杂，下面再谈

### T5-2

新增 OR 、仅if 和 while语句块。

写完，主要结构就可以过T5-4了，如果段错误检查一下你的``NBinaryOperator::codegen()``

### T5-3

提供一种解决全局变量和局部变量的思路

进入``NCompStStmt::codegen()``函数后，由于CompSt可以生成NDefineList块。涉及到了变量表的部分。开头说了，``namedValues``是局部变量，而``curNamedValues``是全局变量。可以认为，进入CompSt语句块后。原有的局部变量应该变成这个块的全局变量，而且原有的局部变量当与全局变量同名时，应覆盖原有全局变量。这个CompStStmt块结束时，我们要恢复到进入这个块的状态，里面定义的变量离开这个块也就不再记录了（作用域超出）

注意一下，为了恢复，进入这个块操作的之前需要备份原有的变量表。这里需要深复制！

运用这种方法，记得要把``NExtDecFunDef::codegen()``有一个把args加入变量表的部分，删去加入全局变量表的那行代码。

## 总结T3-T5

其实没有必要把所有的codegen()全部写完。写一部分就行了。~~面向样例编程~~

~~下次还填“非常简单”~~

![图片](/assets/meme1.jpeg)

\(图源自网络\)