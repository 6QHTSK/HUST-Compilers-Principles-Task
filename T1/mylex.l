%{
#include <stdio.h>
int lineCount=0,charCount=0;
%}
%%
\n              {lineCount++; }
.               {charCount++; }
%%
/* 下面是用户子程序段 */
int yywrap() { return 1; }
int main() {
    yylex();
    printf("Lines=%d,Chars=%d",lineCount,charCount);
    return 0;
}