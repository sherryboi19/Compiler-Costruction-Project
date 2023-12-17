%{
#include <stdio.h>
#include <stdlib.h>

extern FILE *outfile;

%}

%start program

%union {
    int num;
    char *str;
    float fnum;
    char ch;
}

%token INTEGER FLOAT CHARACTER STRING IDENTIFIER
%token OPERATOR PUNCTUATION
%token KW_BEGIN KW_END KW_IF KW_ELSE KW_WHILE KW_ECHO KW_FUNCTION KW_CLASS
%token KW_RETURN KW_VAR KW_TRUE KW_FALSE

%%

program : KW_BEGIN statements KW_END
       ;

statements : statement ';'
           | statements statement ';'
           ;

statement : variable_declaration
          | assignment_statement
          | if_statement
          | while_statement
          | echo_statement
          | function_declaration
          | class_declaration
          ;

variable_declaration : KW_VAR IDENTIFIER '=' expression ';'
                     | KW_VAR IDENTIFIER ';'
                     ;

assignment_statement : IDENTIFIER OPERATOR expression ';'
                     ;

expression : INTEGER
           | FLOAT
           | CHARACTER
           | STRING
           | IDENTIFIER
           | expression OPERATOR expression
           | '(' expression ')'
           | KW_TRUE
           | KW_FALSE
           ;

if_statement : KW_IF '(' expression ')' '{' statements '}'
               | KW_IF '(' expression ')' '{' statements '}' KW_ELSE '{' statements '}'
               ;

while_statement : KW_WHILE '(' expression ')' '{' statements '}'
                 ;

echo_statement : KW_ECHO expression ';'
                | KW_ECHO STRING ';'
                ;

function_declaration : KW_FUNCTION IDENTIFIER '(' parameter_list ')' '{' statements KW_RETURN expression ';''}' 
                     ;

parameter_list : IDENTIFIER
                | parameter_list ',' IDENTIFIER
                ;

class_declaration : KW_CLASS IDENTIFIER '{' member_declaration '}'
                  ;

member_declaration : variable_declaration
                   | function_declaration
                   ;

%%

int yyerror(const char *s) 
{
  printf("Error: %s\n", s);
  return 0;
}

int main() 
{
  outfile = fopen("token.txt", "w");
  yylex();
  fclose(outfile);

  outfile = fopen("Identifier.txt", "w");
  yyparse();
  fclose(outfile);

  return 0;
}