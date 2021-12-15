#pragma once

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class Symbol
{
public:
    std::string name;
    int row;
    int col;
    std::unordered_set<int> scope;
    std::string type;
    bool isArray = false;
    bool isFunction = false;

    Symbol(std::string name, int row, int col) : name(name), row(row), col(col)
    {
        type = "";
    }
};

class SymbolTable
{
public:
    SymbolTable() {}
    virtual ~SymbolTable()
    {
        for (auto &p : this->symbols)
        {
            delete p.second;
            p.second = nullptr;
        }
        this->symbols.clear();
    }
    void addSymbol(Symbol *symbol)
    {
        std::string name = symbol->name;
        this->symbols[name] = symbol;
    }
    bool isSymbol(std::string name)
    {
        return symbols.count(name);
    }
    void deleteSymbol(std::string name, int scope)
    {
        Symbol *&tmp = this->symbols[name];
        tmp->scope.erase(scope);
    }

    Symbol *&getSymbol(std::string name)
    {
        return symbols[name];
    }
    void printSymbols()
    {
        std::cout << "\tSymbols\n";
        for (auto p : symbols)
        {
            std::cout << "Name: " << p.second->name;
            std::cout << " Row: " << p.second->row;
            std::cout << " Col: " << p.second->col;
            std::cout << " Scope: ";
            for (auto sc : p.second->scope)
                std::cout << sc << " ";
            if (p.second->isFunction)
            {
                std::cout << " es función";
                std::cout << " Type: " << p.second->type;
            }
            else
            {
                std::cout << " Type: " << p.second->type;
                std::cout << " isArray?: " << p.second->isArray;
                if (p.second->isArray)
                {
                    std::cout << " size: ";
                }
            }
            std::cout << "\n";
        }
    }

private:
    std::unordered_map<std::string, Symbol *> symbols;
};