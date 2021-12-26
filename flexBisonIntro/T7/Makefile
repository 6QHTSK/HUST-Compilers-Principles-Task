scanner:parser.y
	bison -d parser.y
	flex token.l
	gcc -o scanner parser.tab.c lex.yy.c -lm -lfl
.PHONY:clean
clean:
	rm scanner parser.tab.c lex.yy.c parser.tab.h