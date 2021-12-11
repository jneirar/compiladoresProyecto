#pragma once

#include "Node.h"

//FactorExpresion
FactorExpresion::FactorExpresion(ExpresionNode *expresion)
{
    this->expresion = expresion;
}
void FactorExpresion::print(std::string prev)
{
    std::cout << prev << "Factor expresión\n";
    this->expresion->print(prev + "\n");
}
//FactorVar
FactorVar::FactorVar(VarNode *varNode)
{
    this->varNode = varNode;
}
void FactorVar::print(std::string prev)
{
    std::cout << prev << "Factor Var: ID=" << this->varNode->getId();
    if (this->varNode->getEsArreglo())
    {
        std::cout << ". Es arreglo. Indice:\n";
        this->varNode->getIndice()->print(prev + "\t");
    }
    else
    {
        std::cout << "\n";
    }
}
//FactorCall
FactorCall::FactorCall(std::string id, std::vector<ExpresionNode *> lista_arg)
{
    this->id = id;
    this->lista_arg = lista_arg;
}
void FactorCall::print(std::string prev)
{
    std::cout << prev << "Factor call ID=" << this->id << " LISTA_ARGS: " << this->lista_arg.size() << "\n";
    for (ExpresionNode *&arg : lista_arg)
        arg->print(prev + "\t");
}
//FactorNum
FactorNum::FactorNum(int numero)
{
    this->numero = numero;
}
void FactorNum::print(std::string prev)
{
    std::cout << prev << "Factor número: " << numero << "\n";
}
//DeclaracionVarNode
DeclaracionVarNode::DeclaracionVarNode(std::string id)
{
    this->id = id;
    this->esArreglo = false;
    this->size = 0;
}
DeclaracionVarNode::DeclaracionVarNode(std::string id, int size)
{
    this->id = id;
    this->esArreglo = true;
    this->size = size;
}
void DeclaracionVarNode::print(std::string prev)
{
    std::cout << prev << "Declaración Var\tID=" << this->id << "\ttipo=" << this->tipo;
    if (this->esArreglo)
        std::cout << "\tEs arreglo de tamaño " << this->size;
    std::cout << "\n";
}

//SentCompuestaNode
SentCompuestaNode::SentCompuestaNode(std::vector<DeclaracionVarNode *> declaracionesLocales, std::vector<SentenciaNode *> sentencias)
{
    this->sentencias = sentencias;
    this->declaracionesLocales = declaracionesLocales;
}
void SentCompuestaNode::print(std::string prev)
{
    std::cout << prev << "Declaraciones locales: " << this->declaracionesLocales.size() << "\n";
    int i = 1;
    for (DeclaracionVarNode *&declaracionLocal : this->declaracionesLocales)
    {
        std::cout << prev << "\tDeclaración " << i++ << ":";
        declaracionLocal->print(prev + "\t");
    }

    i = 1;
    std::cout << prev << "Sentencias: " << this->sentencias.size() << "\n";
    for (SentenciaNode *&sentencia : this->sentencias)
    {
        std::cout << prev << "\tSentencia " << i++ << ":";
        sentencia->print(prev + "\t");
    }
}

//
ExpresionAsignacionNode::ExpresionAsignacionNode(std::string id, ExpresionNode *expresion)
{
    this->id = id;
    this->esArreglo = false;
    this->expresion = expresion;
    this->indice = nullptr;
}
ExpresionAsignacionNode::ExpresionAsignacionNode(std::string id, ExpresionNode *expresion, ExpresionNode *indice)
{
    this->id = id;
    this->esArreglo = true;
    this->expresion = expresion;
    this->indice = indice;
}
void ExpresionAsignacionNode::print(std::string prev)
{
    std::cout << prev << "Expresión de asignación.";
    std::cout << "ID: " << this->id << " = \n";
    if (this->esArreglo)
    {
        std::cout << prev << "\tIndice\n";
        this->indice->print(prev + "\t\t");
    }

    std::cout << prev << "\tExpresión a la derecha de la asignación:\n";
    this->expresion->print(prev + "\t\t");
}
//ExpresionSimpleAux
ExpresionSimpleAux::ExpresionSimpleAux(std::string relop,
                                       ExpresionAditiva *expresionAditiva)
{
    this->relop = relop;
    this->expresionAditiva = expresionAditiva;
}
ExpresionSimpleAux::ExpresionSimpleAux()
{
    this->relop = "";
    this->expresionAditiva = nullptr;
}
std::string ExpresionSimpleAux::getRelop() { return relop; }
ExpresionAditiva *ExpresionSimpleAux::getExpresionAditiva() { return expresionAditiva; }
//ExpresionSimple

ExpresionSimple::ExpresionSimple(ExpresionAditiva *expresionAditiva)
{
    this->expresionAditiva1 = expresionAditiva;
    this->relop = "";
    this->expresionAditiva2 = nullptr;
}
ExpresionSimple::ExpresionSimple(ExpresionAditiva *expresionAditiva1, std::string relop, ExpresionAditiva *expresionAditiva2)
{
    this->expresionAditiva1 = expresionAditiva1;
    this->relop = relop;
    this->expresionAditiva2 = expresionAditiva2;
}
void ExpresionSimple::print(std::string prev)
{
    std::cout << prev << "Expresión simple: ";
    if (expresionAditiva2 == nullptr)
    {
        std::cout << "No relop\n";
        this->expresionAditiva1->print(prev + "\t");
    }
    else
    {
        std::cout << "Relop: " << this->relop << "\n";
        this->expresionAditiva1->print(prev + "\t");
        this->expresionAditiva2->print(prev + "\t");
    }
}
