%{
#include "parser.hpp"
#include "FlexScanner.hpp"
#include "SymbolTable.hpp"
#define YY_DECL int utec::compilers::FlexScanner::lex(utec::compilers::Parser::semantic_type *yylval)
extern int row;
extern int col;
extern SymbolTable symbolTable;
%}

%option c++ interactive noyywrap noyylineno nodefault

DIGITO  [0-9]
LETRA   [a-zA-Z]
%%

entero {yylval->build(strdup(yytext)); std::string text = yytext; col += text.size();  return Parser::token::ENTERO;}
retorno {yylval->build(strdup(yytext)); std::string text = yytext; col += text.size(); return Parser::token::RETORNO;}
sin_tipo {yylval->build(strdup(yytext)); std::string text = yytext; col += text.size(); return Parser::token::SIN_TIPO;}
mientras {std::string text = yytext; col += text.size(); return Parser::token::MIENTRAS;}
si {std::string text = yytext; col += text.size(); return Parser::token::SI;}
sino {std::string text = yytext; col += text.size(); return Parser::token::SINO;}

{LETRA}+ { 
    yylval->build(strdup(yytext)); 
    std::string text = yytext; 
    if(!symbolTable.isSymbol(text)){
        Symbol* symbol = new Symbol(text, row, col); 
        symbolTable.addSymbol(symbol); 
    }
    col += text.size(); 
    return Parser::token::ID;}
[+-]?{DIGITO}+ { 
    yylval->build(atoi(yytext)); 
    std::string text = yytext; 
    col += text.size(); 
    return Parser::token::NUM;}

"+" {yylval->build(strdup(yytext)); col++; return Parser::token::ADDOP;}
"-" {yylval->build(strdup(yytext)); col++; return Parser::token::SUBOP;}
"*" {yylval->build(strdup(yytext)); col++; return Parser::token::MULOP;}
"/" {yylval->build(strdup(yytext)); col++; return Parser::token::DIVOP;}

"<" {yylval->build(strdup(yytext)); col++; return Parser::token::RELOP;}
">" {yylval->build(strdup(yytext)); col++; return Parser::token::RELOP;}
"<=" {yylval->build(strdup(yytext)); col+=2; return Parser::token::RELOP;}
">=" {yylval->build(strdup(yytext)); col+=2; return Parser::token::RELOP;}
"==" {yylval->build(strdup(yytext)); col+=2; return Parser::token::RELOP;}
"!=" {yylval->build(strdup(yytext)); col+=2; return Parser::token::RELOP;}

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
[\t]	{col += 2;}
.		{ std::cout<<"Error léxico\n"; std::cout << yytext << " (line " << row << " col " << col << ")\n"; }
%%

int yyFlexLexer::yylex() {
    throw std::runtime_error("Invalid call to yyFlexLexer::yylex()");
}