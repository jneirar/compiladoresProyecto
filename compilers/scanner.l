%{
#include "parser.hpp"
#include "FlexScanner.hpp"
#include "SymbolTable.hpp"
#define YY_DECL int utec::compilers::FlexScanner::lex(utec::compilers::Parser::semantic_type *yylval)
extern int row;
extern int col;
extern SymbolTable symbolTable;
extern int numero;
extern std::string texto;
extern std::string tipo;
extern std::string operacionRelop;
extern std::string operacionArit;
%}

%option c++ interactive noyywrap noyylineno nodefault

DIGITO  [0-9]
LETRA   [a-zA-Z]
%%

entero {tipo = strdup(yytext); std::string text = yytext; col += text.size();  return Parser::token::ENTERO;}
retorno {std::string text = yytext; col += text.size(); return Parser::token::RETORNO;}
sin_tipo {tipo = strdup(yytext); std::string text = yytext; col += text.size(); return Parser::token::SIN_TIPO;}
mientras {std::string text = yytext; col += text.size(); return Parser::token::MIENTRAS;}
si {std::string text = yytext; col += text.size(); return Parser::token::SI;}
sino {std::string text = yytext; col += text.size(); return Parser::token::SINO;}
main {std::string text = yytext; col += text.size(); return Parser::token::MAIN;}

{LETRA}+ {texto = strdup(yytext); std::string text = yytext; Symbol symbol(text, row, col); symbolTable.addSymbol(symbol); col += text.size(); return Parser::token::ID;}
[+-]?{DIGITO}+\.{DIGITO}*[[eE]{DIGITO}+]? {numero = atoi(yytext); std::string text = yytext; col += text.size(); return Parser::token::NUM;}
[+-]?{DIGITO}+ {numero = atoi(yytext); std::string text = yytext; col += text.size(); return Parser::token::NUM;}

"+" {operacionArit = strdup(yytext); col++; return Parser::token::ADDOP;}
"-" {operacionArit = strdup(yytext); col++; return Parser::token::SUBOP;}
"*" {operacionArit = strdup(yytext); col++; return Parser::token::MULOP;}
"/" {operacionArit = strdup(yytext); col++; return Parser::token::DIVOP;}

"<" {operacionRelop = strdup(yytext); col++; return Parser::token::RELOP;}
">" {operacionRelop = strdup(yytext); col++; return Parser::token::RELOP;}
"<=" {operacionRelop = strdup(yytext); col+=2; return Parser::token::RELOP;}
">=" {operacionRelop = strdup(yytext); col+=2; return Parser::token::RELOP;}
"==" {operacionRelop = strdup(yytext); col+=2; return Parser::token::RELOP;}
"!=" {operacionRelop = strdup(yytext); col+=2; return Parser::token::RELOP;}

"=" {col++; return Parser::token::IGUAL;}

";" {col++; return Parser::token::SEMICOLON;}
"," {col++; return Parser::token::COLON;}
"(" {col++; return Parser::token::OPPAR;}
")" {col++; return Parser::token::CLPAR;}
"[" {col++; return Parser::token::OPCOR;}
"]" {col++; return Parser::token::CLCOR;}
"{" {col++; return Parser::token::OPBRA;}
"}" {col++; return Parser::token::CLBRA;}

\n {row++; col = 1;}

[\/.\*].*[\*.\/]

[\/.\*](.|\n)*[\*.\/] {
    std::string t = yytext;
    for(char c : t)
        if(c == '\n')
            row++;
}
[ ]     {col++;}
[\t]	{col += 4;}
.		{ std::cout<<"Error\n"; std::cout << yytext << "(line " << row << " col " << col << ")\n"; std::cerr << "SCANNER \n"; /*yyerror("");*/ exit(1);	}
%%

int yyFlexLexer::yylex() {
    throw std::runtime_error("Invalid call to yyFlexLexer::yylex()");
}