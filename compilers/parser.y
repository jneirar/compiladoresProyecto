%{
#include <iostream>
#include <string>
#include <cmath>
#include <FlexLexer.h>
#include "Node.hpp"
int row = 1;
int col = 1;
Node* syntacticTree = nullptr;
%}

%require "3.5.1"
%language "C++"

%define api.parser.class {Parser}
%define api.namespace {utec::compilers}
%define api.value.type variant
%parse-param {FlexScanner* scanner} {int* result}

%code requires
{
    class Node;
    namespace utec::compilers {
        class FlexScanner;
    } // namespace utec::compilers
}

%code
{
    #include "FlexScanner.hpp"
    #define yylex(x) scanner->lex(x)
}

%start	input 

%token <Node*> ENTERO 
%token <Node*> RETORNO SIN_TIPO MIENTRAS SI SINO MAIN
%token <Node*> ID NUM
%token <Node*> ADDOP SUBOP MULOP DIVOP
%token <Node*> RELOP
%token <Node*> IGUAL
%token SEMICOLON COLON OPPAR CLPAR OPCOR CLCOR OPBRA CLBRA

%type <Node*> input programa lista_declaracion 
%type <Node*> declaracion declaracion_fact
%type <Node*> params sent_compuesta
%type <Node*> var_declaracion var_declaracion_p
%type <Node*> tipo lista_params param param_p
%type <Node*> declaracion_local lista_sentencias
%type <Node*> sentencia sentencia_expresion sentencia_seleccion sentencia_seleccion_p sentencia_iteracion sentencia_retorno sentencia_retorno_p
%type <Node*> expresion expresion_simple expresion_simple_p var var_p
%type <Node*> expresion_aditiva term addope mulope factor 
%type <Node*> call args lista_arg
%%

input:		/* empty */ 
		| programa  { $$ = new Node("input", $1); $$->printPreorder(); syntacticTree = $$; }
		;

programa:  
    lista_declaracion { $$ = new Node("programa", $1); }
    ;
lista_declaracion:
    lista_declaracion declaracion { $$ = new Node("lista_declaracion", $1, $2); }
    |   declaracion { $$ = new Node("lista_declaracion", $1); }
    ;
declaracion:
    ENTERO ID declaracion_fact { $$ = new Node("declaracion", $1, $2, $3); }
    |   SIN_TIPO ID OPPAR params CLPAR sent_compuesta { $$ = new Node("declaracion", $1, $2, $4, $6); }
    ;
declaracion_fact:
    var_declaracion_p { $$ = new Node("declaracion_fact", $1); }
    |   OPPAR params CLPAR sent_compuesta { $$ = new Node("declaracion_fact", $2, $4); }
    ;
var_declaracion:
    ENTERO ID var_declaracion_p { $$ = new Node("var_declaracion", $1, $2, $3); }
    ;
var_declaracion_p:
    SEMICOLON { $$ = new Node("var_declaracion_p"); }
    |   OPCOR NUM CLCOR SEMICOLON { $$ = new Node("var_declaracion_p", $2); }
    ;
tipo:
    ENTERO { $$ = new Node("tipo", $1); }
    |   SIN_TIPO { $$ = new Node("tipo", $1); }
    ;
params:
    lista_params { $$ = new Node("params", $1); }
    |   SIN_TIPO { $$ = new Node("params", $1); }
    ;
lista_params:
    lista_params COLON param { $$ = new Node("lista_params", $1, $3); }
    |   param { $$ = new Node("lista_params", $1); }
    ;
param:
    tipo ID param_p { $$ = new Node("param", $1, $2, $3); }
    ;
param_p:
    OPCOR CLCOR { $$ = new Node("param_p"); }
    |   /* vacío */ { $$ = new Node("param_p"); }
    ;
sent_compuesta:
    OPBRA declaracion_local lista_sentencias CLBRA { $$ = new Node("sent_compuesta", $2, $3); }
    ;
declaracion_local:
    var_declaracion declaracion_local { $$ = new Node("declaracion_local", $1, $2); }
    |   /* vacío */ { $$ = new Node("declaracion_local"); }
    ;
lista_sentencias:
    lista_sentencias sentencia { $$ = new Node("lista_sentencias", $1, $2); }
    |   /* vacío */ { $$ = new Node("lista_sentencias"); }
    ;
sentencia:
    sentencia_expresion { $$ = new Node("sentencia", $1); }
    |   sentencia_seleccion { $$ = new Node("sentencia", $1); }
    |   sentencia_iteracion { $$ = new Node("sentencia", $1); }
    |   sentencia_retorno { $$ = new Node("sentencia", $1); }
    ;
sentencia_expresion:
    expresion SEMICOLON { $$ = new Node("sentencia_expresion", $1); }
    |   SEMICOLON { $$ = new Node("sentencia_expresion"); }
    ;
sentencia_seleccion:
    SI OPPAR expresion CLPAR sentencia sentencia_seleccion_p { $$ = new Node("sentencia_seleccion", $1, $3, $5, $6); }
    ;
sentencia_seleccion_p: 
    SINO sentencia { $$ = new Node("sentencia_seleccion_p", $1, $2); }
    |   /* vacío */ { $$ = new Node("sentencia_seleccion_p"); }
    ;
sentencia_iteracion:
    MIENTRAS OPPAR expresion CLPAR OPBRA lista_sentencias CLBRA { $$ = new Node("sentencia_iteracion", $1, $3, $6); }
    ;
sentencia_retorno:
    RETORNO sentencia_retorno_p { $$ = new Node("sentencia_retorno", $1, $2); }
    ;
sentencia_retorno_p:
    SEMICOLON { $$ = new Node("sentencia_retorno_p"); }
    |   expresion SEMICOLON { $$ = new Node("sentencia_retorno_p", $1); }
    ;
expresion:
    var IGUAL expresion { $$ = new Node("expresion", $1, $2, $3); }
    |   expresion_simple { $$ = new Node("expresion", $1); }
    ;
var:
    ID var_p { $$ = new Node("var", $1, $2); }
    ;
var_p:
    OPCOR expresion CLCOR { $$ = new Node("var_p", $2); }
    |   /* vacío */ { $$ = new Node("var_p"); }
    ;
expresion_simple:
    expresion_aditiva expresion_simple_p { $$ = new Node("expresion_simple", $1, $2); }
    ;
expresion_simple_p:
    RELOP expresion_aditiva { $$ = new Node("expresion_simple_p", $1, $2); }
    |   /* vacío */ { $$ = new Node("expresion_simple_p"); }
    ;
expresion_aditiva:
    expresion_aditiva addope term { $$ = new Node("expresion_aditiva", $1, $2, $3); }
    |   term { $$ = new Node("expresion_aditiva", $1); }
    ;
addope:
    ADDOP { $$ = new Node("addope", $1); }
    |   SUBOP { $$ = new Node("addope", $1); }
    ;
term:
    term mulope factor { $$ = new Node("term", $1, $2, $3); }
    |   factor { $$ = new Node("term", $1); }
    ;
mulope:
    MULOP { $$ = new Node("mulope", $1); }
    |   DIVOP { $$ = new Node("mulope", $1); }
    ;
factor:
    OPPAR expresion CLPAR { $$ = new Node("factor", $2); }
    |   var { $$ = new Node("factor", $1); }
    |   call { $$ = new Node("factor", $1); }
    |   NUM { $$ = new Node("factor", $1); }
    ;
call:
    ID OPPAR args CLPAR { $$ = new Node("call", $1, $3); }
    ;
args:
    lista_arg { $$ = new Node("args", $1); }
    |   /* vacío */ { $$ = new Node("args"); }
    ;
lista_arg:
    lista_arg COLON expresion { $$ = new Node("lista_arg", $1, $3); }
    |   expresion { $$ = new Node("lista_arg", $1); }
    ;

%%
void utec::compilers::Parser::error(const std::string& msg) {
    std::cerr << msg << " " /*<< yylineno*/ <<'\n';
}
