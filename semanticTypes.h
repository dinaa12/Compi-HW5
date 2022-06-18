#ifndef HW3_SEMANTICTYPES_H
#define HW3_SEMANTICTYPES_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

typedef string SemTypeName;

class SemType {
public:
    virtual SemTypeName getTypeName() {};
    virtual string getTypeValue() {};
    virtual SemTypeName getRetTypeName() {};
    virtual vector<SemTypeName> getListTypeName() {};
    virtual vector<string> getListNames() {};
    virtual ~SemType() = default;
};

class STExp : public SemType {
public:
    SemTypeName type_name;
    string type_value;
    STExp(SemTypeName t_name, string t_val = "") : type_name(t_name), type_value(t_val) {};
    SemTypeName getTypeName() override { return type_name; }
    SemTypeName getTypeValue() override { return type_value; }
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
    STCall(SemTypeName t_name) : type_name(t_name) {};
    SemTypeName getTypeName() override { return type_name; }
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

#endif //HW3_SEMANTICTYPES_H
