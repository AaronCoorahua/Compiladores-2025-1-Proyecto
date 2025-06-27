#ifndef VISITOR_H
#define VISITOR_H

#include "exp.h"
#include "environment.h"
#include <unordered_map>
#include <list>

/* adelantos de clases AST */
class BinaryExp;  class NumberExp; class FloatExp; class BoolExp;
class IdentifierExp; class IFExp;   class FCallExp;
class AssignStatement; class PrintStatement;
class IfStatement; class WhileStatement; class ForStatement;
class VarDec; class VarDecList; class StatementList; class Body;
class FunDec; class FunDecList; class ReturnStatement; class Program;

class TypeDecList;
class TypeDec;
class RecordTIdentifierExp;
class RecordTAssignStatement;
class RecordVarDec;

/* ───────────────────── Interface base ──────────────────────────────── */
class Visitor {
public:
    /* expresiones */
    virtual float  visit(BinaryExp*)      = 0;
    virtual float  visit(NumberExp*)      = 0;
    virtual float  visit(BoolExp*)        = 0;
    virtual float  visit(IdentifierExp*)  = 0;
    virtual float  visit(IFExp*)          = 0;
    virtual float visit(FCallExp*)       = 0;
    virtual float visit(FloatExp*) = 0;
    virtual float  visit(RecordTIdentifierExp*) =0;
    /* sentencias */

    virtual void visit(AssignStatement*) = 0;
    virtual void visit(PrintStatement*)  = 0;
    virtual void visit(IfStatement*)     = 0;
    virtual void visit(ForStatement*)    = 0;
    virtual void visit(WhileStatement*)  = 0;
    virtual void visit(ReturnStatement*) = 0;
    virtual void visit(RecordTAssignStatement*)=0;

    /* bloques y listas */
    virtual void visit(VarDec*)          = 0;
    virtual void visit(VarDecList*)      = 0;
    virtual void visit(StatementList*)   = 0;
    virtual void visit(Body*)            = 0;
    virtual void visit(TypeDecList*)      = 0;
    virtual void visit(TypeDec*)          = 0;
    virtual void visit(RecordVarDec*) = 0;

    /* funciones y programa */
    virtual void visit(FunDec*)          = 0;
    virtual void visit(FunDecList*)      = 0;
    virtual void visit(Program*)         = 0;   /* ← importante */
};

/* ───────────────────── Pretty-printer ──────────────────────────────── */
class PrintVisitor : public Visitor {
public:
    int nivel_indentacion = 0;
    string getIndent() {
        return string(nivel_indentacion * 2, ' ');
    }
    void imprimir(Program*);

    /* expresiones */
    float  visit(BinaryExp*)      override;
    float  visit(NumberExp*)      override;
    float visit(FloatExp *) override;
    float  visit(BoolExp*)        override;
    float  visit(IdentifierExp*)  override;
    float  visit(IFExp*)          override;
    float  visit(FCallExp*)       override;
    float  visit(RecordTIdentifierExp*) override;

    /* sentencias + listas + cuerpos */
    void visit(AssignStatement*) override;
    void visit(PrintStatement*)  override;
    void visit(IfStatement*)     override;
    void visit(ForStatement*)  override;
    void visit(WhileStatement*)  override;
    void visit(ReturnStatement*) override;
    void visit(RecordTAssignStatement*) override;

    void visit(VarDec*)          override;
    void visit(VarDecList*)      override;
    void visit(StatementList*)   override;
    void visit(Body*)            override;
    void visit(TypeDecList*)      override;
    void visit(TypeDec*)          override;
    void visit(RecordVarDec*)          override;

    /* funciones y programa */
    void visit(FunDec*)          override;
    void visit(FunDecList*)      override;
    void visit(Program*)         override;   /* imprime main */
};

/* ───────────────────── Evaluador / intérprete ──────────────────────── */
class EVALVisitor : public Visitor {
    Environment env;
    std::unordered_map<std::string, FunDec*> fdecs;
    float  retval  = 0.0f;

    unordered_map<string, vector<RecordVarDec*>> type_registry;

    bool retcall = false;
    std::string currFun;
public:
    EVALVisitor() : currFun("") {}
    void ejecutar(Program*);
    void visit(Program*) override;

    /* expresiones */
    float  visit(BinaryExp*)      override;
    float  visit(NumberExp*)      override;
    float  visit(RecordTIdentifierExp*) override;
    float visit(FloatExp *) override;
    float  visit(BoolExp*)        override;
    float  visit(IdentifierExp*)  override;
    float  visit(IFExp*)          override;
    float  visit(FCallExp*)       override;

    /* sentencias + bloques */
    void visit(AssignStatement*) override;
    void visit(PrintStatement*)  override;
    void visit(IfStatement*)     override;
    void visit(ForStatement*)  override;
    void visit(WhileStatement*)  override;
    void visit(ReturnStatement*) override;
    void visit(RecordTAssignStatement*) override;

    void visit(VarDec*)          override;
    void visit(VarDecList*)      override;
    void visit(StatementList*)   override;
    void visit(Body*)            override;
    void visit(TypeDecList*)      override;
    void visit(TypeDec*)          override;
    void visit(RecordVarDec*)          override;
    /* funciones */
    void visit(FunDec*)          override;
    void visit(FunDecList*)      override;
};

#endif /* VISITOR_H */
