#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class Symbol
{
public:
    Symbol() : name("") {}
    Symbol(std::string name, int row, int col) : name(name), row(row), col(col) {}
    std::string getName() { return this->name; }
    int getRow() { return this->row; }
    int getCol() { return this->col; }
    void printSymbol()
    {
        std::cout << name << " Row: " << row << " Col: " << col << "\n";
    }

private:
    std::string name;
    int row;
    int col;
};

class SymbolTable
{
public:
    SymbolTable() {}
    void addSymbol(Symbol symbol)
    {
        std::string name = symbol.getName();
        this->symbols[name] = symbol;
        if (notValid.count(name))
            notValid.erase(name);
    }
    bool isSymbol(std::string name)
    {
        return symbols.count(name) && !notValid.count(name);
    }
    void deleteSymbol(std::string name)
    {
        notValid.insert(name);
    }
    Symbol getSymbol(std::string name)
    {
        if (notValid.count(name))
            return Symbol();
        return symbols[name];
    }
    void printSymbols()
    {
        std::cout << "\tSymbols\n";
        for (auto p : symbols)
            p.second.printSymbol();
        std::cout << "\tSymbols not valids\n";
        for (auto s : notValid)
            std::cout << s << "\n";
    }

private:
    std::unordered_map<std::string, Symbol> symbols;
    std::unordered_set<std::string> notValid;
};