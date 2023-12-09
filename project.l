%{
#include "project.tab.h"
#include <string.h>

%}

%option noyywrap

%%
[ \t\n]                   ; // Skip whitespace characters
[0-9]+                    { yylval.num = atoi(yytext); return INTEGER; }
\$[a-zA-Z_][a-zA-Z0-9_]*  { yylval.str = strdup(yytext); return IDENTIFIER; }
"=="                      { return EQ; }
"!="                      { return NE; }
">="                      { return GE; }
"<="                      { return LE; }
">"                       { return GT; }
"<"                       { return LT; }
"="                       { return ASSIGN; }
"+"                       { return PLUS; }
"-"                       { return MINUS; }
"*"                       { return MULTIPLY; }
"/"                       { return DIVIDE; }
"**"                      { return POWER; }
";"                       { return SEMICOLON; }
"{"                       { return LEFT_BRACE; }
"}"                       { return RIGHT_BRACE; }
"("                       { return LEFT_PAREN; }
")"                       { return RIGHT_PAREN; }
","                       { return COMMA; }
"\?"                      { return QUESTION_MARK; }
"echo"                    { return ECHO; }
"if"                      { return IF; }
"else"                    { return ELSE; }
"while"                   { return WHILE; }
"function"                { return FUNCTION; }
"class"                   { return CLASS; }
"<?php"                   { return PHP_OPEN; }
"?>"                      { return PHP_CLOSE; }
\"[^"]*\"                 { yylval.str = strdup(yytext); return STRING_LITERAL; }
.                         ; // Ignore any other characters

%%

int yywrap() 
{
    return (1);
}
