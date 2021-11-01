%{
    #include<stdio.h>
   //You need to include a head file here.
%}
%%
a*b             {printf("1");}
ca              {printf("2");}
a*ca*           {printf("3");}
%%
int yywrap(){
    return 1;
}
int main(){
    yylex();
}  