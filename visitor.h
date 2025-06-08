#ifndef VISITOR_H
#define VISITOR_H

#include "exp.h"
#include "environment.h"
#include <unordered_map>
#include <list>

/* adelantos de clases AST */
class BinaryExp;  class NumberExp;  class BoolExp;
class IdentifierExp; class IFExp;   class FCallExp;
class AssignStatement; class PrintStatement;
class IfStatement; class WhileStatement;
class VarDec; class VarDecList; class StatementList; class Body;
class FunDec; class FunDecList; class ReturnStatement; class Program;

/* ───────────────────── Interface base ──────────────────────────────── */
class Visitor {
public:
    /* expresiones */
    virtual int  visit(BinaryExp*)      = 0;
    virtual int  visit(NumberExp*)      = 0;
    virtual int  visit(BoolExp*)        = 0;
    virtual int  visit(IdentifierExp*)  = 0;
    virtual int  visit(IFExp*)          = 0;
    virtual int  visit(FCallExp*)       = 0;

    /* sentencias */
    virtual void visit(AssignStatement*) = 0;
    virtual void visit(PrintStatement*)  = 0;
    virtual void visit(IfStatement*)     = 0;
    virtual void visit(WhileStatement*)  = 0;
    virtual void visit(ReturnStatement*) = 0;

    /* bloques y listas */
    virtual void visit(VarDec*)          = 0;
    virtual void visit(VarDecList*)      = 0;
    virtual void visit(StatementList*)   = 0;
    virtual void visit(Body*)            = 0;

    /* funciones y programa */
    virtual void visit(FunDec*)          = 0;
    virtual void visit(FunDecList*)      = 0;
    virtual void visit(Program*)         = 0;   /* ← importante */
};

/* ───────────────────── Pretty-printer ──────────────────────────────── */
class PrintVisitor : public Visitor {
public:
    void imprimir(Program*);

    /* expresiones */
    int  visit(BinaryExp*)      override;
    int  visit(NumberExp*)      override;
    int  visit(BoolExp*)        override;
    int  visit(IdentifierExp*)  override;
    int  visit(IFExp*)          override;
    int  visit(FCallExp*)       override;

    /* sentencias + listas + cuerpos */
    void visit(AssignStatement*) override;
    void visit(PrintStatement*)  override;
    void visit(IfStatement*)     override;
    void visit(WhileStatement*)  override;
    void visit(ReturnStatement*) override;
    void visit(VarDec*)          override;
    void visit(VarDecList*)      override;
    void visit(StatementList*)   override;
    void visit(Body*)            override;

    /* funciones y programa */
    void visit(FunDec*)          override;
    void visit(FunDecList*)      override;
    void visit(Program*)         override;   /* imprime main */
};

/* ───────────────────── Evaluador / intérprete ──────────────────────── */
class EVALVisitor : public Visitor {
    Environment env;
    std::unordered_map<std::string, FunDec*> fdecs;
    int  retval  = 0;
    bool retcall = false;
    std::string currFun;
public:
    EVALVisitor() : currFun("") {}
    void ejecutar(Program*);
    void visit(Program*) override;

    /* expresiones */
    int  visit(BinaryExp*)      override;
    int  visit(NumberExp*)      override;
    int  visit(BoolExp*)        override;
    int  visit(IdentifierExp*)  override;
    int  visit(IFExp*)          override;
    int  visit(FCallExp*)       override;

    /* sentencias + bloques */
    void visit(AssignStatement*) override;
    void visit(PrintStatement*)  override;
    void visit(IfStatement*)     override;
    void visit(WhileStatement*)  override;
    void visit(ReturnStatement*) override;
    void visit(VarDec*)          override;
    void visit(VarDecList*)      override;
    void visit(StatementList*)   override;
    void visit(Body*)            override;

    /* funciones */
    void visit(FunDec*)          override;
    void visit(FunDecList*)      override;
};

#endif /* VISITOR_H */
