#ifndef HW3_SYMBOLTABLE_H
#define HW3_SYMBOLTABLE_H

#include <iostream>
#include <vector>
#include <string>
#include "semanticTypes.h"
#include "hw3_output.hpp"

using namespace std;

class SymbolTableEntry {
public:
    string name;
    int offset;
    vector<SemTypeName> type; // if symbol is variable - vector size = 1, if symbol is func - vector = {FUNC, arg1 type, ..., argN type, ret type}
    bool is_func;

    SymbolTableEntry(string name, int offset, vector<SemTypeName> type, bool is_func = false) :
        name(name), offset(offset), type(type), is_func(is_func) {};
    ~SymbolTableEntry() = default;
};

class SymbolTable {
public:
    vector<SymbolTableEntry*> table;

    SymbolTable() = default;
    ~SymbolTable() {
        for (auto e : table) {
            delete e;
        }
    }
    void insertEntry(SymbolTableEntry* e);
    void printTable();
    SymbolTableEntry* getEntryByNameInTable(string name);
};




#endif //HW3_SYMBOLTABLE_H
