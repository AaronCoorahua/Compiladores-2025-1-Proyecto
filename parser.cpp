#include <iostream>
#include <stdexcept>
#include "parser.h"

using namespace std;

/*──────────────────── utilidades ────────────────────*/
bool Parser::match(Token::Type t){
    if (check(t)){ advance(); return true; }
    return false;
}
bool Parser::check(Token::Type t){ return !isAtEnd() && current->type==t; }
bool Parser::isAtEnd()           { return current->type==Token::END; }

bool Parser::advance(){
    if (isAtEnd()) return false;
    Token* tmp = current;
    current    = scanner->nextToken();
    if (previous) delete previous;
    previous = tmp;

    if (check(Token::ERR)){
        cerr << "Scanner error: " << current->text << endl; exit(1);
    }
    return true;
}

/*────────────── constructor ──────────────*/
Parser::Parser(Scanner* sc):scanner(sc){
    current = scanner->nextToken();
    if (current->type == Token::ERR){
        cerr << "Primer token inválido: " << current->text << endl;
        exit(1);
    }
}

/*──────────────── vars ────────────────*/
VarDec* Parser::parseVarDec(){
    if (!match(Token::VAR)) return nullptr;

    list<string> ids;
    match(Token::ID);                 // ❶ primer id
    ids.push_back(previous->text);
    while (match(Token::COMA)){
        match(Token::ID); ids.push_back(previous->text);
    }

    match(Token::COLON);              // ❷ ‘:’
    match(Token::ID);                 // ❸ tipo
    string type = previous->text;
    match(Token::PC);                 // ‘;’

    return new VarDec(type, ids);
}
VarDecList* Parser::parseVarDecList(){
    auto* v = new VarDecList();
    while (VarDec* d = parseVarDec()) v->add(d);
    return v;
}

/*──────────────── cuerpos ───────────────*/
StatementList* Parser::parseStatementList(){
    auto* sl = new StatementList();
    sl->add(parseStatement());
    while (match(Token::PC)){                       // ‘;’
        if (check(Token::END_KW) || check(Token::ELSE)) break;
        sl->add(parseStatement());
    }
    return sl;
}
Body* Parser::parseBody(){
    match(Token::BEGIN_KW);
    auto* v = parseVarDecList();
    auto* s = parseStatementList();
    match(Token::END_KW);
    return new Body(v,s);
}

/*──────────────── programa ──────────────*/
Program* Parser::parseProgram(){
    auto* p = new Program();

    if (match(Token::PROGRAM)){     // encabezado opcional
        match(Token::ID);  match(Token::PC);
    }

    p->vardecs  = parseVarDecList();
    p->fundecs  = parseFunDecList();

    match(Token::BEGIN_KW);
    p->mainBody = parseStatementList();
    match(Token::END_KW);
    match(Token::DOT);              // punto final

    if (!isAtEnd()){
        cerr << "Texto extra después del '.' final\n"; exit(1);
    }
    return p;
}

/*──────────────── funciones ─────────────*/
FunDecList* Parser::parseFunDecList(){
    auto* l = new FunDecList();
    while (true){
        while (match(Token::PC));                   // ‘;’ sueltos
        FunDec* f = parseFunDec();
        if (!f) break;
        l->add(f);
    }
    return l;
}
FunDec* Parser::parseFunDec(){
    if (!match(Token::FUNCTION)) return nullptr;

    auto* f = new FunDec();

    match(Token::ID);  f->nombre = previous->text;  // nombre

    /* parámetros */
    match(Token::PI);
    if (!check(Token::PD)){
        do{
            list<string> ids;
            match(Token::ID); ids.push_back(previous->text);
            while (match(Token::COMA)){ match(Token::ID); ids.push_back(previous->text); }

            match(Token::COLON);
            match(Token::ID); string ptype = previous->text;

            for (auto& id: ids){ f->parametros.push_back(id); f->tipos.push_back(ptype); }
        } while (match(Token::PC));                 // ‘;’ entre grupos
    }
    match(Token::PD);                               // )

    /* tipo de retorno */
    match(Token::COLON);
    match(Token::ID);  f->tipo = previous->text;
    match(Token::PC);                               // ;

    /* cuerpo */
    f->cuerpo = parseBody();
    match(Token::PC);                               // ; que cierra la función

    return f;
}

/*──────────────── sentencias ─────────────*/
Stm* Parser::parseStatement(){

    /* id := expresión */
    if (match(Token::ID)){
        string id = previous->text;
        match(Token::ASSIGN);
        Exp* rhs = parseCExp();
        return new AssignStatement(id,rhs);
    }

    /* writeln( exp ) */
    if (match(Token::WRITELN)){
        match(Token::PI);
        Exp* e = parseCExp();
        match(Token::PD);
        return new PrintStatement(e);
    }

    /* return( exp ) */
    if (match(Token::RETURN)){
        auto* rs = new ReturnStatement();
        match(Token::PI);
        rs->e = check(Token::PD)? nullptr : parseCExp();
        match(Token::PD);
        return rs;
    }

    /* if … then … [else …]        (¡ya NO se pide ‘end’ extra!) */
    if (match(Token::IF)){
        Exp*  cond  = parseCExp();  match(Token::THEN);
        Body* thenB = parseBlockOrStmt();
        Body* elseB = nullptr;
        if (match(Token::ELSE)) elseB = parseBlockOrStmt();
        return new IfStatement(cond, thenB, elseB);
    }

    /* while … do … end */
    if (match(Token::WHILE)){
        Exp* cond = parseCExp(); match(Token::DO);
        Body* body = parseBlockOrStmt();
        return new WhileStatement(cond, body);
    }

    if (match(Token::FOR)){
        match(Token::ID);  std::string id = previous->text;
        match(Token::ASSIGN);
        Exp* inicio = parseCExp();

        bool isDown = false;
        if (match(Token::TO))        isDown = false;
        else { match(Token::DOWNTO); isDown = true; }

        Exp* fin = parseCExp();
        match(Token::DO);
        Body* cuerpo = parseBlockOrStmt();

        return new ForStatement(id, inicio, fin, isDown, cuerpo);
    }

    cerr << "Sentencia inesperada: " << *current << endl;
    exit(1);
}

/*──────────────── expresiones ────────────*/
Exp* Parser::parseCExp(){
    Exp* left = parseExpression();
    if ( match(Token::LT) || match(Token::LE) ||
         match(Token::GT) || match(Token::GE) ||
         match(Token::EQ) )
    {
        BinaryOp op =
            (previous->type==Token::LT)?LT_OP :
            (previous->type==Token::LE)?LE_OP :
            (previous->type==Token::GT)?GT_OP :
            (previous->type==Token::GE)?GE_OP : EQ_OP;

        Exp* right = parseExpression();
        left = new BinaryExp(left,right,op);
    }
    return left;
}

Exp* Parser::parseExpression(){
    Exp* left = parseTerm();
    while (match(Token::PLUS) || match(Token::MINUS)){
        BinaryOp op = (previous->type==Token::PLUS)?PLUS_OP:MINUS_OP;
        Exp* right  = parseTerm();
        left = new BinaryExp(left,right,op);
    }
    return left;
}
Exp* Parser::parseTerm(){
    Exp* left = parseFactor();
    while (match(Token::MUL) || match(Token::DIV)){
        BinaryOp op = (previous->type==Token::MUL)?MUL_OP:DIV_OP;
        Exp* right  = parseFactor();
        left = new BinaryExp(left,right,op);
    }
    return left;
}
Exp* Parser::parseFactor(){
    if (match(Token::TRUE))  return new BoolExp(1);
    if (match(Token::FALSE)) return new BoolExp(0);
    if (match(Token::NUM))   return new NumberExp(stoi(previous->text));

    if (match(Token::ID)){
        string name = previous->text;
        if (match(Token::PI)){                      // llamada
            auto* call = new FCallExp(); call->nombre = name;
            if (!check(Token::PD)){
                call->argumentos.push_back(parseCExp());
                while (match(Token::COMA))
                    call->argumentos.push_back(parseCExp());
            }
            match(Token::PD);
            return call;
        }
        return new IdentifierExp(name);             // variable
    }

    if (match(Token::PI)){                          // ( exp )
        Exp* e = parseCExp(); match(Token::PD); return e;
    }

    cerr << "Factor inválido: " << *current << endl;
    exit(1);
}


Body* Parser::parseBlockOrStmt() {
    if (check(Token::BEGIN_KW))          // begin … end
        return parseBody();

    /* sentencia simple ⇒ la envolvemos en Body vacío */
    auto* v  = new VarDecList();         // sin var-decs
    auto* sl = new StatementList();
    sl->add( parseStatement() );         // solo UNA sentencia
    return new Body(v, sl);
}