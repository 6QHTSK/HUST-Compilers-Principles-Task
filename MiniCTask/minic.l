%{
#include "string.h"
#include "def.h"

extern int yycolumn;

#define YYSTYPE yylval
extern "C"
{	
	int yywrap(void);
	int yylex(void);
}

void flexout(std::string name,std::string value){
  std::cout<<name<<"\t:"<<value<<std::endl;
}
//flex为bison提供行号信息
%}
%option yylineno 
INTCON [\-]?[1-9][0-9]*|0[0-7]*|0x[0-9A-F]+
FLOATCON [\-]?[1-9]\.[0-9]+|[\-]?[1-9]\.[0-9]+e[\-\+][0-9]+
ERROROCTCON 0[A-Z0-9]+
ERRORHEXCON 0x[0-9A-Z]*
IDENT [A-Za-z][A-Za-z0-9]*
CHARCON '[^']*'
RELOP '<'|'>'|'<='|'>='|'=='|'!='

%%

"int"       {flexout("TYPE","int");}
"float"     {flexout("TYPE","float");}
"char"      {flexout("TYPE","char");}

"struct"    {flexout("STRUCT",yytext);}
"return"    {flexout("RETURN",yytext);}
"if"        {flexout("IF",yytext);}
"else"      {flexout("ELSE",yytext);}
"while"      {flexout("WHILE",yytext);}

"("         {flexout("LP",yytext);}
")"         {flexout("RP",yytext);}
"["         {flexout("LB",yytext);}
"]"         {flexout("RB",yytext);}
"{"         {flexout("LC",yytext);}
"}"         {flexout("RC",yytext);}

"++"       {flexout("PLUSPLUS",yytext);} 
"--"       {flexout("MINUSMINUS",yytext);} 
"+="       {flexout("PLUSASS",yytext);} 
"-="       {flexout("MINUSASS",yytext);} 

"+"         {flexout("PLUS",yytext);}
"-"         {flexout("MINUS",yytext);}
"*"         {flexout("STAR",yytext);}
"/"         {flexout("DIV",yytext);}

"&&"        {flexout("AND",yytext);}
"||"        {flexout("OR",yytext);}
"!"         {flexout("NOT",yytext);}

"="         {flexout("ASSIGNOP",yytext);}
";"         {flexout("SEMI",yytext);}
","         {flexout("COMMA",yytext);}
"."         {flexout("DOT",yytext);}

{RELOP}     {flexout("RELOP",yytext);}
{INTCON}    {flexout("INT",yytext);}
{IDENT}     {flexout("ID",yytext);}
{FLOATCON}  {flexout("FLOAT",yytext);}

{ERROROCTCON} {printf("Error type A at Line %d: Illegal octal number '%s'\n",yylineno,yytext);}
{ERRORHEXCON} {printf("Error type A at Line %d: Illegal hexadecimal number '%s'\n",yylineno,yytext);}

[\n]		{yycolumn=1;}
[ \r\t]		{/*printf("过滤空格等字符\n");*/}

.           {printf("Error type A at Line %d: Mysterious characters '%s'\n",yylineno,yytext);}
%%
/*
 

*/
int yywrap()
{
    return 1;
}
