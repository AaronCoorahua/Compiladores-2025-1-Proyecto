#include "exp.h"
#include <iostream>

using namespace std;

IFExp::IFExp(Exp* c, Exp* l, Exp* r) : cond(c), left(l), right(r) {}


BinaryExp::BinaryExp(Exp* l, Exp* r, BinaryOp op)
        : left(l), right(r), op(op)
{
    type = (op == PLUS_OP || op == MINUS_OP ||
            op == MUL_OP  || op == DIV_OP) ? "int" : "bool";
}

NumberExp   ::NumberExp(int v)              : value(v) {}
FloatExp    ::FloatExp(float v) : value(v) {}
BoolExp     ::BoolExp(bool v)               : value(v) {}
IdentifierExp::IdentifierExp(const string& n): name(n) {}

NumberExp::~NumberExp()  = default;
FloatExp::~FloatExp()  = default;
BoolExp::~BoolExp()      = default;
IdentifierExp::~IdentifierExp() = default;

BinaryExp::~BinaryExp() { delete left; delete right; }
IFExp::~IFExp()         { delete cond; delete left; delete right; }

AssignStatement::AssignStatement(Exp* l, Exp* r): lhs(l), rhs(r) {}
AssignStatement::~AssignStatement() { delete rhs; delete lhs; }

PrintStatement::PrintStatement(Exp* e): e(e) {}
PrintStatement::~PrintStatement()    { delete e; }

IfStatement::IfStatement(Exp* c, Body* t, Body* e)
        : condition(c), then(t), els(e) {}
IfStatement::~IfStatement() { delete condition; delete then; delete els; }

WhileStatement::WhileStatement(Exp* c, Body* b): condition(c), b(b) {}
WhileStatement::~WhileStatement() { delete condition; delete b; }

ForStatement::ForStatement(string id, Exp* s, Exp* e, bool d, Body* b)
    : id(move(id)), start(s), end(e), downto(d), body(b) {}
ForStatement::~ForStatement() {
    delete start;
    delete end;
    delete body;
}

VarDec::VarDec(string t, list<string> v)
        : type(move(t)), vars(move(v)) {}
VarDec::~VarDec() = default;

VarDecList::VarDecList() = default;
void VarDecList::add(VarDec* v){ vardecs.push_back(v); }
VarDecList::~VarDecList(){
    for (auto v: vardecs) delete v;
}

StatementList::StatementList() = default;
void StatementList::add(Stm* s){ stms.push_back(s); }
StatementList::~StatementList(){
    for (auto s: stms) delete s;
}

Body::Body(VarDecList* v, StatementList* s): vardecs(v), slist(s) {}
Body::~Body(){ delete vardecs; delete slist; }

string Exp::binopToChar(BinaryOp op)
{
    switch(op){
        case PLUS_OP : return "+";
        case MINUS_OP: return "-";
        case MUL_OP  : return "*";
        case DIV_OP  : return "/";
        case LT_OP   : return "<";
        case LE_OP   : return "<=";
        case EQ_OP   : return "==";
        case GT_OP   : return ">";
        case GE_OP   : return ">=";
        default      : return "?";
    }
}

TypeDec::TypeDec(string name, vector<RecordVarDec*> atributs) {
    this->name = name;
    this->atributs = atributs;
}

TypeDec::~TypeDec() {
    for (auto a : atributs) {
        delete a;
    }
}


TypeDecList::TypeDecList(list<TypeDec*> typedecs) {
    this->typedecs=typedecs;
}

void TypeDecList::add(TypeDec* val) {
    this->typedecs.push_back(val);
}

TypeDecList::~TypeDecList() {
    for (auto td : typedecs) {
        delete td;
    }
}

TypeDecList::TypeDecList() {
    typedecs = list<TypeDec*>();
}

RecordTAssignStatement::RecordTAssignStatement(const string& base, const string &field, Exp* val) {
    this->base = base;
    this->field = field;
    this->val=val;
}

RecordTAssignStatement::~RecordTAssignStatement() {
    delete this->val;
}

RecordTIdentifierExp::RecordTIdentifierExp(const string& base,const string& field)
{
    this->base = base;
    this->field = field;
}



RecordTIdentifierExp::~RecordTIdentifierExp() {}

RecordVarDec::RecordVarDec(string atribute, string type)  {
    this->atribute = atribute;
    this->type = type;
}

RecordVarDec::~RecordVarDec() {

}



