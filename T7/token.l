%{
#include "parser.tab.h"
/*跟踪单词位置，利用单词规则每次被匹配前，会调用的宏YY_USER_ACTION的机制，同时，利用Bison中定义的
全程变量YYLTYPE yylloc，该变量有4个子项，记录首部行列及尾部行列。换行时需要将列号变量重置为1，
column=1;一般每读入一个词，可以从flex获得该词的行号（yylineno)、词的宽度(yyleng)。通过简单计算，
将位置信息通过yylloc传给语法函数yyparser(),在parser.y文件中可以使用，进行错误处理的定位 */
/*
int yycolumn=1;
#define YY_USER_ACTION yylloc.first_line=yylloc.last_line=yylineno;yylloc.first_column=yycolumn;yylloc.last_column=yycolumn+yyleng-1;yycolumn=yycolumn+yyleng;
*/
%}
%option noyywrap
%option yylineno
NUMSYM [0-9]+
EOLSYM \n
ADDSYM \+
SUBSYM \-
MULSYM \*
DIVSYM \/
EXPOSYM \^
LPSYM \(
RPSYM \)
WHITESYM [ \t]
UNKNOWSYM .
%%
{NUMSYM} {yylval=atof(yytext);printf("Type(%d):NUM Val=%.10g\n",NUM,yylval);return NUM;}
{EOLSYM} {return EOL;}
{ADDSYM} {printf("Type(%d):ADD\n",ADD);return ADD;}
{SUBSYM} {printf("Type(%d):SUB\n",SUB);return SUB;}
{MULSYM} {printf("Type(%d):MUL\n",MUL);return MUL;}
{DIVSYM} {printf("Type(%d):DIV\n",DIV);return DIV;}
{EXPOSYM} {printf("Type(%d):EXPO\n",EXPO);return EXPO;}
{LPSYM} {printf("Type(%d):LP\n",LP);return LP;}
{RPSYM} {printf("Type(%d):RP\n",RP);return RP;}
[WHITESYM] {/*skip the white space symbol*/}
{UNKNOWSYM} {/*printf("Character %s Unknow at line(%d:%d-%d:%d)\n",yytext,yylloc.first_line,yylloc.first_column,yylloc.last_line,yylloc.last_column);*/}
%%