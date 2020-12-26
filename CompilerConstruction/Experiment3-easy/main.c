/*
 * @Author: akliuxingyuan
 * @Description: main.c
 * @Date: 2020-12-23 10:47:46
 */
#include "lex.yy.c" 
#include "yacc.tab.c"
#include <stdlib.h>
#include <stdio.h>
int lexverbose=1;
extern int yyparse();
 
int main(int argc, char* argv[])
{
 extern FILE *yyin;
 
 printf("Compiling...!\n");
 if((yyin=fopen("input.txt","rt"))==NULL){
   perror("can not open file input.txt\n") ;
   exit(1);
  }
 if (yyparse()==1){
  fprintf(stderr,"parser error\n");
  exit(1);
 }
 printf("yyparse() completed successfully!\n");
 return 0;
}