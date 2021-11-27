%{
#include "parser.hpp"
#include "FlexScanner.hpp"
#define YY_DECL int utec::compilers::FlexScanner::lex(utec::compilers::Parser::semantic_type *yylval)
int n = 1;
%}

%option c++ interactive noyywrap noyylineno nodefault

DIGITO  [0-9]
LETRA   [a-zA-Z]
%%

entero return Parser::token::ENTERO;
retorno return Parser::token::RETORNO;
sin_tipo return Parser::token::SIN_TIPO;
mientras return Parser::token::MIENTRAS;
si return Parser::token::SI;
sino return Parser::token::SINO;
main return Parser::token::MAIN;

{LETRA}+  return Parser::token::ID;
[+-]?{DIGITO}+\.{DIGITO}*[[eE]{DIGITO}+]? return Parser::token::NUM; 
[+-]?{DIGITO}+ return Parser::token::NUM;

"+" return Parser::token::ADDOP;
"-" return Parser::token::SUBOP;
"*" return Parser::token::MULOP;
"/" return Parser::token::DIVOP;

"<" return Parser::token::RELOP;
">" return Parser::token::RELOP;
"<=" return Parser::token::RELOP;
">=" return Parser::token::RELOP;
"==" return Parser::token::RELOP;
"!=" return Parser::token::RELOP;

"=" return Parser::token::IGUAL;

";" return Parser::token::SEMICOLON;
"," return Parser::token::COLON;
"(" return Parser::token::OPPAR;
")" return Parser::token::CLPAR;
"[" return Parser::token::OPCOR;
"]" return Parser::token::CLCOR;
"{" return Parser::token::OPBRA;
"}" return Parser::token::CLBRA;

\n n++;

[\/.\*].*[\*.\/]

[\/.\*](.|\n)*[\*.\/] {
    std::string t = yytext;
    for(char c : t)
        if(c == '\n')
            n++;
}
[ \t]*		{}
.		{ std::cout<<"Error\n"; std::cout << yytext << "(line " << n << ")\n"; std::cerr << "SCANNER \n"; /*yyerror("");*/ exit(1);	}
%%

int yyFlexLexer::yylex() {
    throw std::runtime_error("Invalid call to yyFlexLexer::yylex()");
}