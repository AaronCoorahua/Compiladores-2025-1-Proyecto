#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include "exp.h"
#include <list>     /* necesitado por los métodos privados */

class Parser {

private:


    /*——————  estado ——————*/
    Scanner* scanner;
    Token*   current  {nullptr};
    Token*   previous {nullptr};

    /*——————  utilidades ——————*/
    bool match  (Token::Type);
    bool check  (Token::Type);
    bool advance();
    bool isAtEnd();

    /*——————  sub-parsers ——————*/
    VarDec*        parseVarDec();
    VarDecList*    parseVarDecList();

    TypeDec*       parseTypeDec();
    TypeDecList*   parseTypeDecList();


    FunDec*        parseFunDec();
    FunDecList*    parseFunDecList();

    Body*          parseBody();
    Body*          parseBlockOrStmt();

    StatementList* parseStatementList();
    Stm*           parseStatement();

    Exp*           parseCExp();
    Exp*           parseExpression();
    Exp*           parseTerm();
    Exp*           parseFactor();

    vector<RecordVarDec*>  parseFieldList();
public:
    explicit Parser(Scanner*);
    Program* parseProgram();
};

#endif  /* PARSER_H */
