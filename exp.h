#ifndef EXP_H
#define EXP_H

#include <string>
#include <list>
#include <vector>


using namespace std;

class Visitor;      
class Body;         

enum BinaryOp { PLUS_OP, MINUS_OP, MUL_OP, DIV_OP,
                LT_OP,   LE_OP,    EQ_OP, GT_OP, GE_OP};


class Exp {
public:
    string type;
    virtual float  accept(Visitor*) = 0;
    virtual ~Exp() = default;
    static string binopToChar(BinaryOp);
};

class IFExp : public Exp {
public:
    Exp *cond, *left, *right;
    IFExp(Exp*, Exp*, Exp*);
    ~IFExp();
    float accept(Visitor*) override;
};

class BinaryExp : public Exp {
public:
    Exp *left, *right;
    BinaryOp op;
    BinaryExp(Exp*, Exp*, BinaryOp);
    ~BinaryExp();
    float accept(Visitor*) override;
};

class NumberExp : public Exp {
public:
    int value;
    explicit NumberExp(int v);
    ~NumberExp();
    float accept(Visitor*) override;
};

class FloatExp : public Exp {
public:
    float value;
    explicit FloatExp(float v);
    ~FloatExp();
    float accept(Visitor*) override;
};


class BoolExp : public Exp {
public:
    int value;               
    explicit BoolExp(bool v);
    ~BoolExp();
    float accept(Visitor*) override;
};

class IdentifierExp : public Exp {
public:
    string name;
    explicit IdentifierExp(const string&);
    ~IdentifierExp();
    float accept(Visitor*) override;
};

class FCallExp : public Exp {
public:
    string     nombre;
    list<Exp*> argumentos;
    FCallExp() = default;
    ~FCallExp() override = default;     
    float accept(Visitor*) override;
};

class Stm {
public:
    virtual int accept(Visitor*) = 0;
    virtual ~Stm() = default;
};

class AssignStatement : public Stm {
public:
    Exp* lhs;
    Exp* rhs;
    AssignStatement(Exp* l, Exp* r);
    ~AssignStatement();
    int accept(Visitor*) override;
};

class PrintStatement : public Stm {
public:
    Exp* e;
    explicit PrintStatement(Exp*);
    ~PrintStatement();
    int accept(Visitor*) override;
};

class IfStatement : public Stm {
public:
    Exp*  condition;
    Body* then;
    Body* els;
    IfStatement(Exp*, Body*, Body*);
    ~IfStatement();
    int accept(Visitor*) override;
};

class ForStatement : public Stm {
public:
    string id;
    Exp *start, *end;
    bool downto;
    Body *body;

    ForStatement(string id, Exp* s, Exp* e, bool d, Body* b);
    ~ForStatement() override;

    int accept(Visitor* v) override;
};


class WhileStatement : public Stm {
public:
    Exp*  condition;
    Body* b;
    WhileStatement(Exp*, Body*);
    ~WhileStatement();
    int accept(Visitor*) override;
};

class ReturnStatement : public Stm {
public:
    Exp* e;
    ReturnStatement():e(nullptr) {}
    ~ReturnStatement() override = default;
    int accept(Visitor*) override;
};


class RecordTAssignStatement : public Stm  {
public:
    string base;      
    string field;   
    Exp*   val;
    RecordTAssignStatement(const string& base, const string& field,Exp* val);
    ~RecordTAssignStatement() ;

    int accept(Visitor* v) override;
};

class RecordTIdentifierExp : public Exp {
public:
    string base;
    string field;

    RecordTIdentifierExp(const string& base, const string& field );
    ~RecordTIdentifierExp() override;
    float accept(Visitor*) override;
};

class RecordVarDec {
public:
    string atribute;
    string type;
    RecordVarDec(string, string);
    ~RecordVarDec();
    int accept(Visitor*);
};
class TypeDec {
public:
    string name;  
    vector<RecordVarDec*> atributs;
    TypeDec(string name, vector<RecordVarDec*> atributs);
    ~TypeDec();
    int accept(Visitor* v);
};
class TypeDecList {
public:
    list<TypeDec*> typedecs;
    TypeDecList(list<TypeDec*> typedecs);
    TypeDecList();
    void add(TypeDec* val);
    ~TypeDecList();
    int accept(Visitor* v);

};

class VarDec {
public:
    string type;
    list<string> vars;
    VarDec(string, list<string>);
    ~VarDec();
    int accept(Visitor*);
};



class VarDecList {
public:
    list<VarDec*> vardecs;
    VarDecList();
    void add(VarDec*);
    ~VarDecList();
    int accept(Visitor*);
};

class StatementList {
public:
    list<Stm*> stms;
    StatementList();
    void add(Stm*);
    ~StatementList();
    int accept(Visitor*);
};

class Body {
public:
    VarDecList* vardecs;
    StatementList* slist;
    Body(VarDecList*, StatementList*);
    ~Body();
    int accept(Visitor*);
};

 
class FunDec {
public:
    string nombre;                 
    string tipo;                   
    list<string> parametros;       
    list<string> tipos;           
    Body* cuerpo {nullptr};

    FunDec() = default;
    ~FunDec() = default;
    int accept(Visitor*);
};

class FunDecList {
public:
    list<FunDec*> Fundecs;
    void add(FunDec* f){ Fundecs.push_back(f); }
    ~FunDecList() = default;             
    int accept(Visitor*);
};

 
class Program {
public:
    TypeDecList*   typeDecList{nullptr};  
    VarDecList*    vardecs  {nullptr};    
    FunDecList*    fundecs  {nullptr};   
    Body*          mainBody {nullptr};   
};

#endif   
