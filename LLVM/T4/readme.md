## LLVM优化

### [Task1](/LLVM/T3-opt)

含优化的代码位于LLVM/T3-opt。按照要求所做即可，注意需要额外添加-=符号和<符号的解析

### Task2

把所需命令一行一行写到task2.sh即可。寻找所需参数可以用下面的命令快速寻找

```bash
opt -h | grep "Promote Memory to Register"
```

### Task3

避坑注意，如果使用较新版本的llvm可能会有 ``opt: unknown pass name 'CountPass'`` 的错误！[详细信息](https://groups.google.com/g/llvm-dev/c/kQYV9dCAfSg?pli=1)

解决方法：``opt load`` 命令增加参数 ``-enable-new-pm=0``

Hint: ``outs()``在此题约等于``cout``

写了个Makefile便于调试

```bash
#题目要求功能
make count 
#检查CountPass模块是否正确插入到opt中
make countTest 
```
