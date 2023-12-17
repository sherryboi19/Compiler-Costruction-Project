%{
    #include<stdio.h>
    #include<stdlib.h>
    extern int yylineno;
%}
%token PhpStart PhpEnd ID INT FLOAT STRING LE GE ET NE POW IF ELSE FUNC WHILE ECHO_T CLASS_T

%%

ProgramBlock: PhpStart StatementList PhpEnd  {printf("Code Compiled Successfully");  exit(0);} 

StatementList : Statement StatementList 
              | 
              ;

Statement: DecStmt 
          | BoolExp
          | RepStmt
          | DecStruct
          | OutStmt
          | FuncStmt
          | ClassStmt; 
          

DecStmt: ID '=' RHS ';';
RHS: Value OP RHS
   | Value;
Value: INT 
     | FLOAT 
     | STRING 
     | ID;
OP: '+'
  | '-'
  | '*'
  | POW
  | '/';


BoolExp: Exp '>' Exp  { $$ = $1 > $3; }
       | Exp '<' Exp  { $$ = $1 < $3; }
       | Exp ET Exp   { $$ = $1 == $3; }
       | Exp NE Exp   { $$ = $1 != $3; }
       | Exp GE Exp   { $$ = $1 >= $3; }
       | Exp LE Exp   { $$ = $1 <= $3; }
       ;
Exp: INT 
    |ID;       


RepStmt: WHILE '(' BoolExp ')' '{' StatementList '}';


DecStruct: IF '(' BoolExp ')' '{' StatementList '}'
         | IF '(' BoolExp ')' '{' StatementList '}' ELSE '{' StatementList '}';
 

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


FuncStmt: FUNC ID '(' ')' '{' StatementList '}';

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
  printf("Code cannot be compiled\n");
  printf("\033[31mERROR %s AT LINE %d\n\033[0m",str,yylineno);
}