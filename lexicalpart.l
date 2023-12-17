%{
    #include<stdio.h>
    #include"syntaxpart.tab.h"
%}
%option yylineno
letter[A-Za-z]
digit [0-9]
del [ \t\n]

%%

"<?php"                           {return(PhpStart);}
"?>"                              {return(PhpEnd);}
[Ii][Ff]                          {return(IF);}
[Ee][Ll][Ss][Ee]                  {return(ELSE);}
[Ee][Cc][Hh][Oo]                  {return(ECHO_T);} 
"function"                        {return(FUNC);}
[Ww][Hh][Ii][Ll][Ee]              {return(WHILE);}
[Cc][Ll][Aa][Ss][Ss]              {return(CLASS_T);}
{letter}*                         {;}
"$"_?{letter}({letter}|{digit})*  {return(ID);}
{digit}+                          {return(INT);}
{digit}+"."{digit}*               {return(FLOAT);}
'[^\']'                           {;}
\"[^\"]*\"                        {return(STRING);}
"["|"{"|"("|")"|"}"|"]"|";"       {return yytext[0];}
("//"|"#").*                      {;}
"/*"(.|\n)*"*/"                        {;}
"<"|">"|"="|"*"|"+"|"/"|"-"|"%"|"."  {return yytext[0];}
"<="                              {return(LE);}
">="                              {return(GE);}
"=="                              {return(ET);}
"!="                              {return(NE);}
"**"                              {return(POW);}
{del}*               
. {;}                                 

%%
int yywrap(void){}
int main(){

    /*FILE *inputptr,*tokenptr,*identifierptr;
    inputptr = fopen("source.txt", "a");
    tokenptr = fopen("source.txt", "w");
    identifierptr = fopen("source.txt", "w");*/

    /* yyin points to the file input.txt 
    and opens it in read mode*/
    yyin = fopen("source.txt", "r");

    /* yyout points to the file output.txt 
    and opens it in write mode
    tptr = fopen("token.txt", "w");*/
    //yyout = fopen("token.txt", "w");
    yyparse();
    
    return 0;
}