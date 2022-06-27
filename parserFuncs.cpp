#include "parserFuncs.h"

vector<SymbolTable*> tables_stack;
vector<int> offset_stack;
int while_counter = 0;

SymbolTableEntry* getEntryByName(string name) {
    for (vector<SymbolTable*>::iterator it = tables_stack.begin(); it != tables_stack.end(); ++it) {
        SymbolTableEntry* entry = (*it)->getEntryByNameInTable(name);
        if (entry) {
            return entry;
        }
    }
    return nullptr;
}

void checkMainExist() {
    SymbolTableEntry* main_entry = getEntryByName("main");
    if (!main_entry || !main_entry->is_func || main_entry->type.size() != 2 || main_entry->type[1] != "VOID") {
        output::errorMainMissing();
        exit(0);
    }
}

void checkType(SemTypeName t1, SemTypeName t2, int line) {
    if (t1 != t2) {
        output::errorMismatch(line);
        exit(0);
    }
}

void checkTypeNumeric(SemTypeName t, int line) {
    if (t != "INT" && t != "BYTE") {
        output::errorMismatch(line);
        exit(0);
    }
}

void checkBValid(string val, int line) {
    int num_val = stoi(val);
    if (num_val < 0 || num_val > 255) {
        output::errorByteTooLarge(line, val);
        exit(0);
    }
}

SemTypeName checkBINOPResType(SemTypeName t1, SemTypeName t2) {
    if (t1 == "BYTE" && t2 == "BYTE") {
        return "BYTE";
    }
    return "INT";
}

SemTypeName checkIDInSymTable(string sym_name, int line) {
    SymbolTableEntry* entry = getEntryByName(sym_name);
    if (!entry || entry->is_func) {
        output::errorUndef(line, sym_name);
        exit(0);
    }
    return entry->type[0];
}

SemTypeName checkFuncInSymTable(string func_name, int line) {
    SymbolTableEntry* entry = getEntryByName(func_name);
    if (!entry || !entry->is_func) {
        output::errorUndefFunc(line, func_name);
        exit(0);
    }
    return entry->type[entry->type.size()-1];
}

void checkFuncArgsInTable(string func_name, int line, vector<SemTypeName> arg) {
    SymbolTableEntry* entry = getEntryByName(func_name);
    vector<SemTypeName> entry_args(entry->type);
    entry_args.erase(entry_args.begin());
    entry_args.pop_back();

    //cout << arg[0] << endl;

    if (arg.size() == 0) {
        if (entry_args.size() != 0) {
            output::errorPrototypeMismatch(line, func_name, entry_args);
            exit(0);
        }
    }
    else {
        if (entry_args.size() != arg.size()) {
            output::errorPrototypeMismatch(line, func_name, entry_args);
            exit(0);
        }
        for (int i = 0; i < arg.size(); i++) {
            if (arg[i] != entry_args[i] && !(arg[i] == "BYTE" && entry_args[i] == "INT")) {
                output::errorPrototypeMismatch(line, func_name, entry_args);
                exit(0);
            }
        }
    }
}

void checkInWhile(char x, int line) {
    if (while_counter == 0) {
        if (x == 'B') {
            output::errorUnexpectedBreak(line);
        }
        else {
            output::errorUnexpectedContinue(line);
        }
        exit(0);
    }
}

void openScope() {

    tables_stack.push_back(new SymbolTable());
    if (offset_stack.empty()) {
        offset_stack.push_back(0);
    }
    else {
        offset_stack.push_back(offset_stack[offset_stack.size()-1]);
    }
}

void closeScope() {
    //output::endScope();
    /* pop and print current symbol table */
    SymbolTable* curr_table = tables_stack.back();
    curr_table->SymbolTable::printTable();
    tables_stack.pop_back();
    delete curr_table;
    /* pop offset stack */
    offset_stack.pop_back();

}

void checkRetVal(SemTypeName t, int line) {
    SymbolTable* sym_table = tables_stack.front();
    SymbolTableEntry* func_entry = sym_table->table.back();
    SemTypeName ret_val = func_entry->type.back();
    if (ret_val != t && !(ret_val == "INT" && t == "BYTE")) {
        output::errorMismatch(line);
        exit(0);
    }
}

void checkValidAssign(SemTypeName t1, SemTypeName t2, int line) {
    if (t1 == "FUNC") {
        output::errorMismatch(line);
        exit(0);
    }
    if (!(t1 == t2 || (t1 == "INT" && t2 == "BYTE"))) {
        output::errorMismatch(line);
        exit(0);
    }
}

void addIDToSymTable(string sym_name, SemTypeName t, int line) {
    SymbolTableEntry* entry = getEntryByName(sym_name);
    if (entry) {
        output::errorDef(line, sym_name);
        exit(0);
    }
    SymbolTable* sym_table = tables_stack.back();
    SymbolTableEntry* entry_to_insert = new SymbolTableEntry(sym_name, offset_stack.back(), vector<SemTypeName>(1, t));
    sym_table->insertEntry(entry_to_insert);
    offset_stack[offset_stack.size() - 1]++;
}

void addFuncToSymTable(string func_name, SemTypeName ret_type, vector<SemTypeName> args_types, int line) {
    SymbolTableEntry* entry = getEntryByName(func_name);
    if (entry) {
        output::errorDef(line, func_name);
        exit(0);
    }
    vector<SemTypeName> types;
    types.push_back("FUNC");
    types.insert(types.end(), args_types.begin(), args_types.end());
    types.push_back(ret_type);
    SymbolTable* sym_table = tables_stack.back();
    SymbolTableEntry* entry_to_insert = new SymbolTableEntry(func_name, 0, types, true);
    sym_table->insertEntry(entry_to_insert);
}

void _checkUniqueArgs(vector<string> args_names, int line) {
    for (int i = 0; i < args_names.size(); i++) {
        for (int j = i+1; j < args_names.size(); j++) {
            if (args_names[i] == args_names[j]) {
                output::errorDef(line, args_names[i]);
                exit(0);
            }
        }
    }
}

void addFuncArgsToSymTable(vector<SemTypeName> args_types, vector<string> args_names, int line) {
    _checkUniqueArgs(args_names, line);
    if (!args_types.empty()) {
        SymbolTable* sym_table = tables_stack.back();
        int arg_offset = -1;
        for (int i = 0; i < args_types.size(); i++) {
            SymbolTableEntry* entry_to_insert = new SymbolTableEntry(args_names[i], arg_offset, vector<SemTypeName>(1, args_types[i]));
            sym_table->insertEntry(entry_to_insert);
            arg_offset--;
        }
    }
}

void createCodeBoolExpInExplist(SemType* exp) {
	if (exp->getTypeName() == "BOOL") {
        string true_label = code_buff.genLabel();
        code_buff.bpatch(exp->getTrueList(), true_label);
        int br_true_label_cmd_addr = code_buff.emit("br label @");
        pair<int,BranchLabelIndex> true_label_list_item;
        true_label_list_item.first = br_true_label_cmd_addr;
        true_label_list_item.second = FIRST;

        string false_label = code_buff.genLabel();
        code_buff.bpatch(exp->getFalseList(), false_label);
        int br_false_label_cmd_addr = code_buff.emit("br label @");
        pair<int,BranchLabelIndex> false_label_list_item;
        false_label_list_item.first = br_false_label_cmd_addr;
        false_label_list_item.second = FIRST;

        vector<pair<int,BranchLabelIndex>> curr_list_item_to_patch;
        curr_list_item_to_patch.push_back(true_label_list_item);
        curr_list_item_to_patch.push_back(false_label_list_item);
        string end_label = code_buff.genLabel();
        code_buff.bpatch(curr_list_item_to_patch, end_label);

        code_buff.emit(exp->getReg()->name + " = phi i1 [1, %" + true_label + "], [0, %" + false_label + "]");
    }
}

void emitToCodeAllocateStack() {
	code_buff.emit(local_variables_reg.add() + " = alloca [50 x i32]");
}

void emitToCodeDefaultReturn(string ret_type) {
	if (ret_type == "VOID") {
		code_buff.emit("ret void");
	}
	else {
		code_buff.emit("ret " + sizeOfType(ret_type) + " 0");
	}
	code_buff.emit("}");
	code_buff.emit(""); // add new line
}

void emitToCodeFunctionDefinition(string ret_type, string func_name, vector<string> args_types) {
	string curr_code;
	curr_code = "define " + sizeOfType(ret_type) + " @" + func_name + "(";
	for (auto it = args_types.begin(); it != args_types.end(); ++it) {
		curr_code += sizeOfType((*it)) + ", ";
	}
	if (!args_types.empty()) { // remove '@('
		curr_code = curr_code.substr(0, curr_code.size() - 2);
	}
	curr_code += ") {";
	code_buff.emit(curr_code);
}

void declareGlobalFunctions() {
	code_buff.emitGlobal("declare i32 @printf(i8*, ...)");
	code_buff.emitGlobal("declare void @exit(i32)");
	code_buff.emitGlobal("@.int_specifier = constant [4 x i8] c\"%d\\0A\\00\"");
	code_buff.emitGlobal("@.str_specifier = constant [4 x i8] c\"%s\\0A\\00\"");

	code_buff.emit("define void @printi(i32) {");
	code_buff.emit("%spec_ptr = getelementptr [4 x i8], [4 x i8]* @.int_specifier, i32 0, i32 0");
	code_buff.emit("call i32 (i8*, ...) @printf(i8* %spec_ptr, i32 %0)");
	code_buff.emit("ret void");
	code_buff.emit("}");
	code_buff.emit("");

	code_buff.emit("define void @print(i8*) {");
	code_buff.emit("%spec_ptr = getelementptr [4 x i8], [4 x i8]* @.str_specifier, i32 0, i32 0");
	code_buff.emit("call i32 (i8*, ...) @printf(i8* %spec_ptr, i8* %0)");
	code_buff.emit("ret void");
	code_buff.emit("}");
	code_buff.emit("");
}

void declareDivisionByZeroMessage() {
	code_buff.emitGlobal("@.div_error = internal constant [23 x i8] c\"Error division by zero\\00\"");
}