%{
#include <iostream>
#include <string>
#include <cmath>
#include <FlexLexer.h>
%}

%require "3.5.1"
%language "C++"

%define api.parser.class {Parser}
%define api.namespace {utec::compilers}
%define api.value.type variant
%parse-param {FlexScanner* scanner} {int* result}

%code requires
{
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

%token ENTERO
%token RETORNO
%token SIN_TIPO
%token MIENTRAS
%token SI
%token SINO
%token MAIN

%token ID
%token NUM

%token ADDOP
%token SUBOP
%token MULOP
%token DIVOP

%token RELOP

%token IGUAL

%token SEMICOLON
%token COLON
%token OPPAR
%token CLPAR
%token OPCOR
%token CLCOR
%token OPBRA
%token CLBRA

%%

input:		/* empty */
		| programa  { *result = 0; }
		;

programa:  
    lista_declaracion
    ;
lista_declaracion: 
    declaracion lista_declaracion_p
    ;
lista_declaracion_p:
    declaracion lista_declaracion_p
    |   /* vacío */
    ;
declaracion:
    fun_declaracion 
    |   var_declaracion 
    ;
var_declaracion:
    ENTERO ID var_declaracion_p
    ;
var_declaracion_p:
    SEMICOLON
    |   OPCOR NUM CLCOR SEMICOLON
    ;
tipo:
    ENTERO
    |   SIN_TIPO
    ;
fun_declaracion:
    tipo ID OPPAR params CLPAR sent_compuesta
    ;
params:
    lista_params
    |   SIN_TIPO
    ;
lista_params:
    param lista_params_p
    ;
lista_params_p:
    COLON param lista_params_p
    |   /* vacío */
    ;
param:
    tipo ID param_p
    ;
param_p:
    OPCOR CLCOR
    |   /* vacío */
    ;
sent_compuesta:
    OPBRA declaracion_local lista_sentencias CLBRA
    ;
declaracion_local:
    var_declaracion declaracion_local
    |   /* vacío */
    ;
lista_sentencias:
    sentencia lista_sentencias
    |   /* vacío */
    ;
sentencia:
    sentencia_expresion
    |   sentencia_seleccion
    |   sentencia_iteracion
    |   sentencia_retorno
    ;
sentencia_expresion:
    expresion SEMICOLON
    |   SEMICOLON
    ;
sentencia_seleccion:
    SI OPPAR expresion CLPAR sentencia sentencia_seleccion_p
    ;
sentencia_seleccion_p:
    SINO sentencia
    |   /* vacío */
    ;
sentencia_iteracion:
    MIENTRAS OPPAR expresion CLPAR OPBRA lista_sentencias CLBRA
    ;
sentencia_retorno:
    RETORNO sentencia_retorno_p
    ;
sentencia_retorno_p:
    SEMICOLON
    |   expresion SEMICOLON
    ;
expresion:
    var IGUAL expresion
    |   expresion_simple
    ;
var:
    ID var_p
    ;
var_p:
    OPCOR expresion CLCOR
    |   /* vacío */
    ;
expresion_simple:
    expresion_aditiva expresion_simple_p
    ;
expresion_simple_p:
    RELOP expresion_aditiva
    |   /* vacío */
    ;
expresion_aditiva:
    term expresion_aditiva_p
    ;
expresion_aditiva_p:
    addope term expresion_aditiva_p
    |   /* vacío */
    ;
addope:
    ADDOP
    |   SUBOP
    ;
term:
    factor term_p
    ;
term_p:
    mulope factor term_p
    |   /* vacío */
    ;
mulope:
    MULOP
    |   DIVOP
    ;
factor:
    OPPAR expresion CLPAR
    |   var
    |   call
    |   NUM
    ;
call:
    ID OPPAR args CLPAR
    ;
args:
    lista_arg
    |   /* vacío */
    ;
lista_arg:
    expresion lista_arg_p
    ;
lista_arg_p:
    COLON expresion lista_arg_p
    |   /* vacío */
    ;

%%

void utec::compilers::Parser::error(const std::string& msg) {
    std::cerr << msg << " " /*<< yylineno*/ <<'\n';
    exit(1);
}
