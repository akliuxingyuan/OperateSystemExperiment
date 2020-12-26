%{
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#define false 0
#define ture 1
#include "yacc.tab.h"
extern int lexverbose;
extern int linecount;
%}
digit [0-9]
letter [a-zA-Z]
%%
{digit}+   {
    yylval.real=(float)atof(yytext);
    if(lexverbose)
        printf("real:%g\n",yylval.real);
    return(number);
    }
\+   {
    yylval.chr=yytext[0];
    if(lexverbose)
        printf("opterator:%c\n",yylval.chr);
    return('+');
    }
\-   {
    yylval.chr=yytext[0];
    if(lexverbose)
        printf("oprator:%c\n",yylval.chr);
    return('-');
    }
\*   {
    yylval.chr=yytext[0];
    if(lexverbose)
    printf("oprator:%c\n",yylval.chr);
    return('*');
    }
\/   {
    yylval.chr=yytext[0];
    if(lexverbose)
        printf("oprator:%c\n",yylval.chr);
    return('/');
    }
"("   {
    yylval.chr=yytext[0];
    if(lexverbose)
        printf("separator:%c\n",yylval.chr);
    return('(');
    }
")"   {
    yylval.chr=yytext[0];
    if(lexverbose)
        printf("separtor:%c\n",yylval.chr);
    return(')');
    }
;   {
    return(';');
    }
\n   {
    printf("line %d\n",linecount);
    return('\n');
   }
[ \t]+                  {
    printf("lexical analyzer error\n");
    }
quit  {
    printf("Bye!\n");
    exit(0);
    }
%%
int yywrap()
{
    return(1);
}