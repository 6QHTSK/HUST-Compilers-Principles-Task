%locations
//bison的.y文件中，加入％locations，这样bison加入－d编译后的.h符号表文件中会多出一个YYLTYPE结构体定义和一个该结构体类型的全局变量yylloc，这样，flex的.l文件include该.h文件后，该结构体类型就可以被flex知道，且flex可以向yylloc里面填入信息。

%{
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "def.h"

#include <bits/stdc++.h>
extern int yylineno;
extern char *yytext;
extern FILE *yyin;
char myerror[255];
extern NProgram *p;
extern int allerror;
void myyyerror();

extern "C"{
void yyerror(const char* fmt, ...);
extern int yylex(void);
}
%}

%union {
		NExtDefFunDec  *nExtDefFunDec;
        NExtDefList *nExtDefList;
        NExtDef *nExtDef;
        NSpecifier *nSpecifier;
        NExtDecList *nExtDecList;
        NDecList *nDecList;
        NVarDec *nVarDec;
        NFunDec *nFunDec;
        NCompSt *nCompSt;
        NVarList *nVarList;
        NParamDec *nParamDec;
        NDefList *nDefList;
        NDec *nDec;
        NDef *nDef;
        NStmtList *nStmtList;
        NStmt *nStmt;
        NExpression *nExpression;
        NArgs *nArgs;
        NIdentifier *nIdentifier;
        NStructSpecifier *nStructSpecifier;

        std::string *text;
		int		type_int;
        int		line;
		float	type_float;
		int		type_char;
		char	type_id[32];
}
//union的默认结构体类型为YYSTYPE，相当于自己把YYSTYPE重新定义为union类型。所以相应的yylval也变为union类型。
//这个union类型-d选项编译时会放在头文件中
//  %type 定义非终结符的语义值类型
%type  <nExtDefFunDec> program
%type  <nExtDefList> ExtDefList
%type  <nExtDef> ExtDef
%type  <nSpecifier> Specifier
%type  <nExtDecList> ExtDecList
%type  <nDecList> DecList
%type  <nVarDec> VarDec
%type  <nFunDec> FunDec
%type  <nCompSt> CompSt

%type  <nDefList> DefList
%type  <nVarList> VarList
%type  <nParamDec> ParamDec
%type  <nDec> Dec
%type  <nDef> Def
%type  <nStmtList> StmtList

%type  <nExpression> Exp
%type  <nStmt> Stmt
%type  <nArgs> Args
%type  <nIdentifier> OptTag  Tag
%type  <nStructSpecifier> StructSpecifier
//% token 定义终结符的语义值类型
%token <type_int> INT                   //指定INT的语义值是type_int，有词法分析得到的数值
%token <type_id> ID RELOP TYPE    //指定ID,RELOP 的语义值是type_id，有词法分析得到的标识符字符串mmecpy得到的 char*
%token <type_float> FLOAT               //指定ID的语义值是type_float，有词法分析得到的float
%token <type_char> CHAR
%token <line> STRUCT LP RP LC RC LB RB SEMI COMMA     //用bison对该文件编译时，带参数-d，生成的exp.tab.h中给这些单词进行编码，可在lex.l中包含parser.tab.h使用这些单词种类码
%token <line> DOT PLUS MINUS STAR DIV MOD ASSIGNOP AND OR NOT IF BREAK ELSE WHILE RETURN PLUSASS MINUSASS STARASS DIVASS MODASS PLUSPLUS MINUSMINUS
//由低到高的定义优先级

%left COMMA
%left ASSIGNOP PLUSASS MINUSASS STARASS DIVASS MODASS
//先不支持？三目运算
%left OR
%left AND
%left RELOP//比较运算内部暂不定义优先级
%left PLUS MINUS
%left STAR DIV MOD
%right UMINUS NOT UPLUSPLUS UMINUSMINUS
%left PLUSPLUS MINUSMINUS 
%right DOT
%left LB

%nonassoc LOWER_THEN_ELSE //无结合性
%nonassoc ELSE

// 注意检查line的传递是否会传个null
%%
/*High-level Definitions*/
program: ExtDefList {p=new NProgram($1);if($1) p->line=$1->line;} /*显示语法树*/
        ;
ExtDefList: {  $$=nullptr;}
        | ExtDef ExtDefList {$$=new NExtDefList(*$1,$2); $$->line=$1->line;}
        ;
ExtDef: Specifier ExtDecList SEMI { $$ = new NExtDef(*$1,$2); $$->line=$1->line;}
        | Specifier SEMI {$$ = new NExtDef(*$1); $$->line=$1->line;} 
        | Specifier FunDec CompSt {$$ = new NExtDef(*$1,$2,$3); $$->line=$1->line;}
        ;
ExtDecList: VarDec {$$ = new NExtDecList(*$1,nullptr);$$->line=$1->line;}
        | VarDec COMMA ExtDecList {$$ = new NExtDecList(*$1,$3);$$->line=$1->line;}
        ;

/*Specifiers*/
Specifier: TYPE {{$$=new NSpecifier($1); $$->line=yylineno;}}   
        | StructSpecifier {$$ = $1;} // 注意StructSpecifier继承Specifier 
        ;

StructSpecifier: STRUCT OptTag LC DefList RC {$$ = new NStructSpecifier($2,$4); $$->line=$1;}   
        | STRUCT Tag {$$ = new NStructSpecifier($2); $$->line=$1;}   
        ;

OptTag: ID {$$ = new NIdentifier($1);$$->line=yylineno;}   
        | { $$ = nullptr;}   
        ;

Tag: ID {$$ = new NIdentifier($1);$$->line=yylineno;}   
        ;

/*Declarators*/
VarDec:  ID {NIdentifier* temp = new NIdentifier($1); $$ = new NVarDec(*temp) ;$$->line=yylineno;}   //ID结点，标识符符号串存放结点的type_id
        | VarDec LB INT RB {$$ = $1; $1->v.insert($1->v.begin(),$3);}
        ;
FunDec: ID LP VarList RP {NIdentifier* temp = new NIdentifier($1); $$ = new NFunDec(*temp,$3); $$->line=yylineno;}
        | ID LP RP {NIdentifier* temp = new NIdentifier($1); $$ = new NFunDec(*temp,nullptr); $$->line=yylineno;}
        ;
VarList: ParamDec {$$ = new NVarList(*$1,nullptr); $$->line = $1->line;}
        | ParamDec COMMA VarList {$$ = new NVarList(*$1,$3); $$->line = $1->line;}
        ;
ParamDec: Specifier VarDec {$$ = new NParamDec(*$1,*$2); $$->line = $1->line;}
        ;

/*Statements*/
CompSt:   LC DefList StmtList RC {$$ = new NCompSt($2,$3); $$->line = $1;}
        ;
StmtList: {$$ = nullptr;}  
        | Stmt StmtList  { $$ = new NStmtList(*$1,$2);$$->line = $1->line;}
        ;
Stmt:     Exp SEMI {$$ = new NExpStmt(*$1);$$->line = yylineno;}
        | CompSt {$$ = new NCompStStmt(*$1);$$->line = $1->line;}      //复合语句结点直接最为语句结点，不再生成新的结点
        | RETURN Exp SEMI {$$ = new NRetutnStmt(*$2); $$->line=$1;}
        | IF LP Exp RP Stmt %prec LOWER_THEN_ELSE   {$$ = new NIfStmt(*$3,*$5);$$->line=$1;}
        | IF LP Exp RP Stmt ELSE Stmt {$$ = new NIfElseStmt(*$3,*$5,*$7);$$->line=$1;}
        | WHILE LP Exp RP Stmt {$$ = new NWhileStmt(*$3,*$5);$$->line=$1;}
        | BREAK SEMI {$$ = new NBreakStmt();$$->line=$1;}
        ;

/*Local Definitions*/
DefList: { $$ = nullptr;}
        | Def DefList { $$ = new NDefList(*$1,$2);$$->line=$1->line;}
        ;
Def: Specifier DecList SEMI {$$ = new NDef(*$1,$2);$$->line=$1->line;}
        ;

DecList:  Dec  {$$ = new NDecList(*$1,nullptr);$$->line=$1->line;}
        | Dec COMMA DecList  {$$ = new NDecList(*$1,$3);$$->line=$1->line;}
        ;
Dec:      VarDec  {$$ = new NDec(*$1);$$->line=$1->line;}
        | VarDec ASSIGNOP Exp  {$$ = new NDec(*$1,$3);$$->line=$1->line;}
        ;

/*Expressions*/
Exp:      Exp ASSIGNOP Exp {$$ = new NAssignment("ASSIGNOP",*$1,0,*$3);$$->line=$1->line;}//$$结点type_id空置未用，正好存放运算符
        
        | Exp PLUSASS Exp   {$$ = new NAssignment("PLUSASS",*$1,0,*$3);$$->line=$1->line;}//复合赋值运算
        | Exp MINUSASS Exp   {$$ = new NAssignment("MINUSASS",*$1,0,*$3);$$->line=$1->line;}
        | Exp STARASS Exp   {$$ = new NAssignment("STARASS",*$1,0,*$3);$$->line=$1->line;}
        | Exp DIVASS Exp   {$$ = new NAssignment("ASSIGNOP",*$1,0,*$3);$$->line=$1->line;}

        | PLUSPLUS Exp %prec UPLUSPLUS   {$$ = new NSingleOperator("PLUSPLUS",0,*$2);$$->line=yylineno;}//这里利用BISON %prec表示和UMINUS同优先级
        | MINUSMINUS Exp %prec UMINUSMINUS   {$$ = new NSingleOperator("MINUSMINUS",0,*$2);$$->line=yylineno;}//这里利用BISON %prec表示和UMINUS同优先级
        | Exp PLUSPLUS   {$$ = new NSingleOperator("PLUSPLUS",0,*$1);$$->line=$1->line;}//这里利用BISON %prec表示和UMINUS同优先级
        | Exp MINUSMINUS  {$$ = new NSingleOperator("MINUSMINUS",0,*$1);$$->line=$1->line;}//这里利用BISON %prec表示和UMINUS同优先级

        | Exp AND Exp   {$$ = new NBinaryOperator("AND",*$1,0,*$3);$$->line=$1->line;}
        | Exp OR Exp    {$$ = new NBinaryOperator("OR",*$1,0,*$3);$$->line=$1->line;}
        | Exp RELOP Exp {$$ = new NBinaryOperator("RELOP",*$1,0,*$3);$$->line=$1->line;}  //词法分析关系运算符号自身值保存在$2中
        | Exp PLUS Exp  {$$ = new NBinaryOperator("PLUS",*$1,0,*$3);$$->line=$1->line;}
        | Exp MINUS Exp {$$ = new NBinaryOperator("MINUS",*$1,0,*$3);$$->line=$1->line;}
        | Exp STAR Exp  {$$ = new NBinaryOperator("STAR",*$1,0,*$3);$$->line=$1->line;}
        | Exp DIV Exp   {$$ = new NBinaryOperator("DIV",*$1,0,*$3);$$->line=$1->line;}
        
        | Exp MOD Exp   {$$ = new NBinaryOperator("MOD",*$1,0,*$3);$$->line=$1->line;}
        
        | LP Exp RP     {$$ = new NParenOperator(*$2);$$->line=yylineno;}
        | MINUS Exp %prec UMINUS   {$$ = new NSingleOperator("MINUS",0,*$2);$$->line=yylineno;}//这里利用BISON %prec表示和UMINUS同优先级 相当于虚拟出一个运算符
        | NOT Exp       {$$ = new NSingleOperator("NOT",0,*$2);$$->line=yylineno;}
        | ID LP Args RP {NIdentifier* temp = new NIdentifier($1);$$ = new NMethodCall(*temp,$3);$$->line=yylineno;}
        | ID LP RP      {NIdentifier* temp = new NIdentifier($1);$$ = new NMethodCall(*temp);$$->line=yylineno;}
        | Exp LB Exp RB {$$ = new NListOperator(*$1,*$3);$$->line=$1->line;}
        | Exp DOT ID    {NIdentifier* temp = new NIdentifier($3);$$ = new NDotOperator(*$1,*temp);$$->line=$1->line;}
        | ID            {$$ = new NIdentifier($1);$$->line=yylineno;}
        | INT           {$$ = new NInteger($1);$$->line=yylineno;}
        | FLOAT         {$$ = new NFloat($1);$$->line=yylineno;}
        | CHAR          {$$ = new NChar($1);$$->line=yylineno;}
        ;
Args:   Exp COMMA Args {$$ = new NArgs(*$1,$3);$$->line=$1->line;}
        | Exp {$$ = new NArgs(*$1,nullptr);$$->line=$1->line;}
        ;

%%
// int main(int argc, char *argv[]){
// 	yyin=fopen(argv[1],"r");
// 	if (!yyin) return 1;
// 	yylineno=1;
//         allptr=NULL;
//         allerror=0;
// 	yyparse();
//         if(p) p->parse();
//         if(!allerror)
//         semanticanalysis(allptr,0);
//         if(!allerror){
//         //std::string temp=argv[1];
//         //temp[temp.size()-1]='l';
//         //temp+="l";
//         //FILE *tt=freopen(temp.data(),"w",stdout);
//         //codeGen(allptr,0);
//         //fclose(tt);
//         //toy_as(temp);
//         //char argvv[2][10];
//         //strcpy(argvv[0],"llc");
//         //temp[temp.size()-2]='b';
//         //temp[temp.size()-1]='c';
//         //strcpy(argvv[1],temp.data());
//         //toy_llc(2,(char **)argvv);
//         }
// 	return 0;
// }

#include<stdarg.h>
void yyerror(const char* fmt, ...)
{
    va_list ap;//取参数对应的函数
    va_start(ap, fmt);
    fprintf(stderr, "Grammar Error at Line %d Column %d: ", yylloc.first_line,yylloc.first_column);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr,"%s",myerror);
    fprintf(stderr,"%s", ".\n");
    memset(myerror,0,sizeof(myerror));
}	
void myyyerror()
{
    fprintf(stderr, "Grammar Error at Line %d Column %d: ", yylloc.first_line,yylloc.first_column);
    fprintf(stderr,"%s",myerror);
    fprintf(stderr, "%s",".\n");
    memset(myerror,0,sizeof(myerror));
}