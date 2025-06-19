#ifndef EXP_H
#define EXP_H

#include <string>
#include <list>
#include <vector>

using namespace std;

/* ======================== Adelantos de clases ========================= */
class Visitor;      // interfaz de visitantes
class Body;         // adelante porque se usa en If/While antes de definirse

/* ======================== Operadores binarios ======================== */
enum BinaryOp { PLUS_OP, MINUS_OP, MUL_OP, DIV_OP,
                LT_OP,   LE_OP,    EQ_OP, GT_OP, GE_OP};

/* ===================================================================== */
/*                           EXPRESIONES                                 */
/* ===================================================================== */
class Exp {
public:
    virtual int  accept(Visitor*) = 0;
    virtual ~Exp() = default;
    static string binopToChar(BinaryOp);
};

/* --------- ifexp(cond, a, b)  (compatibilidad) ---------------------- */
class IFExp : public Exp {
public:
    Exp *cond, *left, *right;
    IFExp(Exp*, Exp*, Exp*);
    ~IFExp();
    int accept(Visitor*) override;
};

/* --------- a + b, a < b, … ----------------------------------------- */
class BinaryExp : public Exp {
public:
    Exp *left, *right;
    string type;
    BinaryOp op;
    BinaryExp(Exp*, Exp*, BinaryOp);
    ~BinaryExp();
    int accept(Visitor*) override;
};

/* --------- literales ------------------------------------------------- */
class NumberExp : public Exp {
public:
    int value;
    explicit NumberExp(int v);
    ~NumberExp();
    int accept(Visitor*) override;
};

class BoolExp : public Exp {
public:
    int value;                /* 1 = true / 0 = false */
    explicit BoolExp(bool v);
    ~BoolExp();
    int accept(Visitor*) override;
};

class IdentifierExp : public Exp {
public:
    string name;
    explicit IdentifierExp(const string&);
    ~IdentifierExp();
    int accept(Visitor*) override;
};

/* --------- llamada a función ---------------------------------------- */
class FCallExp : public Exp {
public:
    string     nombre;
    list<Exp*> argumentos;
    FCallExp() = default;
    ~FCallExp() override = default;     /* nada que destruir: se libera visitante */
    int accept(Visitor*) override;
};

/* ===================================================================== */
/*                            SENTENCIAS                                 */
/* ===================================================================== */
class Stm {
public:
    virtual int accept(Visitor*) = 0;
    virtual ~Stm() = default;
};

class AssignStatement : public Stm {
public:
    string id;
    Exp* rhs;
    AssignStatement(string, Exp*);
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

/* ===================================================================== */
/*                    DECLARACIONES, LISTAS, BLOQUES                     */
/* ===================================================================== */
class RecordTAssignStatement : public Stm  {
public:
    string base;       // Ej: d en d.x
    string field;    // Ej: x en d.x

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
    int accept(Visitor*) override;
};

class RecordVarDec {
public:
    string  atribute;
    string      type;
    RecordVarDec(string, string);
    ~RecordVarDec();
    int accept(Visitor*);
};
class TypeDec {
public:
    string name; //name class
    vector<RecordVarDec*> atributs;
    TypeDec(string name, vector<RecordVarDec*> atributs);
    ~TypeDec();
    int accept(Visitor*);
};
class TypeDecList {
public:
    list<TypeDec*> typedecs;
    TypeDecList(list<TypeDec*> typedecs);
    TypeDecList();
    void add(TypeDec* val);
    ~TypeDecList();
    int accept(Visitor*) ;
};

class VarDec {
public:
    string      type;
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
    VarDecList*    vardecs;
    StatementList* slist;
    Body(VarDecList*, StatementList*);
    ~Body();
    int accept(Visitor*);
};

/* ===================================================================== */
/*                           FUNCIONES                                   */
/* ===================================================================== */
class FunDec {
public:
    string nombre;                 /* id de la función        */
    string tipo;                   /* tipo de retorno         */
    list<string> parametros;       /* ids de parámetros       */
    list<string> tipos;            /* tipos de parámetros     */
    Body* cuerpo {nullptr};

    FunDec() = default;
    ~FunDec() = default;
    int accept(Visitor*);
};

class FunDecList {
public:
    list<FunDec*> Fundecs;
    void add(FunDec* f){ Fundecs.push_back(f); }
    ~FunDecList() = default;            /* se destruye en visitor  */
    int accept(Visitor*);
};

/* ===================================================================== */
/*                              PROGRAMA                                 */
/* ===================================================================== */
class Program {
public:
    TypeDecList*   typeDecList{nullptr}; /* declaraciones de Struct */
    VarDecList*    vardecs  {nullptr};   /* variables globales */
    FunDecList*    fundecs  {nullptr};   /* funciones          */
    Body*          mainBody {nullptr};   /* bloque begin … end */
};

#endif  /* EXP_H */
