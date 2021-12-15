#pragma once

#include "parser.hpp"
#include "SymbolTable.hpp"
#include <iostream>
#include <string>
#include <vector>

extern std::vector<std::string> errors;
extern SymbolTable symbolTable;

class Factor;
class FactorExpresion;
class FactorVar;
class FactorCall;
class FactorNum;
class Term;
class ExpresionAditiva;
class ExpresionNode;
class ExpresionAsignacionNode;
class ExpresionSimple;
class ExpresionSimpleAux;

class VarNode;
class VarNodeAux;

class SentenciaNode;
class SentenciaExpresionNode;
class SentenciaSeleccionNode;
class SentenciaIteracionNode;
class SentenciaRetornoNode;
class SentCompuestaNode;

class ParamNodeAux;
class ParamNode;
class DeclaracionNode;
class DeclaracionVarNode;
class DeclaracionFunNode;

class DeclaracionNodeAux;
class DeclaracionFunNodeAux;
class DeclaracionVarNodeAux;

class ProgramaNode;

class Factor
{
public:
    virtual void print(std::string prev, int localScope) = 0;
    virtual ~Factor() {}
};
class FactorExpresion : public Factor
{
private:
    ExpresionNode *expresion;

public:
    FactorExpresion(ExpresionNode *expresion);
    virtual ~FactorExpresion();
    void print(std::string prev, int localScope) override;
};
class FactorVar : public Factor
{
private:
    VarNode *varNode;

public:
    FactorVar(VarNode *varNode);
    virtual ~FactorVar();
    void print(std::string prev, int localScope) override;
};
class FactorCall : public Factor
{
private:
    std::string id;
    std::vector<ExpresionNode *> lista_arg;

public:
    FactorCall(std::string id, std::vector<ExpresionNode *> lista_arg);
    virtual ~FactorCall();
    void print(std::string prev, int localScope) override;
};
class FactorNum : public Factor
{
private:
    int numero;

public:
    FactorNum(int numero);
    virtual ~FactorNum() {}
    void print(std::string prev, int localScope) override;
};
class Term
{
private:
    Term *term;
    std::string mulop;
    Factor *factor;

public:
    Term(Term *term, std::string mulop, Factor *factor);
    Term(Factor *factor);
    virtual ~Term();
    void print(std::string prev, int localScope);
};
class ExpresionAditiva
{
private:
    ExpresionAditiva *expresionAditiva;
    std::string addop;
    Term *term;

public:
    ExpresionAditiva(ExpresionAditiva *expresionAditiva,
                     std::string addop,
                     Term *term);
    ExpresionAditiva(Term *term);
    virtual ~ExpresionAditiva();
    void print(std::string prev, int localScope);
};

class VarNodeAux
{
private:
    ExpresionNode *expresion;
    bool esArreglo;

public:
    VarNodeAux(ExpresionNode *expresion);
    VarNodeAux();
    virtual ~VarNodeAux();
    bool getEsArreglo();
    ExpresionNode *getIndice();
};
class VarNode
{
private:
    std::string id;
    VarNodeAux *varNodeAux;

public:
    VarNode(std::string id, VarNodeAux *varNodeAux);
    virtual ~VarNode();
    std::string getId();
    ExpresionNode *getIndice();
    bool getEsArreglo();
    void evaluate(int localScope);
};

class ExpresionNode
{
public:
    virtual void print(std::string prev, int localScope) = 0;
    virtual ~ExpresionNode(){};
};
class ExpresionAsignacionNode : public ExpresionNode
{
private:
    std::string id;
    bool esArreglo;
    ExpresionNode *indice;
    ExpresionNode *expresion;

public:
    ExpresionAsignacionNode(std::string id, ExpresionNode *expresion);
    ExpresionAsignacionNode(std::string id, ExpresionNode *expresion, ExpresionNode *indice);
    virtual ~ExpresionAsignacionNode();
    void print(std::string prev, int localScope) override;
};

class ExpresionSimpleAux
{
private:
    std::string relop;
    ExpresionAditiva *expresionAditiva;

public:
    ExpresionSimpleAux(std::string relop,
                       ExpresionAditiva *expresionAditiva);
    ExpresionSimpleAux();
    virtual ~ExpresionSimpleAux();
    std::string getRelop();
    ExpresionAditiva *getExpresionAditiva();
};
class ExpresionSimple : public ExpresionNode
{
private:
    std::string relop;
    ExpresionAditiva *expresionAditiva1;
    ExpresionAditiva *expresionAditiva2;

public:
    ExpresionSimple(ExpresionAditiva *expresionAditiva);
    ExpresionSimple(ExpresionAditiva *expresionAditiva1, std::string relop, ExpresionAditiva *expresionAditiva2);
    virtual ~ExpresionSimple();
    void print(std::string prev, int localScope) override;
};

class SentenciaNode
{
public:
    virtual void print(std::string prev, int localScope) = 0;
    virtual ~SentenciaNode() {}
};
class SentenciaExpresionNode : public SentenciaNode
{
private:
    ExpresionNode *expresion;

public:
    SentenciaExpresionNode(ExpresionNode *expresion);
    SentenciaExpresionNode();
    virtual ~SentenciaExpresionNode();
    void print(std::string prev, int localScope) override;
};
class SentenciaSeleccionNode : public SentenciaNode
{
private:
    ExpresionNode *expresionNode;
    SentenciaNode *sentencia;
    SentenciaNode *sentenciaElse;

public:
    SentenciaSeleccionNode(ExpresionNode *expresionNode,
                           SentenciaNode *sentencia);
    SentenciaSeleccionNode(ExpresionNode *expresionNode,
                           SentenciaNode *sentencia,
                           SentenciaNode *sentenciaElse);
    virtual ~SentenciaSeleccionNode();
    void print(std::string prev, int localScope) override;
};
class SentenciaIteracionNode : public SentenciaNode
{
private:
    ExpresionNode *expresion;
    std::vector<SentenciaNode *> sentencias;

public:
    SentenciaIteracionNode(ExpresionNode *expresion,
                           std::vector<SentenciaNode *> sentencias);
    virtual ~SentenciaIteracionNode();
    void print(std::string prev, int localScope) override;
};
class SentenciaRetornoNode : public SentenciaNode
{
private:
    ExpresionNode *expresion;

public:
    SentenciaRetornoNode();
    SentenciaRetornoNode(ExpresionNode *expresion);
    virtual ~SentenciaRetornoNode();
    void print(std::string prev, int localScope) override;
};

class SentCompuestaNode
{
private:
    std::vector<DeclaracionVarNode *> declaracionesLocales;
    std::vector<SentenciaNode *> sentencias;

public:
    SentCompuestaNode(std::vector<DeclaracionVarNode *> declaracionesLocales, std::vector<SentenciaNode *> sentencias);
    virtual ~SentCompuestaNode();
    void print(std::string prev, int localScope);
};

class ParamNodeAux
{
private:
    bool esArreglo;

public:
    ParamNodeAux(bool esArreglo) { this->esArreglo = esArreglo; }
    virtual ~ParamNodeAux() {}
    bool getArreglo() { return esArreglo; }
};
class ParamNode
{
private:
    std::string nombre;
    std::string tipo;
    bool esArreglo;

public:
    ParamNode(std::string nombre, std::string tipo, bool esArreglo)
    {
        this->nombre = nombre;
        this->tipo = tipo;
        this->esArreglo = esArreglo;
    }
    virtual ~ParamNode() {}
    void print(std::string prev, int i, int localScope)
    {
        if (symbolTable.isSymbol(nombre))
        {
            Symbol *&symbol = symbolTable.getSymbol(nombre);
            symbol->type = tipo;
            symbol->isArray = esArreglo;
            if (symbol->scope.count(localScope))
                errors.push_back("Symbol " + nombre + " already declared");
            else
                symbol->scope.insert(localScope);
        }
        else
            errors.push_back("Symbol " + nombre + " not founded\n");
        std::cout << prev << "Param " << i;
        if (this->esArreglo)
            std::cout << " es arreglo.\t";
        else
            std::cout << " no es arreglo.\t";
        std::cout << "ID=" << this->nombre << "\tTipo=" << this->tipo << "\n";
    }
};

class DeclaracionNodeAux
{
public:
    virtual bool getEsVarNode() = 0;
    virtual bool getEsArreglo() { return false; }
    virtual int getSize() { return 0; };
    virtual std::vector<ParamNode *> getParams() { return {}; };
    virtual SentCompuestaNode *getSentCompuestaNode() { return nullptr; };
    virtual ~DeclaracionNodeAux() {}
};
class DeclaracionVarNodeAux : public DeclaracionNodeAux
{
private:
    bool esVarNode;
    bool esArreglo;
    int size;

public:
    DeclaracionVarNodeAux()
    {
        this->esVarNode = true;
        this->esArreglo = false;
        this->size = 0;
    }
    DeclaracionVarNodeAux(int size)
    {
        this->esVarNode = true;
        this->esArreglo = true;
        this->size = size;
    }
    virtual ~DeclaracionVarNodeAux() {}
    virtual bool getEsArreglo() { return this->esArreglo; }
    int getSize() { return this->size; }
    bool getEsVarNode() override { return this->esVarNode; }
};
class DeclaracionFunNodeAux : public DeclaracionNodeAux
{
private:
    bool esVarNode;
    std::vector<ParamNode *> params;
    SentCompuestaNode *sentCompuestaNode;

public:
    DeclaracionFunNodeAux(std::vector<ParamNode *> params,
                          SentCompuestaNode *sentCompuestaNode)
    {
        this->params = params;
        this->sentCompuestaNode = sentCompuestaNode;
        this->esVarNode = false;
    }
    virtual ~DeclaracionFunNodeAux()
    {
        for (int i = 0; i < params.size(); i++)
        {
            if (params[i])
            {
                delete params[i];
                params[i] = nullptr;
            }
        }

        params.clear();
        if (sentCompuestaNode)
        {
            delete sentCompuestaNode;
            sentCompuestaNode = nullptr;
        }
    }
    std::vector<ParamNode *> getParams() { return this->params; }
    SentCompuestaNode *getSentCompuestaNode() { return this->sentCompuestaNode; }
    bool getEsVarNode() override { return this->esVarNode; }
};

class DeclaracionNode
{
public:
    virtual void print(std::string prev, int localScope) = 0;
    virtual ~DeclaracionNode() {}
};
class DeclaracionVarNode : public DeclaracionNode
{
private:
    std::string id;
    std::string tipo = "entero";
    bool esArreglo;
    int size;

public:
    DeclaracionVarNode(std::string id);
    DeclaracionVarNode(std::string id, int size);
    void print(std::string prev, int localScope) override;
    virtual ~DeclaracionVarNode() {}
};

class DeclaracionFunNode : public DeclaracionNode
{
private:
    std::string id;
    std::string tipo;
    std::vector<ParamNode *> params;
    SentCompuestaNode *sentCompuestaNode;

public:
    DeclaracionFunNode(std::string id, std::string tipo, std::vector<ParamNode *> params, SentCompuestaNode *sentCompuestaNode)
    {
        this->id = id;
        this->tipo = tipo;
        this->params = params;
        this->sentCompuestaNode = sentCompuestaNode;
    }
    virtual ~DeclaracionFunNode()
    {
        for (int i = 0; i < params.size(); i++)
        {
            if (params[i])
            {
                delete params[i];
                params[i] = nullptr;
            }
        }
        params.clear();
        delete sentCompuestaNode;
    }
    void print(std::string prev, int localScope) override
    {
        if (symbolTable.isSymbol(id))
        {
            Symbol *&symbol = symbolTable.getSymbol(id);
            symbol->type = tipo;
            symbol->isFunction = true;
            if (symbol->scope.count(localScope))
                errors.push_back("Symbol " + id + " already declared");
            else
                symbol->scope.insert(localScope);
        }
        else
            errors.push_back("Symbol " + id + " not founded\n");
        std::cout << prev << "Declaración Fun\tID=" << this->id << "\tTipo=" << this->tipo;
        std::cout << prev << "Tiene " << params.size() << " parámetro(s)\n";
        int i = 1;
        for (ParamNode *&param : params)
            param->print(prev + "\t", i++, localScope + 1);
        this->sentCompuestaNode->print(prev, localScope + 1);
    }
};

class ProgramaNode
{
private:
    std::vector<DeclaracionNode *> declaraciones;

public:
    ProgramaNode(std::vector<DeclaracionNode *> declaraciones)
    {
        this->declaraciones = declaraciones;
    }
    virtual ~ProgramaNode()
    {
        for (int i = 0; i < declaraciones.size(); i++)
        {
            if (declaraciones[i])
            {
                delete declaraciones[i];
                declaraciones[i] = nullptr;
            }
        }
        declaraciones.clear();
    }
    void print()
    {
        int scope = 0;
        std::cout << "Programa\n";
        std::cout << "Declaraciones: " << declaraciones.size() << "\n";
        for (DeclaracionNode *&declaracion : this->declaraciones)
            declaracion->print("\t", scope);
        std::cout << "Fin Programa\n\n";
        scope = 0;
    }
};