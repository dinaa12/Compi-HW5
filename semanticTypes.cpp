#include "semanticTypes.h"
#include "parserFuncs.h"

CodeBuffer& code_buff = CodeBuffer::instance();

static string sizeOfType(SemTypeName type) {
    if (type == "INT") return "i32";
    if (type == "BYTE") return "i8";
    if (type == "BOOL") return "i1";
    if (type == "STRING") return "i8*";
    return "void";
}

STM::STM() : type_name(code_buff.genLabel()) {}

STN::STN() : next_list(vector<pair<int,BranchLabelIndex>>()) {
    pair<int,BranchLabelIndex> new_pair;
    new_pair.first = code_buff.emit("br label @");
    new_pair.second = FIRST;
    next_list = CodeBuffer::makelist(new_pair);
}

STExp::STExp(SemTypeName t_name, string t_val, DERIVATION_RULE d_rule, SemType* s1, SemType* s2, SemType* s3) :
    type_name(t_name), type_value(t_val), reg(Reg()), true_list(std::vector<pair<int,BranchLabelIndex>>()), false_list(std::vector<pair<int,BranchLabelIndex>>()), next_list(std::vector<pair<int,BranchLabelIndex>>()) {

    string curr_code;
    string global_code;

    if (d_rule == EXP_TO_LPAREN_EXP_RPAREN) {
        if (s1->getTypeName() != "BOOL") {
            curr_code = reg.name + " = add " + sizeOfType(type_name) + " " + s1->getReg()->name + ", 0";
            code_buff.emit(curr_code);
        }
        // TODO: add truelist faselist nextlist ???????
    }
    else if (d_rule == EXP_TO_EXP_BINOP_ADD_EXP) {
        Reg* reg_operand1 = s1->getReg();
        Reg* reg_operand2 = s2->getReg();

        // check operands size
        if (s1->getTypeName() != s2->getTypeName()) {
            if (s1->getTypeName() == "BYTE") {
                Reg extended_reg;
                code_buff.emit(extended_reg.name + " = zext i8 " + reg_operand1->name + " to i32");
                reg_operand1 = &extended_reg;
            }
            else if (s2->getTypeName() == "BYTE") {
                Reg extended_reg;
                code_buff.emit(extended_reg.name + " = zext i8 " + reg_operand2->name + " to i32");
                reg_operand2 = &extended_reg;
            }
        }
        curr_code = reg.name + " = add " + sizeOfType(type_name) + " " + reg_operand1->name + ", " + reg_operand2->name;
        code_buff.emit(curr_code);
        // TODO: add truelist faselist nextlist ???????
    }
    else if (d_rule == EXP_TO_EXP_BINOP_SUB_EXP) {
        Reg* reg_operand1 = s1->getReg();
        Reg* reg_operand2 = s2->getReg();

        // check operands size
        if (s1->getTypeName() != s2->getTypeName()) {
            if (s1->getTypeName() == "BYTE") {
                Reg extended_reg;
                code_buff.emit(extended_reg.name + " = zext i8 " + reg_operand1->name + " to i32");
                reg_operand1 = &extended_reg;
            }
            else if (s2->getTypeName() == "BYTE") {
                Reg extended_reg;
                code_buff.emit(extended_reg.name + " = zext i8 " + reg_operand2->name + " to i32");
                reg_operand2 = &extended_reg;
            }
        }

        curr_code = reg.name + " = sub " + sizeOfType(type_name) + " " + reg_operand1->name + ", " + reg_operand2->name;
        code_buff.emit(curr_code);
        // TODO: add truelist faselist nextlist ???????
    }
    else if (d_rule == EXP_TO_EXP_BINOP_MUL_EXP) {
        Reg* reg_operand1 = s1->getReg();
        Reg* reg_operand2 = s2->getReg();

        // check operands size
        if (s1->getTypeName() != s2->getTypeName()) {
            if (s1->getTypeName() == "BYTE") {
                Reg extended_reg;
                code_buff.emit(extended_reg.name + " = zext i8 " + reg_operand1->name + " to i32");
                reg_operand1 = &extended_reg;
            }
            else if (s2->getTypeName() == "BYTE") {
                Reg extended_reg;
                code_buff.emit(extended_reg.name + " = zext i8 " + reg_operand2->name + " to i32");
                reg_operand2 = &extended_reg;
            }
        }

        curr_code = reg.name + " = mul " + sizeOfType(type_name) + " " + reg_operand1->name + ", " + reg_operand2->name;
        code_buff.emit(curr_code);
        // TODO: add truelist faselist nextlist ???????
    }
    else if (d_rule == EXP_TO_EXP_BINOP_DIV_EXP) {
        Reg* reg_operand1 = s1->getReg();
        Reg* reg_operand2 = s2->getReg();
        string reg_operand2_size = sizeOfType(s2->getTypeName());

        // check operands size
        if (s1->getTypeName() != s2->getTypeName()) {
            if (s1->getTypeName() == "BYTE") {
                Reg extended_reg;
                code_buff.emit(extended_reg.name + " = zext i8 " + reg_operand1->name + " to i32");
                reg_operand1 = &extended_reg;
            }
            else if (s2->getTypeName() == "BYTE") {
                Reg extended_reg;
                code_buff.emit(extended_reg.name + " = zext i8 " + reg_operand2->name + " to i32");
                reg_operand2 = &extended_reg;
            }
        }

        // check division by 0
            // conditional branch code
        Reg cond_reg;
        code_buff.emit(cond_reg.name + " = icmp eq " + reg_operand2_size + " " + reg_operand2->name + ", 0");
        int br_cmd_addr = code_buff.emit("br i1 " + cond_reg.name + ", label @, label @");
        string equal_label = code_buff.genLabel();
        Reg div_by_0_reg;
        code_buff.emit(div_by_0_reg.name + " = getelementptr [23 x i8], [23 x i8]* @.div_error, i32 0, i32 0");
        code_buff.emit("call void @print(i8* " + div_by_0_reg.name + ")");
        code_buff.emit("call void @exit(i32 0)");
        int div_by_0_br_cmd_addr = code_buff.emit("br label @");
        string unequal_label = code_buff.genLabel();
            // backpatching
        code_buff.bpatch(CodeBuffer::makelist(pair<int,BranchLabelIndex>(br_cmd_addr, FIRST)), equal_label);
        code_buff.bpatch(CodeBuffer::makelist(pair<int,BranchLabelIndex>(br_cmd_addr, SECOND)), unequal_label);
        code_buff.bpatch(CodeBuffer::makelist(pair<int,BranchLabelIndex>(div_by_0_br_cmd_addr, FIRST)), unequal_label);

        // division code
        curr_code = reg.name + " = ";
        if (type_name == "BYTE") {
            curr_code += "udiv ";
        }
        else {
            curr_code += "sdiv ";
        }
        curr_code += sizeOfType(type_name) + " " + reg_operand1->name + ", " + reg_operand2->name;
        code_buff.emit(curr_code);
        // TODO: add truelist faselist nextlist ???????
    }
    else if (d_rule == EXP_TO_ID) {
        int id_offset = getEntryByName(s1->getTypeValue())->offset;
        Reg new_reg;
        if (id_offset < 0) { // function argument
            string arg_index = to_string((-1 * id_offset) - 1);
            curr_code = new_reg.name + " = add " + sizeOfType(type_name) + " %" + arg_index + ", 0";
            code_buff.emit(curr_code);
        }
        else { // local variable
            Reg addr_reg;
            string local_index = to_string(id_offset);
            code_buff.emit(addr_reg.name + " = getelementptr [50 x i32], [50 x i32]* " + local_variables_reg.name + ", i32 0, i32 " + local_index);
            if (sizeOfType(type_name) != "i32") { // type is BOOL or BYTE
                Reg extended_reg;
                code_buff.emit(extended_reg.name + " load i32, i32* " + addr_reg.name);
                code_buff.emit(new_reg.name + " = trunc i32 "+ extended_reg.name + " to " + sizeOfType(type_name));
            }
            else {
                code_buff.emit(new_reg.name + " = load i32, i32* " + addr_reg.name);
            }
            if (s1->getTypeName() == "BOOL") {
                Reg cond_reg;
                code_buff.emit(cond_reg.name + " = icmp eq i1 " + new_reg.name + ", 1");
                int br_cmd_addr = code_buff.emit("br i1 " + cond_reg.name + ", label @, label @");
                pair<int, BranchLabelIndex> curr_list;
                curr_list.first = br_cmd_addr;
                curr_list.second = FIRST;
                true_list = CodeBuffer::makelist((curr_list));
                curr_list.second = SECOND;
                false_list = CodeBuffer::makelist((curr_list));
            }
        }
    }
    else if (d_rule == EXP_TO_CALL) {
        if (type_name == "BOOL") {
            Reg cond_reg;
            code_buff.emit(cond_reg.name + " = icmp eq i1 " + (s1->getReg())->name + ", 1");
            int br_cmd_addr = code_buff.emit("br i1 " + cond_reg.name + ", label @, label @");
            pair<int, BranchLabelIndex> curr_list;
            curr_list.first = br_cmd_addr;
            curr_list.second = FIRST;
            true_list = CodeBuffer::makelist((curr_list));
            curr_list.second = SECOND;
            false_list = CodeBuffer::makelist((curr_list));
        }
        else {
            curr_code = reg.name + " = add " + sizeOfType(s1->getTypeName()) + " " + (s1->getReg())->name + ", 0";
            code_buff.emit(curr_code);
        }
    }
    else if (d_rule == EXP_TO_NUM) {
        code_buff.emit(reg.name + " = add i32 " + t_val + ", 0");
    }
    else if (d_rule == EXP_TO_NUM_B) {
        code_buff.emit(reg.name + " = add i8 " + t_val + ", 0");
    }
    else if (d_rule == EXP_TO_STRING) {
        global_code = "@." + s1->getStringName() + " = internal constant [" + to_string(s1->getTypeValue().length()) + " x i8] c\"" + s1->getTypeValue() + "\\00\"";
        curr_code = reg.name + " = getelementptr [" + to_string(s1->getTypeValue().length()) + " x i8], [" + to_string(s1->getTypeValue().length()) + " x i8]* @." + s1->getStringName() + ", i32 0, i32 0";
        code_buff.emitGlobal(global_code);
        code_buff.emit(curr_code);
    }
    else if (d_rule == EXP_TO_TRUE) {
        pair<int, BranchLabelIndex> curr_list;
        int br_cmd_addr = code_buff.emit("br label @");
        curr_list.first = br_cmd_addr;
        curr_list.second = FIRST;
        true_list = CodeBuffer::makelist(curr_list);
    }
    else if (d_rule == EXP_TO_FALSE) {
        pair<int, BranchLabelIndex> curr_list;
        int br_cmd_addr = code_buff.emit("br label @");
        curr_list.first = br_cmd_addr;
        curr_list.second = FIRST;
        false_list = CodeBuffer::makelist(curr_list);
    }
    else if (d_rule == EXP_TO_NOT_EXP) {
        true_list = s1->getFalseList();
        false_list = s1->getTrueList();
    }
    else if (d_rule == EXP_TO_EXP_AND_EXP) {
        code_buff.bpatch(s1->getTrueList(), "AND");
        false_list = CodeBuffer::merge(s1->getFalseList(), s2->getFalseList());
        true_list = s2->getTrueList();
    }
    else if (d_rule == EXP_TO_EXP_OR_EXP) {
        code_buff.bpatch(s1->getFalseList(), "OR");
        true_list = CodeBuffer::merge(s1->getTrueList(), s2->getTrueList());
        false_list = s2->getFalseList();
    }
    else if (d_rule == EXP_TO_EXP_RELOP_COMPARE_EXP) {
        Reg* reg_operand1 = s1->getReg();
        Reg* reg_operand2 = s2->getReg();
        Reg cmp_reg;

        // check operands size
        if (s1->getTypeName() != s2->getTypeName()) {
            if (s1->getTypeName() == "BYTE") {
                Reg extended_reg;
                code_buff.emit(extended_reg.name + " = zext i8 " + reg_operand1->name + " to i32");
                reg_operand1 = &extended_reg;
            }
            else if (s2->getTypeName() == "BYTE") {
                Reg extended_reg;
                code_buff.emit(extended_reg.name + " = zext i8 " + reg_operand2->name + " to i32");
                reg_operand2 = &extended_reg;
            }
        }

        curr_code = cmp_reg.name + " = icmp ";
        if (s3->getTypeValue() == "<") {
            curr_code += "slt ";
        }
        else if (s3->getTypeValue() == ">") {
            curr_code += "sgt ";
        }
        else if (s3->getTypeValue() == "<=") {
            curr_code += "sle ";
        }
        else { // (s3->getTypeValue() == ">=")
            curr_code += "sge ";
        }

        curr_code += sizeOfType(checkBINOPResType(s1->getTypeName(), s2->getTypeName())) + " " + reg_operand1->name + ", " + reg_operand2->name;
        code_buff.emit(curr_code);
        int br_cmd_addr = code_buff.emit("br i1 " + cmp_reg.name + ", label @, label @");
        pair<int, BranchLabelIndex> curr_list;
        curr_list.first = br_cmd_addr;
        curr_list.second = FIRST;
        true_list = CodeBuffer::makelist((curr_list));
        curr_list.second = SECOND;
        false_list = CodeBuffer::makelist((curr_list));
    }
    else if (d_rule == EXP_TO_EXP_RELOP_EQUAL_EXP) {
        Reg* reg_operand1 = s1->getReg();
        Reg* reg_operand2 = s2->getReg();
        Reg cmp_reg;

        // check operands size
        if (s1->getTypeName() != s2->getTypeName()) {
            if (s1->getTypeName() == "BYTE") {
                Reg extended_reg;
                code_buff.emit(extended_reg.name + " = zext i8 " + reg_operand1->name + " to i32");
                reg_operand1 = &extended_reg;
            }
            else if (s2->getTypeName() == "BYTE") {
                Reg extended_reg;
                code_buff.emit(extended_reg.name + " = zext i8 " + reg_operand2->name + " to i32");
                reg_operand2 = &extended_reg;
            }
        }

        curr_code = cmp_reg.name + " = icmp ";
        if (s3->getTypeValue() == "!=") {
            curr_code += "ne ";
        }
        else { // (s3->getTypeValue() == "==")
            curr_code += "eq ";
        }

        curr_code += sizeOfType(checkBINOPResType(s1->getTypeName(), s2->getTypeName())) + " " + reg_operand1->name + ", " + reg_operand2->name;
        code_buff.emit(curr_code);
        int br_cmd_addr = code_buff.emit("br i1 " + cmp_reg.name + ", label @, label @");
        pair<int, BranchLabelIndex> curr_list;
        curr_list.first = br_cmd_addr;
        curr_list.second = FIRST;
        true_list = CodeBuffer::makelist((curr_list));
        curr_list.second = SECOND;
        false_list = CodeBuffer::makelist((curr_list));
    }
    else if (d_rule == EXP_TO_LPAREN_TYPE_RPAREN_EXP) {
        if (sizeOfType(type_name) == sizeOfType(s1->getTypeName())) {
            string source_op = s1->getReg()->name;
            code_buff.emit(reg.name + " = add " + sizeOfType(type_name) + " " + source_op + ", 0");
        }
        else if (sizeOfType(type_name) == "i32" && sizeOfType(s1->getTypeName()) == "i8") {
            string source_op = s1->getReg()->name;
            code_buff.emit(reg.name + " = trunc i32 " + source_op + " to i8");
        }
        else if (sizeOfType(type_name) == "i8" && sizeOfType(s1->getTypeName()) == "i32") {
            string source_op = s1->getReg()->name;
            code_buff.emit(reg.name + " = zext i8 " + source_op + " to i32");
        }
    }
    else {
        std::cerr << "DERIVATION RULE ERROR" << std::endl;
    }
}

