%{
#include "Enums.h"
int n = 1;
%}

%option noyywrap c++

DIGITO  [0-9]
LETRA   [a-zA-Z]
%%
entero return Categoria::ENTERO;
retorno return Categoria::RETORNO;
sin_tipo return Categoria::SIN_TIPO;
mientras return Categoria::MIENTRAS;
si return Categoria::SI;
sino return Categoria::SINO;
main return Categoria::MAIN;

{LETRA}+  return Categoria::ID;
[+-]?{DIGITO}+\.{DIGITO}*[[eE]{DIGITO}+]? return Categoria::NUM; 
[+-]?{DIGITO}+ return Categoria::NUM;

"+" return Categoria::ADDOP;
"-" return Categoria::SUBOP;
"*" return Categoria::MULOP;
"/" return Categoria::DIVOP;

"<" return Categoria::RELOP;
">" return Categoria::RELOP;
"<=" return Categoria::RELOP;
">=" return Categoria::RELOP;
"==" return Categoria::RELOP;
"!=" return Categoria::RELOP;

"=" return Categoria::IGUAL;

";" return Categoria::SEMICOLON;
"," return Categoria::COLON;
"(" return Categoria::OPPAR;
")" return Categoria::CLPAR;
"[" return Categoria::OPCOR;
"]" return Categoria::CLCOR;
"{" return Categoria::OPBRA;
"}" return Categoria::CLBRA;

"@" return Categoria::ERROR;
\n n++;

[\/.\*].*[\*.\/]

[\/.\*](.|\n)*[\*.\/] {
    std::string t = yytext;
    for(char c : t)
        if(c == '\n')
            n++;
}

.|\n
%%