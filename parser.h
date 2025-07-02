#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include "exp.h"
#include <list>   

class Parser {

private:


    Scanner* scanner;
    Token*   current  {nullptr};
    Token*   previous {nullptr};

    bool match  (Token::Type);
    bool check  (Token::Type);
    bool advance();
    bool isAtEnd();

    VarDec* parseVarDec();
    VarDecList* parseVarDecList();

    TypeDec* parseTypeDec();
    TypeDecList* parseTypeDecList();


    FunDec* parseFunDec();
    FunDecList* parseFunDecList();

    void parseParamList(FunDec* f); 
    void parseParam(FunDec* f);    

    Body* parseBody();
    Body* parseBlockOrStmt();

    StatementList* parseStatementList();
    Stm* parseStatement();

    Exp* parseCExp();
    Exp* parseExpression();
    Exp* parseTerm();
    Exp* parseFactor();

    vector<RecordVarDec*> parseFieldList();

    std::vector<RecordVarDec*> parseRecordType();

public:
    explicit Parser(Scanner*);
    Program* parseProgram();
};

#endif 
