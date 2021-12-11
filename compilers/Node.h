#pragma once

#include <iostream>
#include <string>
#include <vector>
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
    virtual void print(std::string prev) = 0;
};
class FactorExpresion : public Factor
{
private:
    ExpresionNode *expresion;

public:
    FactorExpresion(ExpresionNode *expresion);
    void print(std::string prev) override;
};
class FactorVar : public Factor
{
private:
    VarNode *varNode;

public:
    FactorVar(VarNode *varNode);
    void print(std::string prev) override;
};
class FactorCall : public Factor
{
private:
    std::string id;
    std::vector<ExpresionNode *> lista_arg;

public:
    FactorCall(std::string id, std::vector<ExpresionNode *> lista_arg);
    void print(std::string prev) override;
};
class FactorNum : public Factor
{
private:
    int numero;

public:
    FactorNum(int numero);
    void print(std::string prev) override;
};
class Term
{
private:
    Term *term;
    std::string mulop;
    Factor *factor;

public:
    Term(Term *term, std::string mulop, Factor *factor)
    {
        this->term = term;
        this->mulop = mulop;
        this->factor = factor;
    }
    Term(Factor *factor)
    {
        this->term = nullptr;
        this->mulop = "";
        this->factor = factor;
    }
    void print(std::string prev)
    {
        std::cout << prev << "Term. ";
        if (this->term == nullptr)
        {
            std::cout << "Solo factor\n";
            this->factor->print(prev + "\t");
        }
        else
        {
            std::cout << this->mulop << "\n";
            this->term->print(prev + "\t");
            this->factor->print(prev + "\t");
        }
    }
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
                     Term *term)
    {
        this->expresionAditiva = expresionAditiva;
        this->addop = addop;
        this->term = term;
    }
    ExpresionAditiva(Term *term)
    {
        this->expresionAditiva = nullptr;
        this->addop = "";
        this->term = term;
    }
    void print(std::string prev)
    {
        std::cout << prev << "Expresión aditiva. ";
        if (this->expresionAditiva == nullptr)
        {
            std::cout << "Solo term\n";
            this->term->print(prev + "\t");
        }
        else
        {
            std::cout << this->addop << "\n";
            this->expresionAditiva->print(prev + "\t");
            this->term->print(prev + "\t");
        }
    }
};

class VarNodeAux
{
private:
    ExpresionNode *expresion;
    bool esArreglo;

public:
    VarNodeAux(ExpresionNode *expresion)
    {
        this->expresion = expresion;
        this->esArreglo = true;
    }
    VarNodeAux()
    {
        this->expresion = nullptr;
        this->esArreglo = false;
    }
    bool getEsArreglo() { return esArreglo; }
    ExpresionNode *getIndice() { return expresion; }
};
class VarNode
{
private:
    std::string id;
    VarNodeAux *varNodeAux;

public:
    VarNode(std::string id, VarNodeAux *varNodeAux)
    {
        this->id = id;
        this->varNodeAux = varNodeAux;
    }
    std::string getId() { return id; }
    ExpresionNode *getIndice() { return varNodeAux->getIndice(); }
    bool getEsArreglo() { return varNodeAux->getEsArreglo(); }
};

class ExpresionNode
{
public:
    virtual void print(std::string prev) = 0;
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
    void print(std::string prev) override;
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
    void print(std::string prev) override;
};

class SentenciaNode
{
public:
    virtual void print(std::string prev) = 0;
};
class SentenciaExpresionNode : public SentenciaNode
{
private:
    ExpresionNode *expresion;

public:
    SentenciaExpresionNode(ExpresionNode *expresion) { this->expresion = expresion; }
    SentenciaExpresionNode() { this->expresion = nullptr; }
    void print(std::string prev) override
    {
        std::cout << prev << "Sentencia Expresión\n";
        this->expresion->print(prev + "\t");
    }
};
class SentenciaSeleccionNode : public SentenciaNode
{
private:
    ExpresionNode *expresionNode;
    SentenciaNode *sentencia;
    SentenciaNode *sentenciaElse;

public:
    SentenciaSeleccionNode(ExpresionNode *expresionNode,
                           SentenciaNode *sentencia)
    {
        this->expresionNode = expresionNode;
        this->sentencia = sentencia;
        this->sentenciaElse = nullptr;
    }
    SentenciaSeleccionNode(ExpresionNode *expresionNode,
                           SentenciaNode *sentencia,
                           SentenciaNode *sentenciaElse)
    {
        this->expresionNode = expresionNode;
        this->sentencia = sentencia;
        this->sentenciaElse = sentenciaElse;
    }

    void print(std::string prev) override
    {
        std::cout << prev << "Sentencia Selección\n";
        std::cout << prev + "\t"
                  << "Expresión de condición\n";
        this->expresionNode->print(prev + "\t\t");
        std::cout << prev + "\t"
                  << "Sentencia Then\n";
        this->sentencia->print(prev + "\t\t");
        if (this->sentenciaElse != nullptr)
        {
            std::cout << prev + "\t"
                      << "Sentencia Else\n";
            this->sentenciaElse->print(prev + "\t\t");
        }
    }
};
class SentenciaIteracionNode : public SentenciaNode
{
private:
    ExpresionNode *expresion;
    std::vector<SentenciaNode *> sentencias;

public:
    SentenciaIteracionNode(ExpresionNode *expresion,
                           std::vector<SentenciaNode *> sentencias)
    {
        this->expresion = expresion;
        this->sentencias = sentencias;
    }
    void print(std::string prev) override
    {
        std::cout << prev << "\tSentencia Iteración\n";
        std::cout << prev << "\tExpresión de condición:\n";
        this->expresion->print(prev + "\t\t");
        std::cout << prev << "\tSentencias de iteración: " << this->sentencias.size() << "\n";
        int i = 1;
        for (SentenciaNode *&sentencia : sentencias)
        {
            std::cout << prev + "\t"
                      << "Sentencia " << i++ << ":";
            sentencia->print(prev + "\t\t");
        }
    }
};
class SentenciaRetornoNode : public SentenciaNode
{
private:
    ExpresionNode *expresion;

public:
    SentenciaRetornoNode()
    {
        this->expresion = nullptr;
    }
    SentenciaRetornoNode(ExpresionNode *expresion)
    {
        this->expresion = expresion;
    }
    void print(std::string prev) override
    {
        std::cout << prev << "Sentencia Retorno\n";
        if (this->expresion == nullptr)
            std::cout << prev + "\tNo retorna nada\n";
        else
        {
            std::cout << prev + "\tRetorna una expresión\n";
            this->expresion->print(prev + "\t");
        }
    }
};

class SentCompuestaNode
{
private:
    std::vector<DeclaracionVarNode *> declaracionesLocales;
    std::vector<SentenciaNode *> sentencias;

public:
    SentCompuestaNode(std::vector<DeclaracionVarNode *> declaracionesLocales, std::vector<SentenciaNode *> sentencias);
    void print(std::string prev);
};

class ParamNodeAux
{
private:
    bool esArreglo;

public:
    ParamNodeAux(bool esArreglo) { this->esArreglo = esArreglo; }
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
    void print(std::string prev, int i)
    {
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
    std::vector<ParamNode *> getParams() { return this->params; }
    SentCompuestaNode *getSentCompuestaNode() { return this->sentCompuestaNode; }
    bool getEsVarNode() override { return this->esVarNode; }
};

class DeclaracionNode
{
public:
    virtual void print(std::string prev) = 0;
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
    void print(std::string prev) override;
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
    void print(std::string prev) override
    {
        std::cout << prev << "Declaración Fun\tID=" << this->id << "\tTipo=" << this->tipo;
        std::cout << prev << "Tiene " << params.size() << " parámetro(s)\n";
        int i = 1;
        for (ParamNode *&param : params)
            param->print(prev + "\t", i++);
        this->sentCompuestaNode->print(prev);
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
    void print()
    {
        std::cout << "Programa\n";
        std::cout << "Declaraciones: " << declaraciones.size() << "\n";
        for (DeclaracionNode *&declaracion : this->declaraciones)
            declaracion->print("\t");
        std::cout << "Fin Programa\n\n";
    }
};