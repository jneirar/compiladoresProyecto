#include <gtest/gtest.h>

#include <strstream>

#include "FlexScanner.hpp"
#include "parser.hpp"

using namespace utec::compilers;

class ParamTest : public testing::TestWithParam<std::pair<std::string, int>>
{
};

TEST_P(ParamTest, basicTest)
{
    std::istrstream str(GetParam().first.c_str());

    FlexScanner scanner{str, std::cerr};
    int result = 0;
    Parser parser{&scanner, &result};

    parser.parse();
    EXPECT_EQ(result, GetParam().second);
}

INSTANTIATE_TEST_SUITE_P(SimpleTest, ParamTest,
                         testing::Values(
                             std::make_pair(
                                 "entero variable;", 0)
                             /*,std::make_pair(
                                "entero variable;\n"
                                "entero variableDos;", 0)
                            ,std::make_pair(
                                "entero variable;\n"
                                "entero array[5];", 0)
                            ,std::make_pair(
                                "entero factorial ( entero abc ) { }", 0)*/
                             ));

/*
class LexicTest : public testing::TestWithParam<std::pair<std::string, std::vector<Token>>>
{
};

TEST_P(LexicTest, basicTest)
{
    std::istrstream str(GetParam().first.c_str());

    FlexScanner scanner(str, std::cout);
    std::vector<Token> tokens;

    while (1)
    {
        auto token = scanner.get_token();
        if (token._atributo == END)
            break;
        tokens.push_back(token);
    }

    EXPECT_EQ(tokens, GetParam().second);
}

INSTANTIATE_TEST_SUITE_P(LexicTest,
                         LexicTest,
                         testing::Values(
                             std::make_pair("a=-2.7E4",
                                            std::vector<Token>{Token("a", Categoria::ID),
                                                               Token("=", Categoria::IGUAL),
                                                               Token("-2.7E4", Categoria::NUM)}),
                             std::make_pair("a = b /* shouldn't be printed * /c = d ",
                                            std::vector<Token>{Token("a", Categoria::ID),
                                                               Token("=", Categoria::IGUAL),
                                                               Token("b", Categoria::ID),
                                                               Token("c", Categoria::ID),
                                                               Token("=", Categoria::IGUAL),
                                                               Token("d", Categoria::ID)}),
                             std::make_pair("a = b\n@",
                                            std::vector<Token>{
                                                Token("a", Categoria::ID),
                                                Token("=", Categoria::IGUAL),
                                                Token("b", Categoria::ID),
                                                Token("@", Categoria::ERROR),
                                            }),
                             std::make_pair("a < 5\n",
                                            std::vector<Token>{Token("a", Categoria::ID),
                                                               Token("<", Categoria::RELOP),
                                                               Token("5", Categoria::NUM)}),
                             std::make_pair("si(a == b) a = c;\n",
                                            std::vector<Token>{Token("si", Categoria::SI),
                                                               Token("(", Categoria::OPPAR),
                                                               Token("a", Categoria::ID),
                                                               Token("==", Categoria::RELOP),
                                                               Token("b", Categoria::ID),
                                                               Token(")", Categoria::CLPAR),
                                                               Token("a", Categoria::ID),
                                                               Token("=", Categoria::IGUAL),
                                                               Token("c", Categoria::ID),
                                                               Token(";", Categoria::SEMICOLON)}),
                             std::make_pair("{a = b}\n",
                                            std::vector<Token>{Token("{", Categoria::OPBRA),
                                                               Token("a", Categoria::ID),
                                                               Token("=", Categoria::IGUAL),
                                                               Token("b", Categoria::ID),
                                                               Token("}", Categoria::CLBRA)}),
                             std::make_pair("si(a) a;\nsino{ a = c}",
                                            std::vector<Token>{Token("si", Categoria::SI),
                                                               Token("(", Categoria::OPPAR),
                                                               Token("a", Categoria::ID),
                                                               Token(")", Categoria::CLPAR),
                                                               Token("a", Categoria::ID),
                                                               Token(";", Categoria::SEMICOLON),
                                                               Token("sino", Categoria::SINO),
                                                               Token("{", Categoria::OPBRA),
                                                               Token("a", Categoria::ID),
                                                               Token("=", Categoria::IGUAL),
                                                               Token("c", Categoria::ID),
                                                               Token("}", Categoria::CLBRA)}),
                             std::make_pair("mientras(a) a;\n",
                                            std::vector<Token>{Token("mientras", Categoria::MIENTRAS),
                                                               Token("(", Categoria::OPPAR),
                                                               Token("a", Categoria::ID),
                                                               Token(")", Categoria::CLPAR),
                                                               Token("a", Categoria::ID),
                                                               Token(";", Categoria::SEMICOLON)}),
                             std::make_pair("a = b * c;\n",
                                            std::vector<Token>{Token("a", Categoria::ID),
                                                               Token("=", Categoria::IGUAL),
                                                               Token("b", Categoria::ID),
                                                               Token("*", Categoria::MULOP),
                                                               Token("c", Categoria::ID),
                                                               Token(";", Categoria::SEMICOLON)}),
                             std::make_pair("a = b / c;\n",
                                            std::vector<Token>{Token("a", Categoria::ID),
                                                               Token("=", Categoria::IGUAL),
                                                               Token("b", Categoria::ID),
                                                               Token("/", Categoria::DIVOP),
                                                               Token("c", Categoria::ID),
                                                               Token(";", Categoria::SEMICOLON)}),
                             std::make_pair("entero a = 1;\n",
                                            std::vector<Token>{Token("entero", Categoria::ENTERO),
                                                               Token("a", Categoria::ID),
                                                               Token("=", Categoria::IGUAL),
                                                               Token("1", Categoria::NUM),
                                                               Token(";", Categoria::SEMICOLON)}),
                             std::make_pair("sin_tipo funct\n",
                                            std::vector<Token>{Token("sin_tipo", Categoria::SIN_TIPO),
                                                               Token("funct", Categoria::ID)}),
                             std::make_pair("sin_tipo main()\n{\nretorno 0;\n}\n",
                                            std::vector<Token>{Token("sin_tipo", Categoria::SIN_TIPO),
                                                               Token("main", Categoria::MAIN),
                                                               Token("(", Categoria::OPPAR),
                                                               Token(")", Categoria::CLPAR),
                                                               Token("{", Categoria::OPBRA),
                                                               Token("retorno", Categoria::RETORNO),
                                                               Token("0", Categoria::NUM),
                                                               Token(";", Categoria::SEMICOLON),
                                                               Token("}", Categoria::CLBRA)}),
                             std::make_pair("a/* Este es un \n comentario \n multilínea\n* /",
                                            std::vector<Token>{Token("a", Categoria::ID)})));

class SintacticTest : public testing::TestWithParam<std::pair<std::string, std::vector<std::string>>>
{
};

TEST_P(SintacticTest, basicTest)
{
    std::istrstream str(GetParam().first.c_str());

    Parser parser(str, std::cout);
    EXPECT_EQ(parser.parse(), GetParam().second);
}

INSTANTIATE_TEST_SUITE_P(SintacticTest,
                         SintacticTest,
                         testing::Values(
                             std::make_pair("entero var ;", std::vector<std::string>{}),
                             std::make_pair("entero array[5] ;", std::vector<std::string>()),
                             std::make_pair("entero array[5] = ;", std::vector<std::string>{"caracter = unexpected", "caracter = unexpected"})));
*/
int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}