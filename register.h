#ifndef HW5_REGISTER_H
#define HW5_REGISTER_H

#include <iostream>
#include <string>

using namespace std;

static unsigned int reg_counter = 0;
static unsigned int local_variables_reg_counter = 0;

class Reg {
public:
    string name;
    Reg();
};

class LocalVariablesReg {
public:
    string name;
    LocalVariablesReg() = default;
    string add();
};

extern LocalVariablesReg local_variables_reg;

#endif //HW5_REGISTER_H