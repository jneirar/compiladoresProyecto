#pragma once

// https://stackoverflow.com/questions/40663527/how-to-inherit-from-yyflexlexer
#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "parser.hpp"

namespace utec {
namespace compilers {

class FlexScanner : public yyFlexLexer {
 public:
  FlexScanner(std::istream& arg_yyin, std::ostream& arg_yyout)
      : yyFlexLexer(arg_yyin, arg_yyout) {}
  FlexScanner(std::istream* arg_yyin = nullptr,
              std::ostream* arg_yyout = nullptr)
      : yyFlexLexer(arg_yyin, arg_yyout) {}
  int lex(
      Parser::semantic_type* yylval);  // note: this is the prototype we need
};

}  // namespace compilers
}  // namespace utec