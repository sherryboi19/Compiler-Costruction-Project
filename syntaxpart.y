%{
    #include<stdio.h>
    #include<stdlib.h>
    extern int yylineno;
%}
%token PhpStart PhpEnd ID INT FLOAT STRING LE GE ET NE POW IF ELSE FUNC WHILE ECHO_T CLASS_T FNAME_T

%%

ProgramBlock: PhpStart StatementList PhpEnd  {printf("\033[32mCode Compiled Successfully\033[0m");  exit(0);} 
;

StatementList : Statement StatementList 
              | 
;

Statement: DecStmt 
          | BoolExp
          | RepStmt
          | DesStmt
          | OutStmt
          | FuncStmt
          | ClassStmt
          | FunCall
; 
          

DecStmt: ID '=' RHS ';'
;

RHS: Value OP RHS
   | Value
   | BoolExp
;

Value: INT 
     | FLOAT 
     | STRING 
     | ID
;

OP: '+'
  | '-'
  | '*'
  | POW
  | '/'
;

BoolExp: Exp '>' Exp 
       | Exp '<' Exp  
       | Exp ET Exp   
       | Exp NE Exp   
       | Exp GE Exp  
       | Exp LE Exp  
;

Exp: INT 
    |ID
    |FLOAT
;       

RepStmt: WHILE '(' BoolExp ')' '{' StatementList '}'
;


DesStmt: IF '(' BoolExp ')' '{' StatementList '}'
         | IF '(' BoolExp ')' '{' StatementList '}' ELSE '{' StatementList '}'
;
 

OutStmt: ECHO_T Output OutputP ';'
;

OutputP: '.' Output OutputP
        | 
;

Output: STRING
      | ID 
      | INT
      | FLOAT
;


FuncStmt: FUNC FNAME_T '(' Args ')' '{' StatementList '}'
;

Args : Arg ',' Args
     | Arg
     | 
;

FunCall : FNAME_T '(' Args ')' ';'
;

Arg: ID
    | ID '=' Value
;


ClassStmt: CLASS_T ID '{' ClassStatements '}';

ClassStatements: ClassStatement ClassStatements
               | 
;

ClassStatement: DecStmt
              | FuncStmt
;


%%
void yyerror(const char *str)
{
  printf("\033[31mCode Cannot be Compiled!\033[0m");
  //printf("\033[31mERROR %s AT LINE %d\033[0m",str,yylineno);
}