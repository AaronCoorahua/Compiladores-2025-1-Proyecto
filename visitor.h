#ifndef VISITOR_H
#define VISITOR_H

#include "exp.h"
#include "environment.h"
#include <unordered_map>
#include <ostream>
#include <string>
#include <map>
class BinaryExp;  class NumberExp; class FloatExp; class BoolExp;
class IdentifierExp; class IFExp;   class FCallExp; class MethodCallExp;
class AssignStatement; class PrintStatement;
class IfStatement; class WhileStatement; class ForStatement;
class VarDec; class VarDecList; class StatementList; class Body;
class FunDec; class FunDecList; class ReturnStatement; class Program;

class TypeDecList;
class TypeDec;
class RecordTIdentifierExp;
class RecordTAssignStatement;
class RecordVarDec;

class Visitor {
public:
    virtual float  visit(BinaryExp*) = 0;
    virtual float  visit(NumberExp*) = 0;
    virtual float  visit(BoolExp*) = 0;
    virtual float  visit(IdentifierExp*) = 0;
    virtual float  visit(IFExp*) = 0;
    virtual float visit(FCallExp*) = 0;
    virtual float visit(FloatExp*) = 0;
    virtual float  visit(RecordTIdentifierExp*) =0;
    virtual float visit(MethodCallExp* e) = 0;

    virtual void visit(AssignStatement*) = 0;
    virtual void visit(PrintStatement*) = 0;
    virtual void visit(IfStatement*) = 0;
    virtual void visit(ForStatement*) = 0;
    virtual void visit(WhileStatement*) = 0;
    virtual void visit(ReturnStatement*) = 0;
    virtual void visit(RecordTAssignStatement*)=0;

    virtual void visit(VarDec*) = 0;
    virtual void visit(VarDecList*) = 0;
    virtual void visit(StatementList*) = 0;
    virtual void visit(Body*) = 0;
    virtual void visit(TypeDecList*) = 0;
    virtual void visit(TypeDec*) = 0;
    virtual void visit(RecordVarDec*) = 0;

    virtual void visit(FunDec*) = 0;
    virtual void visit(FunDecList*) = 0;
    virtual void visit(Program*) = 0;   
};

 class PrintVisitor : public Visitor {
public:
    int nivel_indentacion = 0;
    string getIndent() {
        return string(nivel_indentacion * 2, ' ');
    }
    void imprimir(Program*);

    float  visit(BinaryExp*) override;
    float  visit(NumberExp*) override;
    float visit(FloatExp *) override;
    float  visit(BoolExp*) override;
    float  visit(IdentifierExp*)  override;
    float  visit(IFExp*) override;
    float  visit(FCallExp*) override;
    float  visit(RecordTIdentifierExp*) override;
    float  visit(MethodCallExp*) override;


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

    void visit(FunDec*)          override;
    void visit(FunDecList*)      override;
    void visit(Program*)         override;   
};

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

 
    float  visit(BinaryExp*) override;
    float  visit(NumberExp*) override;
    float  visit(RecordTIdentifierExp*) override;
    float  visit(MethodCallExp*) override;
    float visit(FloatExp *) override;
    float  visit(BoolExp*) override;
    float  visit(IdentifierExp*) override;
    float  visit(IFExp*) override;
    float  visit(FCallExp*) override;
 
    void visit(AssignStatement*) override;
    void visit(PrintStatement*) override;
    void visit(IfStatement*) override;
    void visit(ForStatement*) override;
    void visit(WhileStatement*) override;
    void visit(ReturnStatement*) override;

    void visit(RecordTAssignStatement*) override;

    void visit(VarDec*) override;
    void visit(VarDecList*) override;
    void visit(StatementList*) override;
    void visit(Body*) override;
    void visit(TypeDecList*) override;
    void visit(TypeDec*) override;
    void visit(RecordVarDec*) override;
 
    void visit(FunDec*) override;
    void visit(FunDecList*) override;
};

class TYPEVisitor : public Visitor {
    Environment env;
    std::unordered_map<std::string, FunDec *> fdecs;
    std::unordered_map<std::string, vector<RecordVarDec *>> type_registry;

    std::unordered_map<std::string, std::string> current_fields;
    string currFun;
public:
    TYPEVisitor() : currFun("") {}

    void visit(Program *) override;
    float visit(BinaryExp *) override;
    float visit(NumberExp *) override;
    float visit(FloatExp *) override;
    float visit(BoolExp *) override;
    float visit(IdentifierExp *) override;
    float visit(IFExp *) override;
    float visit(FCallExp *) override;
    float visit(RecordTIdentifierExp *) override;
    float visit(MethodCallExp *) override;


    void visit(AssignStatement *) override;
    void visit(PrintStatement *) override;
    void visit(IfStatement *) override;
    void visit(ForStatement *) override;
    void visit(WhileStatement *) override;
    void visit(ReturnStatement *) override;
    void visit(RecordTAssignStatement *) override;
    void visit(VarDec *) override;
    void visit(VarDecList *) override;
    void visit(StatementList *) override;
    void visit(Body *) override;
    void visit(TypeDecList *) override;
    void visit(TypeDec *) override;
    void visit(RecordVarDec *) override;
    void visit(FunDec *) override;
    void visit(FunDecList *) override;
};

#include <ostream>
#include <string>
#include <map>
class CodeGenVisitor : public Visitor {
    std::ostream& out;
    std::map<std::string, double> floatConsts; 
    std::map<std::string, bool> isFloatVar;
    std::map<std::string,std::map<std::string,std::string>> recordFieldTypes;
    std::map<std::string,std::map<std::string,int>> recordLayouts;
    std::map<std::string,std::string> varTypes;
    int floatLabelCount;

    std::string currFun;

public:
    explicit CodeGenVisitor(std::ostream& output);
    void generate(Program* p);


    void registrarVariables(Program* p);



    void visit(TypeDecList*) override;
    void visit(TypeDec*) override;
    void visit(RecordVarDec*) override;
    float visit(RecordTIdentifierExp*) override;
    void  visit(RecordTAssignStatement*) override;


    void  visit(FunDec*) override;
    void  visit(FunDecList*) override;
    float visit(FCallExp*) override;
    float visit(MethodCallExp *) override;


    float visit(BinaryExp*) override;
    float visit(NumberExp*) override;
    float visit(FloatExp*) override;
    float visit(BoolExp*) override { return 0; }
    float visit(IdentifierExp*) override;
    float visit(IFExp*) override { return 0; }



    void  visit(AssignStatement*) override;
    void  visit(PrintStatement*) override;
    void  visit(IfStatement*) override;
    void  visit(ForStatement*) override;

    void  visit(WhileStatement*) override;

    void  visit(ReturnStatement*) override;


    void  visit(VarDec*) override;
    void  visit(VarDecList*) override;
    void  visit(StatementList*) override;
    void  visit(Body*) override;



    void  visit(Program*) override;
};


class ConstCollector : public Visitor {
    std::map<std::string, double>& floatConsts;
    int& floatLabelCount;
public:
    ConstCollector(std::map<std::string, double>& fc,int& cnt);

    float visit(FloatExp*) override;
    float visit(BinaryExp* e) override;
    void visit(AssignStatement* s) override;

    void visit(PrintStatement* s) override;

    void visit(StatementList* s) override;
    void visit(Body* b) override;

    void visit(Program* p) override;

    void  visit(RecordTAssignStatement* s) override;

    float visit(NumberExp*) override { return 0; }
    float visit(IdentifierExp*) override { return 0; }
    float visit(BoolExp*) override { return 0; }
    float visit(IFExp*) override { return 0; }
    float visit(FCallExp*) override;
    float visit(RecordTIdentifierExp*) override { return 0; }
    float visit(MethodCallExp*) override { return 0; }

    void  visit(IfStatement*) override {}
    void  visit(ForStatement*) override {}
    void  visit(WhileStatement*) override {}
    void  visit(ReturnStatement*) override {}

    void  visit(VarDec*) override {}
    void  visit(VarDecList*) override {}
    void  visit(TypeDecList*) override {}
    void  visit(TypeDec*) override {}
    void  visit(RecordVarDec*) override {}
    void  visit(FunDec*) override {}
    void  visit(FunDecList*) override {}
};

#endif 
