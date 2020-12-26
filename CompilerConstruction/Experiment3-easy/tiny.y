%{#include <ctype.h>
#include<stdio.h>
#define MSDOS
int linecount;
extern int yylex();
extern int yyerror();
%}
%union{
   char chr;
   char *str;
   int integer;
   float real;
   double dbl;
   }
%token number
%type <real> expr number
%left '+' '-'
%left '*' '/'
%right uminus
%%
lines: lines expr'\n'
 {
  printf("line %d:%g\n",linecount++,$2);
 }
 |lines'\n'
 {
  linecount++;
 }
 |
 ;
expr:   expr'+'expr
 {
  $$=$1+$3;
 }
 |expr '-' expr
 {
  $$=$1-$3;
 }
 |expr '*' expr
 {
  $$=$1*$3;
 }
 | expr '/' expr
 {
  $$=$1/$3;
 }
 | '(' expr ')'
 {
  $$=$2;
 }
 | '-' expr %prec uminus
 {
  $$=-$2;
 }
 | number
 ;
%%
int yyerror(s)
char *s;
{
 fprintf(stderr,"syntactic error:%s\n",s);
 return 0;
}