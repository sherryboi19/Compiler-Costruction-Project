%{
#include <stdio.h>
#include <stdlib.h>
extern int yylineno;  // Declare yylineno here
void yyerror(const char *s);  // Declare yyerror here
%}

%union {
    int num;
    char *str;
}

%token EQ NE GE LE GT LT ASSIGN PLUS MINUS MULTIPLY DIVIDE POWER SEMICOLON LEFT_BRACE RIGHT_BRACE LEFT_PAREN RIGHT_PAREN QUESTION_MARK ECHO IF ELSE WHILE PHP_CLOSE COMMA CLASS FUNCTION PHP_OPEN

%token <num> INTEGER
%token <str> IDENTIFIER STRING_LITERAL

%%
program:
    PHP_OPEN statements PHP_CLOSE
    {
        printf("Code Compiled Successfully\n");
        exit(0);
    }
    ;

statements:
    /* empty */
    | statements statement
    ;

statement:
    assignment_statement
    | bool_expression_statement
    | repetition_statement
    | decision_structure
    | output_statement
    | function_statement
    | class_definition
    ;

assignment_statement:
    IDENTIFIER ASSIGN expr SEMICOLON
    ;

bool_expression_statement:
    expr SEMICOLON
    ;

repetition_statement:
    WHILE LEFT_PAREN expr RIGHT_PAREN LEFT_BRACE statements RIGHT_BRACE
    ;

decision_structure:
    if_statement
    | if_else_statement
    ;

if_statement:
    IF LEFT_PAREN expr RIGHT_PAREN LEFT_BRACE statements RIGHT_BRACE
    ;

if_else_statement:
    IF LEFT_PAREN expr RIGHT_PAREN LEFT_BRACE statements RIGHT_BRACE ELSE LEFT_BRACE statements RIGHT_BRACE
    ;

output_statement:
    ECHO STRING_LITERAL SEMICOLON
    ;

function_statement:
    FUNCTION IDENTIFIER LEFT_PAREN parameter_list RIGHT_PAREN LEFT_BRACE statements RIGHT_BRACE
    ;

parameter_list:
    /* empty */
    | IDENTIFIER
    | IDENTIFIER COMMA parameter_list
    ;

class_definition:
    CLASS IDENTIFIER LEFT_BRACE assign_statements function_statements RIGHT_BRACE
    ;

assign_statements:
    /* empty */
    | assign_statement assign_statements
    ;

assign_statement:
    IDENTIFIER ASSIGN expr SEMICOLON
    ;

function_statements:
    /* empty */
    | function_statement function_statements
    ;

expr:
    INTEGER
    | IDENTIFIER
    | expr PLUS expr
    | expr MINUS expr
    | expr MULTIPLY expr
    | expr DIVIDE expr
    | expr POWER expr
    | expr EQ expr
    | expr NE expr
    | expr GE expr
    | expr LE expr
    | expr GT expr
    | expr LT expr
    | LEFT_PAREN expr RIGHT_PAREN
    ;

%%

void yyerror(const char *s) {
    print("Line %d: %s\n", yylineno);
    printf("Code cannot be compiled\n");
    exit(1);
}

int main() {
    yyparse();
    return 0;
}
