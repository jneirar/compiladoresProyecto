#pragma once

#include <iostream>

#include "Enums.h"

class Token
{
public:
  std::string _lexema;
  Categoria _atributo;

  Token(std::string lexema, Categoria atributo)
      : _lexema(lexema), _atributo(atributo) {}

  Token() : _lexema("END"), _atributo(END) {}

  bool operator==(const Token &in) const
  {
    return in._lexema == this->_lexema && in._atributo == this->_atributo;
  }
};

//Allows printing Tokens on the Google Test failing reports
std::ostream &operator<<(std::ostream &os, Token token)
{
  os << "<";
  switch (token._atributo)
  {
  case Categoria::ID:
    os << "id";
    break;
  case Categoria::NUM:
    os << "num";
    break;
  case Categoria::ENTERO:
    os << "entero";
    break;
  case Categoria::RETORNO:
    os << "retorno";
    break;
  case Categoria::SIN_TIPO:
    os << "sin tipo";
    break;
  case Categoria::MIENTRAS:
    os << "mientras";
    break;
  case Categoria::SI:
    os << "si";
    break;
  case Categoria::SINO:
    os << "sino";
    break;
  case Categoria::MAIN:
    os << "main";
    break;
  case Categoria::ADDOP:
    os << "+";
    break;
  case Categoria::SUBOP:
    os << "-";
    break;
  case Categoria::MULOP:
    os << "*";
    break;
  case Categoria::DIVOP:
    os << "/";
    break;
  case Categoria::RELOP:
    os << "relop";
    break;
  case Categoria::IGUAL:
    os << "=";
    break;
  case Categoria::SEMICOLON:
    os << ";";
    break;
  case Categoria::COLON:
    os << ",";
    break;
  case Categoria::OPPAR:
    os << "(";
    break;
  case Categoria::CLPAR:
    os << ")";
    break;
  case Categoria::OPCOR:
    os << "[";
    break;
  case Categoria::CLCOR:
    os << "]";
    break;
  case Categoria::OPBRA:
    os << "{";
    break;
  case Categoria::CLBRA:
    os << "}";
    break;
  case Categoria::ERROR:
    os << "error";
    break;
  case Categoria::END:
    os << "end";
    break;
  default:
    os << "Sin Categoria";
    break;
  }
  os << ", " << token._lexema << ">";
  return os;
}
