#ifndef HW3_SEMANTICTYPES_H
#define HW3_SEMANTICTYPES_H

#include <iostream>
#include <string>
#include <vector>
#include "register.h"
#include "bp.hpp"
#include "symbolTable.h"

using namespace std;

typedef string SemTypeName;

extern CodeBuffer& code_buff;

// TODO
enum DERIVATION_RULE {
    NONE,
    EXP_TO_LPAREN_EXP_RPAREN,
    EXP_TO_EXP_BINOP_ADD_EXP,
    EXP_TO_EXP_BINOP_SUB_EXP,
    EXP_TO_EXP_BINOP_MUL_EXP,
    EXP_TO_EXP_BINOP_DIV_EXP,
    EXP_TO_ID,
    EXP_TO_CALL,
    EXP_TO_NUM,
    EXP_TO_NUM_B,
    EXP_TO_STRING,
    EXP_TO_TRUE,
    EXP_TO_FALSE,
    EXP_TO_NOT_EXP,
    EXP_TO_EXP_AND_EXP,
    EXP_TO_EXP_OR_EXP,
    EXP_TO_EXP_RELOP_COMPARE_EXP,
    EXP_TO_EXP_RELOP_EQUAL_EXP,
    EXP_TO_LPAREN_TYPE_RPAREN_EXP
};

class SemType {
public:
    virtual SemTypeName getTypeName() {};
    virtual string getTypeValue() {};
    virtual SemTypeName getRetTypeName() {};
    virtual vector<SemTypeName> getListTypeName() {};
    virtual vector<string> getListNames() {};
    virtual Reg* getReg() {}; // TODO: implement in all classes
    virtual vector<pair<int, BranchLabelIndex>> getTrueList() {};
    virtual vector<pair<int, BranchLabelIndex>> getFalseList() {};
    virtual vector<pair<int, BranchLabelIndex>> getNextList() {};
    virtual ~SemType() = default;
};

class STExp : public SemType {
public:
    SemTypeName type_name;
    string type_value;
    Reg reg;
    vector<pair<int, BranchLabelIndex>> true_list;
    vector<pair<int, BranchLabelIndex>> false_list;
    vector<pair<int, BranchLabelIndex>> next_list;
    STExp(SemTypeName t_name, string t_val = "", DERIVATION_RULE d_rule = NONE, SemType* s1 = nullptr, SemType* s2 = nullptr, SemType* s3 = nullptr);
    //STExp(SemTypeName t_name, string t_val = "") : type_name(t_name), type_value(t_val) {};
    SemTypeName getTypeName() override { return type_name; }
    SemTypeName getTypeValue() override { return type_value; }
    Reg* getReg() override { return &reg; }
    vector<pair<int, BranchLabelIndex>> getTrueList() override { return true_list; }
    vector<pair<int, BranchLabelIndex>> getFalseList() override { return false_list; }
    vector<pair<int, BranchLabelIndex>> getNextList() override { return next_list; }
};

class STExpFunc : public SemType {
public:
    SemTypeName type_name;
    SemTypeName ret_type_name;
    STExpFunc(SemTypeName t_name, SemTypeName ret_t_name) : type_name(t_name), ret_type_name(ret_t_name) {};
    SemTypeName getTypeName() override { return type_name; }
    SemTypeName getRetTypeName() override { return ret_type_name; }
};

class STType : public SemType {
public:
    SemTypeName type_name;
    STType(SemTypeName t_name) : type_name(t_name) {};
    SemTypeName getTypeName() override { return type_name; }
};

class STExpList : public SemType {
public:
    SemTypeName type_name;
    vector<SemTypeName> list_type_name;
    STExpList(SemTypeName t_name)
        { list_type_name.push_back(t_name); };
    STExpList(SemTypeName t_name, vector<SemTypeName> l_t_name)
        { list_type_name.push_back(t_name); list_type_name.insert(list_type_name.end(), l_t_name.begin(), l_t_name.end()); };
    SemTypeName getTypeName() override { return type_name; }
    vector<SemTypeName> getListTypeName() override { return list_type_name; }
};

class STCall : public SemType {
public:
    SemTypeName type_name; // return type name
    Reg reg;
    STCall(SemTypeName t_name) : type_name(t_name), reg(Reg()) {};
    SemTypeName getTypeName() override { return type_name; }
    Reg* getReg() override { return &reg; }
};

class STRetType : public SemType {
public:
    SemTypeName type_name;
    STRetType(SemTypeName t_name) : type_name(t_name) {};
    SemTypeName getTypeName() override { return type_name; }
};

class STFormalsList : public SemType {
public:
    SemTypeName type_name;
    vector<SemTypeName> list_type_name;
    vector<string> list_names;
    STFormalsList() = default;
    STFormalsList(SemTypeName t_name, string t_val)
        { list_type_name.push_back(t_name); list_names.push_back(t_val); };
    STFormalsList(SemTypeName t_name, vector<SemTypeName> l_t_name, string t_val, vector<string> l_t_val)
        { list_type_name.push_back(t_name); list_type_name.insert(list_type_name.end(), l_t_name.begin(), l_t_name.end());
            list_names.push_back(t_val); list_names.insert(list_names.end(), l_t_val.begin(), l_t_val.end()); };
    SemTypeName getTypeName() override { return type_name; }
    vector<SemTypeName> getListTypeName() override { return list_type_name; }
    vector<string> getListNames() override { return list_names; }
};

class STFormalDecl : public SemType {
public:
    SemTypeName type_name;
    string type_value;
    STFormalDecl(SemTypeName t_name, string t_val) : type_name(t_name), type_value(t_val) {};
    SemTypeName getTypeName() override { return type_name; }
    SemTypeName getTypeValue() override { return type_value; }
};

class STRelopEqual : public SemType {
public:
    SemTypeName type_name;
    string type_value;
    STRelopEqual(SemTypeName t_name, string t_val) : type_name(t_name), type_value(t_val) {};
    SemTypeName getTypeName() override { return type_name; }
    SemTypeName getTypeValue() override { return type_value; }
};

class STRelopCompare : public SemType {
public:
    SemTypeName type_name;
    string type_value;
    STRelopCompare(SemTypeName t_name, string t_val) : type_name(t_name), type_value(t_val) {};
    SemTypeName getTypeName() override { return type_name; }
    SemTypeName getTypeValue() override { return type_value; }
};

class STBinopAdd : public SemType {
public:
    SemTypeName type_name;
    string type_value;
    STBinopAdd(SemTypeName t_name, string t_val) : type_name(t_name), type_value(t_val) {};
    SemTypeName getTypeName() override { return type_name; }
    SemTypeName getTypeValue() override { return type_value; }
};

class STBinopSub : public SemType {
public:
    SemTypeName type_name;
    string type_value;
    STBinopSub(SemTypeName t_name, string t_val) : type_name(t_name), type_value(t_val) {};
    SemTypeName getTypeName() override { return type_name; }
    SemTypeName getTypeValue() override { return type_value; }
};

class STBinopMUL : public SemType {
public:
    SemTypeName type_name;
    string type_value;
    STBinopMUL(SemTypeName t_name, string t_val) : type_name(t_name), type_value(t_val) {};
    SemTypeName getTypeName() override { return type_name; }
    SemTypeName getTypeValue() override { return type_value; }
};

class STBinopDiv : public SemType {
public:
    SemTypeName type_name;
    string type_value;
    STBinopDiv(SemTypeName t_name, string t_val) : type_name(t_name), type_value(t_val) {};
    SemTypeName getTypeName() override { return type_name; }
    SemTypeName getTypeValue() override { return type_value; }
};


class STID : public SemType {
public:
    SemTypeName type_name;
    string type_value;
    STID(SemTypeName t_name, string t_val) : type_name(t_name), type_value(t_val) {};
    SemTypeName getTypeName() override { return type_name; }
    SemTypeName getTypeValue() override { return type_value; }
};

class STNum : public SemType {
public:
    SemTypeName type_name;
    string type_value;
    STNum(SemTypeName t_name, string t_val) : type_name(t_name), type_value(t_val) {};
    SemTypeName getTypeName() override { return type_name; }
    SemTypeName getTypeValue() override { return type_value; }
};

class STString : public SemType {
public:
    SemTypeName type_name;
    string type_value;
    STString(SemTypeName t_name, string t_val) : type_name(t_name), type_value(t_val) {};
    SemTypeName getTypeName() override { return type_name; }
    SemTypeName getTypeValue() override { return type_value; }
};

class STM : public SemType {
public:
    SemTypeName type_name;
    STM();
    SemTypeName getTypeName() override { return type_name; }
};


class STN : public SemType {
public:
    SemTypeName type_name;
    STN();
    SemTypeName getTypeName() override { return type_name; }
};

#endif //HW3_SEMANTICTYPES_H
