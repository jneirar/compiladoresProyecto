#pragma once

#include "Node.h"

//FactorExpresion
FactorExpresion::FactorExpresion(ExpresionNode *expresion)
{
    this->expresion = expresion;
}
FactorExpresion::~FactorExpresion()
{
    if (expresion)
    {
        delete expresion;
        expresion = nullptr;
    }
}
void FactorExpresion::print(std::string prev, int localScope)
{
    std::cout << prev << "Factor expresión\n";
    this->expresion->print(prev + "\n", localScope);
}
//FactorVar
FactorVar::FactorVar(VarNode *varNode)
{
    this->varNode = varNode;
}
FactorVar::~FactorVar()
{
    if (varNode)
    {
        delete varNode;
        varNode = nullptr;
    }
}
void FactorVar::print(std::string prev, int localScope)
{
    this->varNode->evaluate(localScope);
    std::cout << prev << "Factor Var: ID=" << this->varNode->getId();
    if (this->varNode->getEsArreglo())
    {
        std::cout << ". Es arreglo. Indice:\n";
        this->varNode->getIndice()->print(prev + "\t", localScope);
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
FactorCall::~FactorCall()
{
    for (int i = 0; i < lista_arg.size(); i++)
    {
        if (lista_arg[i])
        {
            delete lista_arg[i];
            lista_arg[i] = nullptr;
        }
    }
    lista_arg.clear();
}
void FactorCall::print(std::string prev, int localScope)
{
    if (symbolTable.isSymbol(id))
    {
        Symbol *&symbol = symbolTable.getSymbol(id);
        bool flag = false;
        for (int scope : symbol->scope)
        {
            if (scope <= localScope)
            {
                flag = true;
                break;
            }
        }
        if (!flag)
            errors.push_back("Symbol " + id + " has not been declared (row: " + std::to_string(symbol->row) + ", col: " + std::to_string(symbol->col) + ")");
    }
    else
        errors.push_back("Symbol " + id + " not founded\n");
    std::cout << prev << "Factor call ID=" << this->id << " LISTA_ARGS: " << this->lista_arg.size() << "\n";
    for (ExpresionNode *&arg : lista_arg)
        arg->print(prev + "\t", localScope);
}
//FactorNum
FactorNum::FactorNum(int numero)
{
    this->numero = numero;
}
void FactorNum::print(std::string prev, int localScope)
{
    std::cout << prev << "Factor número: " << numero << "\n";
}
//Term
Term::Term(Term *term, std::string mulop, Factor *factor)
{
    this->term = term;
    this->mulop = mulop;
    this->factor = factor;
}
Term::Term(Factor *factor)
{
    this->term = nullptr;
    this->mulop = "";
    this->factor = factor;
}
Term::~Term()
{
    if (term)
    {
        delete term;
        term = nullptr;
    }
    if (factor)
    {
        delete factor;
        factor = nullptr;
    }
}
void Term::print(std::string prev, int localScope)
{
    std::cout << prev << "Term. ";
    if (this->term == nullptr)
    {
        std::cout << "Solo factor\n";
        this->factor->print(prev + "\t", localScope);
    }
    else
    {
        std::cout << this->mulop << "\n";
        this->term->print(prev + "\t", localScope);
        this->factor->print(prev + "\t", localScope);
    }
}
//VarNodeAux
VarNodeAux::VarNodeAux(ExpresionNode *expresion)
{
    this->expresion = expresion;
    this->esArreglo = true;
}
VarNodeAux::VarNodeAux()
{
    this->expresion = nullptr;
    this->esArreglo = false;
}
VarNodeAux::~VarNodeAux()
{
    if (expresion)
    {
        delete expresion;
        expresion = nullptr;
    }
}
bool VarNodeAux::getEsArreglo() { return esArreglo; }
ExpresionNode *VarNodeAux::getIndice() { return expresion; }
//VarNode
VarNode::VarNode(std::string id, VarNodeAux *varNodeAux)
{
    this->id = id;
    this->varNodeAux = varNodeAux;
}
VarNode::~VarNode()
{
    if (varNodeAux)
    {
        delete varNodeAux;
        varNodeAux = nullptr;
    }
}
std::string VarNode::getId() { return id; }
ExpresionNode *VarNode::getIndice() { return varNodeAux->getIndice(); }
bool VarNode::getEsArreglo() { return varNodeAux->getEsArreglo(); }
void VarNode::evaluate(int localScope)
{
    if (symbolTable.isSymbol(id))
    {
        Symbol *&symbol = symbolTable.getSymbol(id);
        bool flag = false;
        for (int scope : symbol->scope)
        {
            if (scope <= localScope)
            {
                flag = true;
                break;
            }
        }
        if (!flag)
            errors.push_back("Symbol " + id + " has not been declared (row: " + std::to_string(symbol->row) + ", col: " + std::to_string(symbol->col) + ")");
    }
    else
        errors.push_back("Symbol " + id + " not founded\n");
}
//ExpresionAditiva
ExpresionAditiva::ExpresionAditiva(ExpresionAditiva *expresionAditiva,
                                   std::string addop,
                                   Term *term)
{
    this->expresionAditiva = expresionAditiva;
    this->addop = addop;
    this->term = term;
}
ExpresionAditiva::ExpresionAditiva(Term *term)
{
    this->expresionAditiva = nullptr;
    this->addop = "";
    this->term = term;
}
ExpresionAditiva::~ExpresionAditiva()
{
    if (expresionAditiva)
    {
        delete expresionAditiva;
        expresionAditiva = nullptr;
    }
    if (term)
    {
        delete term;
        term = nullptr;
    }
}
void ExpresionAditiva::print(std::string prev, int localScope)
{
    std::cout << prev << "Expresión aditiva. ";
    if (this->expresionAditiva == nullptr)
    {
        std::cout << "Solo term\n";
        this->term->print(prev + "\t", localScope);
    }
    else
    {
        std::cout << this->addop << "\n";
        this->expresionAditiva->print(prev + "\t", localScope);
        this->term->print(prev + "\t", localScope);
    }
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
void DeclaracionVarNode::print(std::string prev, int localScope)
{
    if (symbolTable.isSymbol(id))
    {
        Symbol *&symbol = symbolTable.getSymbol(id);
        symbol->type = this->tipo;
        symbol->isArray = this->esArreglo;
        if (symbol->scope.count(localScope))
            errors.push_back("Symbol " + id + " already declared");
        else
            symbol->scope.insert(localScope);
    }
    else
        errors.push_back("Symbol " + id + " not founded\n");
    std::cout << prev << "Declaración Var\tID=" << this->id << "\ttipo=" << this->tipo;
    if (this->esArreglo)
        std::cout << "\tEs arreglo de tamaño " << this->size;
    std::cout << "\n";
}
//SentenciaExpresionNode
SentenciaExpresionNode::SentenciaExpresionNode(ExpresionNode *expresion) { this->expresion = expresion; }
SentenciaExpresionNode::SentenciaExpresionNode() { this->expresion = nullptr; }
SentenciaExpresionNode::~SentenciaExpresionNode()
{
    if (expresion)
    {
        delete expresion;
        expresion = nullptr;
    }
}
void SentenciaExpresionNode::print(std::string prev, int localScope)
{
    std::cout << prev << "Sentencia Expresión\n";
    this->expresion->print(prev + "\t", localScope);
}
//SentenciaSeleccionNode
SentenciaSeleccionNode::SentenciaSeleccionNode(ExpresionNode *expresionNode,
                                               SentenciaNode *sentencia)
{
    this->expresionNode = expresionNode;
    this->sentencia = sentencia;
    this->sentenciaElse = nullptr;
}
SentenciaSeleccionNode::SentenciaSeleccionNode(ExpresionNode *expresionNode,
                                               SentenciaNode *sentencia,
                                               SentenciaNode *sentenciaElse)
{
    this->expresionNode = expresionNode;
    this->sentencia = sentencia;
    this->sentenciaElse = sentenciaElse;
}
SentenciaSeleccionNode::~SentenciaSeleccionNode()
{
    if (expresionNode)
    {
        delete expresionNode;
        expresionNode = nullptr;
    }
    if (sentencia)
    {
        delete sentencia;
        sentencia = nullptr;
    }
    if (sentenciaElse)
    {
        delete sentenciaElse;
        sentenciaElse = nullptr;
    }
}

void SentenciaSeleccionNode::print(std::string prev, int localScope)
{
    std::cout << prev << "Sentencia Selección\n";
    std::cout << prev + "\t"
              << "Expresión de condición\n";
    this->expresionNode->print(prev + "\t\t", localScope + 1);
    std::cout << prev + "\t"
              << "Sentencia Then\n";
    this->sentencia->print(prev + "\t\t", localScope + 1);
    if (this->sentenciaElse != nullptr)
    {
        std::cout << prev + "\t"
                  << "Sentencia Else\n";
        this->sentenciaElse->print(prev + "\t\t", localScope + 1);
    }
}
//SentenciaIteracionNode
SentenciaIteracionNode::SentenciaIteracionNode(ExpresionNode *expresion,
                                               std::vector<SentenciaNode *> sentencias)
{
    this->expresion = expresion;
    this->sentencias = sentencias;
}
SentenciaIteracionNode::~SentenciaIteracionNode()
{
    if (expresion)
    {
        delete expresion;
        expresion = nullptr;
    }
    for (int i = 0; i < sentencias.size(); i++)
    {
        if (sentencias[i])
        {
            delete sentencias[i];
            sentencias[i] = nullptr;
        }
    }
    sentencias.clear();
}
void SentenciaIteracionNode::print(std::string prev, int localScope)
{
    std::cout << prev << "\tSentencia Iteración\n";
    std::cout << prev << "\tExpresión de condición:\n";
    this->expresion->print(prev + "\t\t", localScope);
    std::cout << prev << "\tSentencias de iteración: " << this->sentencias.size() << "\n";
    int i = 1;
    for (SentenciaNode *&sentencia : sentencias)
    {
        std::cout << prev + "\t"
                  << "Sentencia " << i++ << ":";
        sentencia->print(prev + "\t\t", localScope + 1);
    }
}
//SentenciaRetornoNode
SentenciaRetornoNode::SentenciaRetornoNode() { this->expresion = nullptr; }
SentenciaRetornoNode::SentenciaRetornoNode(ExpresionNode *expresion) { this->expresion = expresion; }
SentenciaRetornoNode::~SentenciaRetornoNode()
{
    if (expresion)
    {
        delete expresion;
        expresion = nullptr;
    }
}
void SentenciaRetornoNode::print(std::string prev, int localScope)
{
    std::cout << prev << "Sentencia Retorno\n";
    if (this->expresion == nullptr)
        std::cout << prev + "\tNo retorna nada\n";
    else
    {
        std::cout << prev + "\tRetorna una expresión\n";
        this->expresion->print(prev + "\t", localScope);
    }
}

//SentCompuestaNode
SentCompuestaNode::SentCompuestaNode(std::vector<DeclaracionVarNode *> declaracionesLocales, std::vector<SentenciaNode *> sentencias)
{
    this->sentencias = sentencias;
    this->declaracionesLocales = declaracionesLocales;
}
SentCompuestaNode::~SentCompuestaNode()
{
    for (int i = 0; i < declaracionesLocales.size(); i++)
    {
        if (declaracionesLocales[i])
        {
            delete declaracionesLocales[i];
            declaracionesLocales[i] = nullptr;
        }
    }
    declaracionesLocales.clear();
    for (int i = 0; i < sentencias.size(); i++)
    {
        if (sentencias[i])
        {
            delete sentencias[i];
            sentencias[i] = nullptr;
        }
    }
    sentencias.clear();
}
void SentCompuestaNode::print(std::string prev, int localScope)
{
    std::cout << prev << "Declaraciones locales: " << this->declaracionesLocales.size() << "\n";
    int i = 1;
    for (DeclaracionVarNode *&declaracionLocal : this->declaracionesLocales)
    {
        std::cout << prev << "\tDeclaración " << i++ << ":";
        declaracionLocal->print(prev + "\t", localScope);
    }

    i = 1;
    std::cout << prev << "Sentencias: " << this->sentencias.size() << "\n";
    for (SentenciaNode *&sentencia : this->sentencias)
    {
        std::cout << prev << "\tSentencia " << i++ << ":";
        sentencia->print(prev + "\t", localScope);
    }
}

//ExpresionAsignacionNode
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
ExpresionAsignacionNode::~ExpresionAsignacionNode()
{
    if (indice)
    {
        delete indice;
        indice = nullptr;
    }
    if (expresion)
    {
        delete expresion;
        expresion = nullptr;
    }
}
void ExpresionAsignacionNode::print(std::string prev, int localScope)
{
    if (symbolTable.isSymbol(id))
    {
        Symbol *&symbol = symbolTable.getSymbol(id);
        bool flag = false;
        for (int scope : symbol->scope)
        {
            if (scope <= localScope)
            {
                flag = true;
                break;
            }
        }
        if (!flag)
            errors.push_back("Symbol " + id + " has not been declared (row: " + std::to_string(symbol->row) + ", col: " + std::to_string(symbol->col) + ")");
    }
    else
        errors.push_back("Symbol " + id + " not founded\n");
    std::cout << prev << "Expresión de asignación.";
    std::cout << "ID: " << this->id << " = \n";
    if (this->esArreglo)
    {
        std::cout << prev << "\tIndice\n";
        this->indice->print(prev + "\t\t", localScope);
    }

    std::cout << prev << "\tExpresión a la derecha de la asignación:\n";
    this->expresion->print(prev + "\t\t", localScope);
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
ExpresionSimpleAux::~ExpresionSimpleAux()
{
    if (expresionAditiva)
    {
        delete expresionAditiva;
        expresionAditiva = nullptr;
    }
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
ExpresionSimple::~ExpresionSimple()
{
    if (expresionAditiva1)
    {
        delete expresionAditiva1;
        expresionAditiva1 = nullptr;
    }
    if (expresionAditiva2)
    {
        delete expresionAditiva2;
        expresionAditiva2 = nullptr;
    }
}
void ExpresionSimple::print(std::string prev, int localScope)
{
    std::cout << prev << "Expresión simple: ";
    if (expresionAditiva2 == nullptr)
    {
        std::cout << "No relop\n";
        this->expresionAditiva1->print(prev + "\t", localScope);
    }
    else
    {
        std::cout << "Relop: " << this->relop << "\n";
        this->expresionAditiva1->print(prev + "\t", localScope);
        this->expresionAditiva2->print(prev + "\t", localScope);
    }
}
