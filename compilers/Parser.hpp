#pragma once

#include <vector>
#include "FlexScanner.hpp"
#include "Token.hpp"

namespace utec
{
  namespace compilers
  {

    // Grammar:
    // var_declaration -> entero ID var_declaration_p
    // var_declaration_p -> ; | [NUM] ;

    class Parser
    {
    public:
      Parser(std::istream &arg_yyin, std::ostream &arg_yyout) : scanner(arg_yyin, arg_yyout) {}

      void expect(Categoria cat_expected)
      {
        auto token = scanner.query_token();
        if (token._atributo == cat_expected)
        {
          scanner.get_token();
        }
        else
        {
          errors.push_back("caracter " + token._lexema + " unexpected");
        }
      }

      void var_declaration_p()
      {
        auto token = scanner.query_token();
        if (token._atributo == Categoria::SEMICOLON)
        {
          expect(Categoria::SEMICOLON);
        }
        else if (token._atributo == Categoria::OPCOR)
        {
          expect(Categoria::OPCOR);
          expect(Categoria::NUM);
          expect(Categoria::CLCOR);
          expect(Categoria::SEMICOLON);
        }
      }
      void var_declaration()
      {
        expect(Categoria::ENTERO);
        expect(Categoria::ID);
        var_declaration_p();
      }

      std::vector<std::string> parse()
      {
        var_declaration();

        expect(Categoria::END);
        return errors;

        // if( scanner.query_token()._atributo == Categoria::END
        //   && errors.empty())
        //   return true;
        // return false;
      }

    private:
      std::vector<std::string> errors;
      FlexScanner scanner;
    };

  } // namespace compilers
} // namespace utec