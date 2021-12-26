%{
    #include<stdio.h>
   //You need to include a head file here.
%}
INTCON [\-]?[1-9][0-9]*|0
IDENT [A-Za-z][A-Za-z0-9]*
CHARCON '[^']*'

%%
"+"                 {printf("%s: PLUS\n",yytext);}
"-"                 {printf("%s: MINUS\n",yytext);}
"*"                 {printf("%s: TIMES\n",yytext);}
"/"                 {printf("%s: DIVSYM\n",yytext);}
"="                 {printf("%s: EQL\n",yytext);}
"<>"                {printf("%s: NEQ\n",yytext);}
"<"                 {printf("%s: LSS\n",yytext);}
"<="                {printf("%s: LEQ\n",yytext);}
">"                 {printf("%s: GTR\n",yytext);}
">="                {printf("%s: GEQ\n",yytext);}
"["                 {printf("%s: LBRACK\n",yytext);}
"]"                 {printf("%s: RBRACK\n",yytext);}
"("                 {printf("%s: LPAREN\n",yytext);}
")"                 {printf("%s: RPAREN\n",yytext);}
","                 {printf("%s: COMMA\n",yytext);}
";"                 {printf("%s: SEMICOLON\n",yytext);}
"."                 {printf("%s: PERIOD\n",yytext);}
":="                {printf("%s: BECOME\n",yytext);}
":"                 {printf("%s: COLON\n",yytext);}
of                  {printf("%s: OFSYM\n",yytext);}
array               {printf("%s: ARRAYSYM\n",yytext);}
program             {printf("%s: PROGRAMSYM\n",yytext);}
mod                 {printf("%s: MODSYM\n",yytext);}
and                 {printf("%s: ANDSYM\n",yytext);}
or                  {printf("%s: ORSYM\n",yytext);}
not                 {printf("%s: NOTSYM\n",yytext);}
begin               {printf("%s: BEGINSYM\n",yytext);}
end                 {printf("%s: ENDSYM\n",yytext);}
if                  {printf("%s: IFSYM\n",yytext);}
then                {printf("%s: THENSYM\n",yytext);}
else                {printf("%s: ELSESYM\n",yytext);}
while               {printf("%s: WHILESYM\n",yytext);}
do                  {printf("%s: DOSYM\n",yytext);}
call                {printf("%s: CALLSYM\n",yytext);}
const               {printf("%s: CONSTSYM\n",yytext);}
type                {printf("%s: TYPESYM\n",yytext);}
var                 {printf("%s: VARSYM\n",yytext);}
procedure           {printf("%s: PROCSYM\n",yytext);}
{INTCON}            {printf("%s: INTCON\n", yytext);}
{IDENT}             {printf("%s: IDENT\n", yytext);}
{CHARCON}           {printf("%s: CHARCON\n",yytext);}
[ \t\n]+            /* eat up whitespace */
.                   {printf("%s: ERROR\n",yytext);}
%%
int yywrap() { return 1; }
int main(int argc, char **argv)
{
	if (argc > 1) {
		if (!(yyin = fopen(argv[1], "r"))) {
			perror(argv[1]);
			return 1;
		}
	}
	while (yylex());
	return 0;
}