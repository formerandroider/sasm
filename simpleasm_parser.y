%{
    #include <string.h>
    #include "lex.yy.h"
    
    void yyerror(char const *estring) {
        fprintf(stderr, "%s\n", estring);
        exit(1);
    }
%}

%code requires {
    #include "simpleasm.h"
}

%define parse.error verbose
%define api.value.type union

%left OPERATION
%token <int> ADDRESS "T_ADDRESS"
%token <int> NUM "T_NUM"
%token <char*> IDENTIFIER "T_IDENTIFIER"
%token <char*> OPERATION "T_OPERATION"
%token <char*> LABEL "T_LABEL"
%type <unsigned int*> address
%type <unsigned int*> operation_arg
%type <operation*> operation_call

%%

input:
    %empty
  | input operation_line
 ;

operation_line:
    LABEL operation_call { add_operation_label($1, $2); }
  | operation_call { add_operation($1); }
 ;

operation_call:
    OPERATION operation_arg {
        operation *op = malloc(sizeof(operation));
        op->operation = malloc(sizeof(char) * (strlen($1) + 1));
        sprintf(op->operation, "%s", $1);
        free($1);
        op->address = $2;

        $$ = op;
    }
 ;

operation_arg: %empty { $$ = 0; } | address | NUM { unsigned int *tmp = malloc(sizeof(unsigned int)); *tmp = $1; $$ = tmp; };

address: ADDRESS { unsigned int *tmp = malloc(sizeof(unsigned int)); *tmp = $1; $$ = tmp; } | IDENTIFIER { $$ = add_or_find_label_address($1); } ; // TODO: Identifier dereference
