#include "register.h"

LocalVariablesReg local_variables_reg;

Reg::Reg() {
    name = "%reg" + to_string(reg_counter);
    reg_counter+=2;
}

string LocalVariablesReg::add() {
    name = "%local_variables_reg" + to_string(local_variables_reg_counter);
    local_variables_reg_counter++;
    return name;
}