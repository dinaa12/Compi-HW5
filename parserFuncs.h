#ifndef HW3_PARSERFUNCS_H
#define HW3_PARSERFUNCS_H

#include <iostream>
#include <string>
#include "symbolTable.h"
#include "semanticTypes.h"
#include "hw3_output.hpp"

using namespace std;

extern vector<SymbolTable*> tables_stack;
extern vector<int> offset_stack;
extern int while_counter;
//extern SymbolTableEntry* getEntryByName(string name);

void checkMainExist();

void checkType(SemTypeName t1, SemTypeName t2, int line);

void checkTypeNumeric(SemTypeName t, int line);

void checkBValid(string val, int line);

// check binop result type (int or byte) and return the type
SemTypeName checkBINOPResType(SemTypeName t1, SemTypeName t2);

SemTypeName checkIDInSymTable(string sym_name, int line);

SemTypeName checkFuncInSymTable(string func_name, int line);

void checkFuncArgsInTable(string func_name, int line, vector<SemTypeName> arg);

// x = B - for break, x = C - for continue
void checkInWhile(char x, int line);

void openScope();

void closeScope();

void checkRetVal(SemTypeName t, int line);

void checkValidAssign(SemTypeName t1, SemTypeName t2, int line);

void addIDToSymTable(string sym_name, SemTypeName t, int line);

void addFuncToSymTable(string func_name, SemTypeName ret_type, vector<SemTypeName> args_types, int line);

void addFuncArgsToSymTable(vector<SemTypeName> args_types, vector<string> args_names, int line);

void createCodeBoolExpInExplist(SemType* exp = nullptr);

void emitToCodeAllocateStack();

//void emitToCodeExplicitReturn(SemType* exp = nullptr);

void emitToCodeDefaultReturn(string ret_type);

void emitToCodeFunctionDefinition(string ret_type, string func_name, vector<std::string> args_types);

#endif //HW3_PARSERFUNCS_H
