%{
    #include<stdio.h>
    #include<stdlib.h>
    #include<string.h>

    typedef struct {
        char name[100];
        char type[20];
        int line;
    } Symbol;

    Symbol symbolTable[100];
    int symbolCount = 0;

    void addToSymbolTable(char* name, char* type, int line) {
        strcpy(symbolTable[symbolCount].name, name);
        strcpy(symbolTable[symbolCount].type, type);
        symbolTable[symbolCount].line = line;
        symbolCount++;
    }

    void printSymbolTable() {
        printf("\nSymbol Table:\n");
        printf("| %-15s | %-15s | %-5s |\n", "Name", "Type", "Line");
        printf("---------------------------------\n");
        for (int i = 0; i < symbolCount; i++) {
            printf("| %-15s | %-15s | %-5d |\n", symbolTable[i].name, symbolTable[i].type, symbolTable[i].line);
        }
        printf("---------------------------------\n");
    }
%}

%token PhpStart PhpEnd ID FLOAT LE GE ET NE POW IF ELSE FUNC WHILE ECHO_T

%union {
    char* stringValue;
    int intValue;
}

%token <stringValue> STRING
%token <intValue> INT
%type <intValue> RHS Value Exp DecStmt 

%start ProgramBlock

%%

ProgramBlock: PhpStart StatementList PhpEnd  {
    printf("Code Compiled Successfully\n");
    printSymbolTable();
    exit(0);
} 

StatementList : Statement StatementList 
              | 
              ;

Statement: DecStmt 
          | BoolExp
          | RepStmt
          | DecStruct
          | OutStmt
          | FuncStmt; 

DecStmt: ID '=' RHS ';' {
    addToSymbolTable($1.intValue, "INT", yylineno);
};

RHS: Value OP RHS {
    // Perform type checking if necessary
}
   | Value;

Value: INT {
    $$ = $1.intValue;
}
     | FLOAT {
    // Handle FLOAT type and add to symbol table if needed
}
     | STRING {
    // Handle STRING type and add to symbol table if needed
}
     | ID {
    // Check if the variable exists in the symbol table
    int found = 0;
    for (int i = 0; i < symbolCount; i++) {
        if (strcmp(symbolTable[i].name, $1) == 0) {
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("Error: Variable '%s' used without declaration at line %d\n", $1, yylineno);
        exit(1);
    }
    $$ = $1.intValue;
};

OP: '+'
  | '-'
  | '*'
  | POW
  | '/';

BoolExp: Exp '>' Exp 
       | Exp '<' Exp 
       | Exp ET Exp 
       | Exp NE Exp 
       | Exp GE Exp 
       | Exp LE Exp 
       ;

Exp: INT;

RepStmt: WHILE '(' BoolExp ')' '{' StatementList '}';

DecStruct: IF '(' BoolExp ')' '{' StatementList '}'
         | IF '(' BoolExp ')' '{' StatementList '}' ELSE '{' StatementList '}';

OutStmt: ECHO_T Output ';' ;

Output: STRING
       | ID 
       | STRING '.' ID '.' STRING;

FuncStmt: FUNC ID '(' ')' '{' StatementList '}';

%%

void yyerror(const char *str)
{
    printf("Code cannot be compiled\n");
    fprintf(stderr,"Error type: %s\n",str);
    fprintf(stderr,"Line number: %d\n",yylineno);
    exit(1);
}
