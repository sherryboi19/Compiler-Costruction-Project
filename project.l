%{
#include <stdio.h>
#include <string.h>
#include "project.tab.h"

FILE *outfile;

%}

%option case-insensitive

%%

"<?php" { printf("Keyword: <?php\n"); yylval.str = strdup(yytext); return KW_BEGIN; }
"?>" { printf("Keyword: ?>\n"); yylval.str = strdup(yytext); return KW_END; }
"if" { printf("Keyword: if\n"); yylval.str = strdup(yytext); return KW_IF; }
"else" { printf("Keyword: else\n"); yylval.str = strdup(yytext); return KW_ELSE; }
"while" { printf("Keyword: while\n"); yylval.str = strdup(yytext); return KW_WHILE; }
"function" { printf("Keyword: function\n"); yylval.str = strdup(yytext); return KW_FUNCTION; }
"echo" { printf("Keyword: echo\n"); yylval.str = strdup(yytext); return KW_ECHO; }
"var" { printf("Keyword: var\n"); yylval.str = strdup(yytext); return KW_VAR; }
"class" { printf("Keyword: class\n"); yylval.str = strdup(yytext); return KW_CLASS; }
"return" { printf("Keyword: return\n"); yylval.str = strdup(yytext); return KW_RETURN; }
"true" { printf("Keyword: true\n"); yylval.str = strdup(yytext); return KW_TRUE; }
"false" { printf("Keyword: false\n"); yylval.str = strdup(yytext); return KW_FALSE; }

[\$][a-zA-Z_][a-zA-Z0-9_]* { printf("Identifier: %s\n", yytext); yylval.str = strdup(yytext); return IDENTIFIER; }
[0-9]+ { printf("Integer: %s\n", yytext); yylval.num = atoi(yytext); return INTEGER; }
[0-9]+\.[0-9]+ { printf("Float: %s\n", yytext); yylval.fnum = atof(yytext); return FLOAT; }
"." { printf("Character: %s\n", yytext); yylval.ch = yytext[0]; return CHARACTER; }
"['\"]"(.|\n)*"['\"]" { printf("String: %s\n", yytext); yylval.str = strdup(yytext); return STRING; }


[ \t\n] ;

\+|\-|\*|\/|\%|\*\*|\!|\=\=|\<\=|\>\=|\<\>|\=\>|\<|\> { printf("Operator: %s\n", yytext); return yytext[0]; }
\;|\,|\(|\)|\{|\}|\[|\] { printf("Punctuation: %s\n", yytext); return yytext[0]; }

"//".* { printf("Single-line comment: %s\n", yytext); }
"/*"(.|\n)*"*/" { printf("Multi-line comment: %s\n", yytext); }

.|\n { printf("Unknown character: %s\n", yytext);}

%%

int yywrap() {
  return 1;
}
