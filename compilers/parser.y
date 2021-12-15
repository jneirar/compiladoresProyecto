%{
#include <iostream>
#include <string>
#include <cmath>
#include <FlexLexer.h>
#include "Node.h"
#include "Node.cpp"
#include "SymbolTable.hpp"
#include <vector>
#include <variant>

int row = 1;
int col = 1;
ProgramaNode* syntacticTree = nullptr;
SymbolTable symbolTable;

std::vector<std::string> errors;
%}

%require "3.5.1"
%language "C++"

%define api.parser.class {Parser}
%define api.namespace {utec::compilers}
%define api.value.type variant
%parse-param {FlexScanner* scanner} {int* result}

%code requires
{
    class Factor;
    class FactorExpresion;
    class FactorVar;
    class FactorCall;
    class FactorNum;
    class Term;
    class ExpresionAditiva;
    class ExpresionNode;
    class ExpresionAsignacionNode;
    class ExpresionSimple;
    class ExpresionSimpleAux;

    class VarNode;
    class VarNodeAux;

    class SentenciaNode;
    class SentenciaExpresionNode;
    class SentenciaSeleccionNode;
    class SentenciaIteracionNode;
    class SentenciaRetornoNode;
    class SentCompuestaNode;

    class ParamNodeAux;
    class ParamNode;
    class DeclaracionNode;
    class DeclaracionVarNode;
    class DeclaracionFunNode;

    class DeclaracionNodeAux;
    class DeclaracionFunNodeAux;
    class DeclaracionVarNodeAux;

    class ProgramaNode;

    class SymbolTable;
    class Symbol;
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

%token <std::string> ENTERO SIN_TIPO
%token RETORNO MIENTRAS SI SINO
%token <std::string> ID
%token <int> NUM
%token <std::string> ADDOP SUBOP MULOP DIVOP
%token <std::string> RELOP
%token IGUAL
%token SEMICOLON COLON OPPAR CLPAR OPCOR CLCOR OPBRA CLBRA

%type <ProgramaNode*> programa 
%type <std::vector<DeclaracionNode*>> lista_declaracion
%type <DeclaracionNode*> declaracion
%type <DeclaracionNodeAux*> declaracion_fact
%type <DeclaracionVarNodeAux*> var_declaracion_p
%type <std::vector<ParamNode*>> params lista_params
%type <std::string> tipo
%type <DeclaracionVarNode*> var_declaracion
%type <SentCompuestaNode*> sent_compuesta
%type <ParamNode*> param
%type <ParamNodeAux*> param_p
%type <std::vector<DeclaracionVarNode *>> declaracion_local
%type <std::vector<SentenciaNode *>> lista_sentencias
%type <SentenciaNode*> sentencia
%type <SentenciaExpresionNode*> sentencia_expresion
%type <SentenciaSeleccionNode*> sentencia_seleccion
%type <SentenciaIteracionNode*> sentencia_iteracion
%type <SentenciaRetornoNode*> sentencia_retorno sentencia_retorno_p
%type <SentenciaNode*> sentencia_seleccion_p
%type <ExpresionNode*> expresion
%type <ExpresionSimple*> expresion_simple
%type <ExpresionSimpleAux*> expresion_simple_p
%type <ExpresionAditiva*> expresion_aditiva
%type <VarNode*>  var
%type <VarNodeAux*>  var_p
%type <std::string> addope mulope
%type <Term*> term
%type <Factor*> factor
%type <std::vector<ExpresionNode*>> args lista_arg
%%

input:		/* empty */ 
		| programa  { 
            syntacticTree = $1; 
            syntacticTree->print(); 
            symbolTable.printSymbols(); 
            
            if(!symbolTable.isSymbol("main")){
                errors.push_back("No existe una función main");
            }
            if(errors.size() > 0){
                std::cout << "\n\tErrores semánticos:\n";
                for(std::string &error: errors)
                    std::cout << error << "\n";
                errors.clear();
            }
            row = 1;
            col = 1;
            delete syntacticTree;
            syntacticTree = nullptr;
            symbolTable.clear();
        }
		;

programa:  
    lista_declaracion { $$ = new ProgramaNode($1); }
    ;
lista_declaracion:
    lista_declaracion declaracion { $$ = $1; $$.push_back($2); }
    |   declaracion { $$.push_back($1); }
    ;
declaracion:
    ENTERO ID declaracion_fact { 
        if($3->getEsVarNode()){
            if($3->getEsArreglo())
                $$ = new DeclaracionVarNode($<char*>2, $3->getSize()); 
            else
                $$ = new DeclaracionVarNode($<char*>2); 
        }else{
            $$ = new DeclaracionFunNode($<char*>2, $<char*>1, $3->getParams(), $3->getSentCompuestaNode()); 
        }
    }
    |   SIN_TIPO ID OPPAR params CLPAR sent_compuesta { $$ = new DeclaracionFunNode($<char*>2, $<char*>1, $4, $6); }
    ;
declaracion_fact:
    var_declaracion_p { $$ = $1; }
    |   OPPAR params CLPAR sent_compuesta { $$ = new DeclaracionFunNodeAux($2, $4); }
    ;
var_declaracion:
    ENTERO ID var_declaracion_p { 
        if($3->getEsArreglo()) $$ = new DeclaracionVarNode($<char*>2, $3->getSize()); 
        else $$ = new DeclaracionVarNode($<char*>2); 
    }
    ;
var_declaracion_p:
    SEMICOLON { $$ = new DeclaracionVarNodeAux(); }
    |   OPCOR NUM CLCOR SEMICOLON { $$ = new DeclaracionVarNodeAux($<int>2); }
    ;
tipo:
    ENTERO { $$ = $<char*>1; }
    |   SIN_TIPO { $$ = $<char*>1; }
    ;
params:
    lista_params { $$ = $1; }
    |   SIN_TIPO { }
    ;
lista_params:
    lista_params COLON param { $$ = $1; $$.push_back($3); }
    |   param { $$.push_back($1); }
    ;
param:
    tipo ID param_p { $$ = new ParamNode($<char*>2, $1, $3->getArreglo()); }
    ;
param_p:
    OPCOR CLCOR { $$ = new ParamNodeAux(true); }
    |   /* vacío */ { $$ = new ParamNodeAux(false); }
    ;
sent_compuesta:
    OPBRA declaracion_local lista_sentencias CLBRA { $$ = new SentCompuestaNode($2, $3); }
    ;
declaracion_local:
    var_declaracion declaracion_local { $$ = $2; $$.push_back($1); }
    |   /* vacío */ {}
    ;
lista_sentencias:
    lista_sentencias sentencia { $$ = $1; $$.push_back($2); }
    |   /* vacío */ {}
    ;
sentencia:
    sentencia_expresion { $$ = $1; }
    |   sentencia_seleccion { $$ = $1; }
    |   sentencia_iteracion { $$ = $1; }
    |   sentencia_retorno { $$ = $1; }
    ;
sentencia_expresion:
    expresion SEMICOLON { $$ = new SentenciaExpresionNode($1); }
    |   SEMICOLON { $$ = new SentenciaExpresionNode(); }
    ;
sentencia_seleccion:
    SI OPPAR expresion CLPAR sentencia sentencia_seleccion_p { $$ = new SentenciaSeleccionNode($3, $5, $6); }
    ;
sentencia_seleccion_p: 
    SINO sentencia { $$ = $2; }
    |   /* vacío */ { $$ = nullptr; }
    ;
sentencia_iteracion:
    MIENTRAS OPPAR expresion CLPAR OPBRA lista_sentencias CLBRA { $$ = new SentenciaIteracionNode($3, $6); }
    ;
sentencia_retorno:
    RETORNO sentencia_retorno_p {$$ = $2; }
    ;
sentencia_retorno_p:
    SEMICOLON { $$ = new SentenciaRetornoNode(); }
    |   expresion SEMICOLON { $$ = new SentenciaRetornoNode($1); }
    ;
expresion:
    var IGUAL expresion { 
        if($1->getEsArreglo())
            $$ = new ExpresionAsignacionNode($1->getId(), $3, $1->getIndice()); 
        else
            $$ = new ExpresionAsignacionNode($1->getId(), $3); 
    }
    |   expresion_simple { $$ = $1; }
    ;
var:
    ID var_p { $$ = new VarNode($<char*>1, $2); }
    ;
var_p:
    OPCOR expresion CLCOR { $$ = new VarNodeAux($2); }
    |   /* vacío */ { $$ = new VarNodeAux(); }
    ;
expresion_simple:
    expresion_aditiva expresion_simple_p { 
        if($2->getExpresionAditiva() == nullptr){
            $$ = new ExpresionSimple($1); 
        }
        else{
            $$ = new ExpresionSimple($1, $2->getRelop(), $2->getExpresionAditiva()); 
        }
    }
    ;
expresion_simple_p:
    RELOP expresion_aditiva {$$ = new ExpresionSimpleAux($<char*>1, $2); }
    |   /* vacío */ { $$ = new ExpresionSimpleAux(); }
    ;
expresion_aditiva:
    expresion_aditiva addope term { $$ = new ExpresionAditiva($1, $2, $3); }
    |   term { $$ = new ExpresionAditiva($1); }
    ;
addope:
    ADDOP { $$ = $<char*>1; }
    |   SUBOP { $$ = $<char*>1; }
    ;
term:
    term mulope factor { $$ = new Term($1, $2, $3); }
    |   factor { $$ = new Term($1); }
    ;
mulope:
    MULOP { $$ = $<char*>1; }
    |   DIVOP { $$ = $<char*>1; }
    ;
factor:
    OPPAR expresion CLPAR { $$ = new FactorExpresion($2); }
    |   var { $$ = new FactorVar($1); }
    |   ID OPPAR args CLPAR { $$ = new FactorCall($<char*>1, $3); }
    |   NUM { $$ = new FactorNum($<int>1); }
    ;
args:
    lista_arg { $$ = $1; }
    |   /* vacío */ {}
    ;
lista_arg:
    lista_arg COLON expresion { $$ = $1; $$.push_back($3); }
    |   expresion { $$.push_back($1); }
    ;
%%
void utec::compilers::Parser::error(const std::string& msg) {
    symbolTable.printSymbols(); 

    std::string error = "Error sintáctico en fila: " + std::to_string(row) + ", col: " + std::to_string(col);
    errors.push_back(error);
    std::cout << "Error sintáctico:\n";
    for(std::string &error: errors){
        std::cout << "\t" << error << "\n";
    }
    errors.clear();
    row = 1;
    col = 1;
    delete syntacticTree;
    syntacticTree = nullptr;
    symbolTable.clear();
}