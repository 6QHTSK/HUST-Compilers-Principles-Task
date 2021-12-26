#填充本shell文本命令，完成test.txt文件中的中间代码，编译生成可执行文件test
rm -f test
opt --mem2reg test.txt -S > test.ll
llc test.ll -o test.s
clang test.s -o test
rm -f test.s