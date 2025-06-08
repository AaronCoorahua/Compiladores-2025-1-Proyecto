#include "exp.h"
#include <iostream>

using namespace std;

/* ===================================================================== */
/*                    Constructores / destructores                       */
/* ===================================================================== */
IFExp::IFExp(Exp* c, Exp* l, Exp* r) : cond(c), left(l), right(r) {}

BinaryExp::BinaryExp(Exp* l, Exp* r, BinaryOp op)
        : left(l), right(r), op(op)
{
    type = (op == PLUS_OP || op == MINUS_OP ||
            op == MUL_OP  || op == DIV_OP) ? "int" : "bool";
}

NumberExp   ::NumberExp(int v)              : value(v) {}
BoolExp     ::BoolExp(bool v)               : value(v) {}
IdentifierExp::IdentifierExp(const string& n): name(n) {}

NumberExp::~NumberExp()  = default;
BoolExp::~BoolExp()      = default;
IdentifierExp::~IdentifierExp() = default;

/* liberar sub-árboles donde hace falta */
BinaryExp::~BinaryExp() { delete left; delete right; }
IFExp::~IFExp()         { delete cond; delete left; delete right; }

AssignStatement::AssignStatement(string id, Exp* e): id(std::move(id)), rhs(e) {}
AssignStatement::~AssignStatement() { delete rhs; }

PrintStatement::PrintStatement(Exp* e): e(e) {}
PrintStatement::~PrintStatement()    { delete e; }

IfStatement::IfStatement(Exp* c, Body* t, Body* e)
        : condition(c), then(t), els(e) {}
IfStatement::~IfStatement() { delete condition; delete then; delete els; }

WhileStatement::WhileStatement(Exp* c, Body* b): condition(c), b(b) {}
WhileStatement::~WhileStatement() { delete condition; delete b; }

/* ---------------------------- Var-Decs ------------------------------- */
VarDec::VarDec(string t, list<string> v)
        : type(std::move(t)), vars(std::move(v)) {}
VarDec::~VarDec() = default;

VarDecList::VarDecList() = default;
void VarDecList::add(VarDec* v){ vardecs.push_back(v); }
VarDecList::~VarDecList(){
    for (auto v: vardecs) delete v;
}

/* -------------------------- StatementList ---------------------------- */
StatementList::StatementList() = default;
void StatementList::add(Stm* s){ stms.push_back(s); }
StatementList::~StatementList(){
    for (auto s: stms) delete s;
}

/* ------------------------------- Body -------------------------------- */
Body::Body(VarDecList* v, StatementList* s): vardecs(v), slist(s) {}
Body::~Body(){ delete vardecs; delete slist; }

/* ===================================================================== */
/*                dispatches accept()   (definidos inline)               */
/* ===================================================================== */
/* (todos los accept() simples se definen en visitor.cpp) */

/* ===================================================================== */
/*              utilidad: conversión de enumeración a string             */
/* ===================================================================== */
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
