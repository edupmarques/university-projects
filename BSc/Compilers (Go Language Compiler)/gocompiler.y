/*
Eduardo Marques 2022231584
João Cardoso 2022222301
*/

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
int yylex(void);
void yyerror(char *);
struct node *program;

%}

%token<lexeme> PACKAGE IDENTIFIER SEMICOLON VAR INT FLOAT32 BOOL STR COMMA LPAR RPAR LBRACE RBRACE FUNC IF ELSE ASSIGN FOR RETURN PRINT STRLIT NATURAL DECIMAL
%token<lexeme> OR AND LT GT EQ NE LE GE PLUS MINUS STAR DIV MOD NOT
%token<lexeme> BLANKID PARSEINT CMDARGS LSQ RSQ 
%type<node> Program Declarations VarDeclaration VarSpec_aux FuncDeclaration FuncBody VarsAndStatements Statement Statement_aux ParseArgs FuncInvocation FuncInvocation_aux Parameters Parameters_aux Param_Decl Expr Type

%left COMMA
%right ASSIGN 
%left OR
%left AND
%left EQ NE LT GT LE GE
%left PLUS MINUS
%left STAR DIV MOD
%right NOT

%union{
    char *lexeme;
    struct node *node;
}

/* START grammar rules section -- BNF grammar */

%%

Program:
      PACKAGE IDENTIFIER SEMICOLON Declarations                               { $$ = program = new_node("Program", NULL); add_childs(program, 1, $4); }
    ;

Declarations:
      VarDeclaration SEMICOLON Declarations                                   { $$ = $1; if ($3) add_sibling($$, $3); }
    | FuncDeclaration SEMICOLON Declarations                                  { $$ = $1; if ($3) add_sibling($$, $3); }
    |                                                                         { $$ = NULL; }
    ;

VarDeclaration:
      VAR IDENTIFIER VarSpec_aux Type                                         { $$ = new_node("VarDecl", NULL); add_childs($$, 2, $4, new_node("Identifier",$2)); if ($3) { organize_childs($3,$4); add_sibling($$, $3); }; }
    | VAR LPAR IDENTIFIER VarSpec_aux Type SEMICOLON RPAR                     { $$ = new_node("VarDecl", NULL); add_childs($$, 2, $5, new_node("Identifier",$3)); if ($4) { organize_childs($4,$5); add_sibling($$, $4); }; }
    ;

VarSpec_aux:
      COMMA IDENTIFIER VarSpec_aux                                            { $$ = new_node("VarDecl", NULL); add_childs($$, 1, new_node("Identifier",$2)); if ($3) add_sibling($$, $3); }
    |                                                                         { $$ = NULL; }
    ;
  
FuncDeclaration:
      FUNC IDENTIFIER LPAR RPAR FuncBody                                      { $$ = new_node("FuncDecl", NULL); node *aux = new_node("FuncHeader", NULL); add_childs(aux, 2, new_node("Identifier", $2), new_node("FuncParams", NULL)); add_childs($$, 2, aux, $5); }
    | FUNC IDENTIFIER LPAR Parameters RPAR FuncBody                           { $$ = new_node("FuncDecl", NULL); node *aux = new_node("FuncHeader", NULL); add_childs(aux, 2, new_node("Identifier", $2), $4); add_childs($$, 2, aux, $6); }
    | FUNC IDENTIFIER LPAR Parameters RPAR Type FuncBody                      { $$ = new_node("FuncDecl", NULL); node *aux = new_node("FuncHeader", NULL); add_childs(aux, 3, new_node("Identifier", $2), $6, $4); add_childs($$, 2, aux, $7); }
    | FUNC IDENTIFIER LPAR RPAR Type FuncBody                                 { $$ = new_node("FuncDecl", NULL); node *aux = new_node("FuncHeader", NULL); add_childs(aux, 3, new_node("Identifier", $2), $5, new_node("FuncParams", NULL)); add_childs($$, 2, aux, $6); }
    ;

FuncBody:
      LBRACE VarsAndStatements RBRACE                                         { $$ = new_node("FuncBody",NULL); if ($2) add_childs($$, 1, $2); }
    ;
    
Parameters:
      Param_Decl Parameters_aux                                               { $$  = new_node("FuncParams", NULL); if ($1) add_childs($$, 2, $1, $2); }
    ;

Parameters_aux:
      COMMA Param_Decl Parameters_aux                                         { $$ = $2; if ($3) add_sibling($$, $3); } 
    |                                                                         { $$ = NULL; }
    ; 

Param_Decl:
      IDENTIFIER Type                                                         { $$ = new_node("ParamDecl",NULL); add_childs($$, 2, $2, new_node("Identifier", $1)); }                     
    ;

VarsAndStatements:
      VarDeclaration SEMICOLON VarsAndStatements                              { if ($$) { $$ = $1; if ($3) add_sibling($$, $3);} else { $$ = $3; }; }
    | Statement SEMICOLON VarsAndStatements                                   { if ($$) { $$ = $1; if ($3) add_sibling($$, $3);} else { $$ = $3; }; }
    | SEMICOLON VarsAndStatements                                             { $$ = $2; }
    |                                                     	                  { $$ = NULL; }
    ;

Statement:
      IDENTIFIER ASSIGN Expr                                                  { $$ = new_node("Assign",NULL); add_childs($$, 2, new_node("Identifier", $1), $3); }
    | LBRACE Statement_aux RBRACE                                             { if ($2) { if ( count_siblings($2) >= 2 ){ $$ = new_node("Block", NULL); add_childs($$, 1, $2); } else { $$ = $2; }} else { $$ = NULL; }; }
    | IF Expr LBRACE Statement_aux RBRACE                                     { $$ = new_node("If",NULL); node* aux = new_node("Block",NULL); if ($4) add_childs(aux, 1 , $4); node* aux1 = new_node("Block",NULL); if ($2) add_childs($$, 3, $2, aux, aux1); }
    | IF Expr LBRACE Statement_aux RBRACE ELSE LBRACE Statement_aux RBRACE    { $$ = new_node("If",NULL); node* aux = new_node("Block",NULL); if ($4) add_childs(aux, 1 , $4); node* aux1 = new_node("Block",NULL); if ($8) add_childs(aux1, 1, $8); if ($2) add_childs($$, 3, $2, aux, aux1);}
    | FOR LBRACE Statement_aux RBRACE                                         { $$ = new_node("For",NULL); node* aux = new_node("Block",NULL); add_childs(aux, 1 , $3); if ($2) add_childs($$, 1, aux); }
    | FOR Expr LBRACE Statement_aux RBRACE                                    { $$ = new_node("For",NULL); node* aux = new_node("Block",NULL); add_childs(aux, 1 , $4); if ($2) add_childs($$, 2, $2, aux); }
    | RETURN                                                                  { $$ = new_node("Return", NULL); }
    | RETURN Expr                                                             { $$ = new_node("Return", NULL); if ($2) add_childs($$, 1, $2); }
    | ParseArgs                                                               { $$ = new_node("ParseArgs", NULL); if ($1) add_childs($$, 1, $1); }
    | FuncInvocation                                                          { $$ = new_node("Call", NULL); if ($1) add_childs($$, 1, $1); }
    | PRINT LPAR Expr RPAR                                                    { $$ = new_node("Print", NULL); add_childs($$, 1, $3); }
    | PRINT LPAR STRLIT RPAR                                                  { $$ = new_node("Print", NULL); add_childs($$, 1, new_node("StrLit", $3)); }
    | error                                                                   { $$ = NULL; }
    ;
  
  Statement_aux:
      Statement SEMICOLON Statement_aux                                       { if ($$) { $$ = $1; if ($3) add_sibling($$, $3); } else {$$ = $3; }; }
    |                                                                         { $$ = NULL; }
    ;
  
  ParseArgs:
      IDENTIFIER COMMA BLANKID ASSIGN PARSEINT LPAR CMDARGS LSQ Expr RSQ RPAR { $$ = new_node("Identifier", $1); add_sibling($$, $9); }
    | IDENTIFIER COMMA BLANKID ASSIGN PARSEINT LPAR error RPAR                { $$ = NULL; }
    ;

  FuncInvocation:
      IDENTIFIER LPAR RPAR                                                    { $$ = new_node("Identifier", $1); }
    | IDENTIFIER LPAR Expr FuncInvocation_aux RPAR                            { $$ = new_node("Identifier", $1); if ($3) add_sibling($$, $3); if ($4) add_sibling($$, $4); }
    | IDENTIFIER LPAR error RPAR                                              { $$ = NULL; } 
    ;

FuncInvocation_aux:
      COMMA Expr FuncInvocation_aux                                           { $$ = $2; add_sibling($$, $3); }
    |                                                                         { $$ = NULL; }
     ;

Expr:
      Expr OR Expr                                                            { $$ = new_node("Or", NULL); add_childs($$, 2, $1, $3); }
    | Expr AND Expr                                                           { $$ = new_node("And", NULL); add_childs($$, 2, $1, $3); }
    | Expr LT Expr                                                            { $$ = new_node("Lt", NULL); add_childs($$, 2, $1, $3); }
    | Expr GT Expr                                                            { $$ = new_node("Gt", NULL); add_childs($$, 2, $1, $3); }
    | Expr EQ Expr                                                            { $$ = new_node("Eq", NULL); add_childs($$, 2, $1, $3); }
    | Expr NE Expr                                                            { $$ = new_node("Ne", NULL); add_childs($$, 2, $1, $3); }
    | Expr LE Expr                                                            { $$ = new_node("Le", NULL); add_childs($$, 2, $1, $3); }
    | Expr GE Expr                                                            { $$ = new_node("Ge", NULL); add_childs($$, 2, $1, $3); }
    | Expr PLUS Expr                                                          { $$ = new_node("Add", NULL); add_childs($$, 2, $1, $3); }
    | Expr MINUS Expr                                                         { $$ = new_node("Sub", NULL); add_childs($$, 2, $1, $3); }
    | Expr STAR Expr                                                          { $$ = new_node("Mul", NULL); add_childs($$, 2, $1, $3); }
    | Expr DIV Expr                                                           { $$ = new_node("Div", NULL); add_childs($$, 2, $1, $3); }
    | Expr MOD Expr                                                           { $$ = new_node("Mod", NULL); add_childs($$, 2, $1, $3); }
    | NOT Expr %prec NOT                                                      { $$ = new_node("Not", NULL); add_childs($$, 1, $2); } 
    | MINUS Expr %prec NOT                                                    { $$ = new_node("Minus", NULL); add_childs($$, 1, $2); } 
    | PLUS Expr %prec NOT                                                     { $$ = new_node("Plus", NULL); add_childs($$, 1, $2); } 
    | NATURAL                                                                 { $$ = new_node("Natural", $1); }
    | DECIMAL                                                                 { $$ = new_node("Decimal", $1); }
    | IDENTIFIER                                                              { $$ = new_node("Identifier", $1); }
    | FuncInvocation                                                          { $$ = new_node("Call", NULL); add_childs($$, 1, $1); }
    | LPAR Expr RPAR                                                          { $$ = $2; }
    | LPAR error RPAR                                                         { $$ = NULL; }
    ;

Type:
      INT                                                                     { $$ = new_node("Int", NULL); }
    | FLOAT32                                                                 { $$ = new_node("Float32", NULL); }
    | BOOL                                                                    { $$ = new_node("Bool", NULL); }
    | STR                                                                     { $$ = new_node("String", NULL); }
    ;


%%

/* START subroutines section */

// all needed functions are collected in the .l and ast.* files