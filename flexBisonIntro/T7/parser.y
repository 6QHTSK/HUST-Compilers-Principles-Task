%{
/*
Filename:lab107.y
Author:
Date:
Makefile:
______________
scanner:lab107.l lab107.y
	bison -v -d lab107.y
	flex lab107.l
	gcc -o scanner 406.tab.c lex.yy.c -lm -lfl
.PHONY:clean
clean:
	rm scanner lab107.tab.c lex.yy.c lab107.tab.h
_______________
Description:

*/
// Notice: '-' using as -5+2=-3 ;or 5-2, need something special. By LM. 2021 using
// with %precedence NEG used as the highest token, higher than '^', then we can get -2^2=4; without %prec NEG in the rule, SUB is lower than ^, then -2^2=-4
#include <stdio.h>
#include <math.h>
extern int yylineno;
int yylex();
void yyerror(const char *s);
%}

%define api.value.type {double}
%token NUM
%token EOL
%token ADD
%token SUB
%token MUL
%token DIV
%token EXPO
%token LP
%token RP
%left ADD SUB
%left MUL DIV
%right EXPO

%precedence NEG

%%
calclist:
	%empty
	|calclist exp EOL {printf("=%.10g\n",$2);}

exp:term
   	|exp ADD exp {$$=$1+$3;}
	|exp SUB exp {$$=$1-$3;}
    |exp MUL exp {$$=$1*$3;}
    |exp DIV exp {$$=$1/$3;}
    |exp EXPO exp {$$=pow($1,$3);}
	|SUB exp %prec NEG {{$$= -$2;}}
	|error {}
	;

term:NUM
	;
%%
int main(int args,char **argv){
	yyparse();
	return 0;
}
void yyerror(char const *s){
	fprintf(stderr,"MyError:%s yylineno:%d\n",s,yylineno);
}
