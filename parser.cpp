#include <iostream>
#include <stdexcept>
#include "parser.h"

using namespace std;

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


// Program        ::= 'program' id ';' TypeDecList? VarDecList FunDecList? Body '.'

Program* Parser::parseProgram(){
    auto* p = new Program();

    if (match(Token::PROGRAM)){   
        match(Token::ID);  match(Token::PC);
    }
    p->typeDecList= parseTypeDecList();
    p->vardecs  = parseVarDecList();
    p->fundecs  = parseFunDecList();

    if (!p->fundecs) p->fundecs = new FunDecList();  
    p->mainBody = parseBody();
    match(Token::DOT);

    if (!isAtEnd()){
        cerr << "Texto extra después del '.' final: "<<current<<"\n"; exit(1);
    }
    return p;
}

// Body 	       ::= 'begin' VarDecList? StmtList 'end'

Body* Parser::parseBody(){
    match(Token::BEGIN_KW);
    auto* v = check(Token::VAR) ? parseVarDecList() : new VarDecList();
    auto* s = parseStatementList();
    match(Token::END_KW);
    return new Body(v,s);
}


// TypeDecList    ::= ('type' TypeDec)*

TypeDecList* Parser::parseTypeDecList() {
    auto* l = new TypeDecList();
    if (!match(Token::TYPE)) return l;


    while (check(Token::ID)) {
        TypeDec* t = parseTypeDec();
        if (t) l->add(t);
    }

    return l;
}

// TypeDec        ::= id '=' Type ';'

TypeDec* Parser::parseTypeDec() {
    if (!match(Token::ID)) {
        cerr << "Se espera un ID (nombre del tipo)\n"; exit(1);
    }
    string name = previous->text;

    if (!match(Token::ASSIGN)) {
        cerr << "Se espera '=' después del nombre del tipo\n"; exit(1);
    }

    std::vector<RecordVarDec*> fields = parseRecordType();

    match(Token::PC);

    return new TypeDec(name, fields);

}

// FunDecList     ::= (FunctionDec)*

FunDecList* Parser::parseFunDecList(){
    auto* l = new FunDecList();
    while (true){
        while (match(Token::PC));                 
        FunDec* f = parseFunDec();
        if (!f) break;
        l->add(f);
    }
    return l;
}

//FunctionDec    ::= 'function' id '(' ParamList? ')' ':' Type ';' 'begin' StmtList 'end' ';'   

FunDec* Parser::parseFunDec() {
    if (!match(Token::FUNCTION)) return nullptr;

    auto* f = new FunDec();

    match(Token::ID);
    f->nombre = previous->text;

    match(Token::PI);
    if (!check(Token::PD)) {
        parseParamList(f);  
    }
    match(Token::PD);

    match(Token::COLON);
    match(Token::ID);
    f->tipo = previous->text;
    match(Token::PC);

    f->cuerpo = parseBody();
    match(Token::PC);

    return f;
}


// ParamList      ::= Param (',' Param)*

void Parser::parseParamList(FunDec* f) {
    parseParam(f);  

    while (match(Token::PC)) {  
        parseParam(f);
    }
}


// Param          ::= id ':' Type

void Parser::parseParam(FunDec* f) {
    std::list<std::string> ids;

    match(Token::ID);
    ids.push_back(previous->text);

    while (match(Token::COMA)) {
        match(Token::ID);
        ids.push_back(previous->text);
    }

    match(Token::COLON);
    match(Token::ID);
    std::string ptype = previous->text;

    for (auto& id : ids) {
        f->parametros.push_back(id);
        f->tipos.push_back(ptype);
    }
}

// VarList        ::= id (',' id)*

VarDec* Parser::parseVarDec(){


    list<string> ids;
    match(Token::ID);                 
    ids.push_back(previous->text);
    while (match(Token::COMA)){
        match(Token::ID); ids.push_back(previous->text);
    }

    match(Token::COLON);            
    match(Token::ID);                 
    string type = previous->text;
    match(Token::PC);
    return new VarDec(type, ids);
}



VarDecList* Parser::parseVarDecList(){
    auto* v = new VarDecList();

    if (!match(Token::VAR)) return v;
    while (check(Token::ID)) {         
        VarDec* d = parseVarDec();
        if (d) v->add(d);
    }
    return v;
}

StatementList* Parser::parseStatementList(){
    auto* sl = new StatementList();
    sl->add(parseStatement());
    while (match(Token::PC)){                  
        if (check(Token::END_KW) || check(Token::ELSE)) break;
        sl->add(parseStatement());
    }
    return sl;
}

// record_type    ::= 'record' field_list 'end'

std::vector<RecordVarDec*> Parser::parseRecordType() {
    match(Token::RECORD);                              
    std::vector<RecordVarDec*> fields = parseFieldList();
    match(Token::END_KW);                                
    return fields;
}

// field_list     ::= field (';' field)* ';'

std::vector<RecordVarDec*> Parser::parseFieldList() {
    std::vector<RecordVarDec*> fields;

    
    while (current->type == Token::ID) {
   
        std::list<std::string> names;
        names.push_back(current->text);
        match(Token::ID);

        while (match(Token::COMA)) {
            names.push_back(current->text);
            match(Token::ID);
        }

     
        match(Token::COLON);
        std::string fieldType = current->text;
        match(Token::ID);


        match(Token::PC);

   
        for (auto& nm : names) {
            fields.push_back(new RecordVarDec(nm, fieldType));
        }
    }

    return fields;
}


Stm* Parser::parseStatement(){


    if (match(Token::ID)){

        string id = previous->text;

        if (check(Token::DOT)){
            match(Token::DOT);
            match(Token::ID);
            string id2 = previous->text;
            if (!match(Token::ASSIGN)){
                cerr << "Se espera ':=' después de '" << id << "." << id2 << "'\n";
                exit(1);
            }
            Exp* val = parseCExp();
            return new RecordTAssignStatement(id, id2, val);
        }

        match(Token::ASSIGN);
        Exp* lhs = new IdentifierExp(id);
        Exp* rhs = parseCExp();
        return new AssignStatement(lhs, rhs);

    }



    if (match(Token::WRITELN)){
        match(Token::PI);
        Exp* e = parseCExp();
        match(Token::PD);
        return new PrintStatement(e);
    }

    if (match(Token::RETURN)){
        auto* rs = new ReturnStatement();
        match(Token::PI);
        rs->e = check(Token::PD)? nullptr : parseCExp();
        match(Token::PD);
        return rs;
    }


    if (match(Token::IF)){
        Exp*  cond  = parseCExp();  match(Token::THEN);
        Body* thenB = parseBlockOrStmt();
        Body* elseB = nullptr;
        if (match(Token::ELSE)) elseB = parseBlockOrStmt();
        return new IfStatement(cond, thenB, elseB);
    }


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


Parser::Parser(Scanner* sc):scanner(sc){
    current = scanner->nextToken();
    if (current->type == Token::ERR){
        cerr << "Primer token inválido: " << current->text << endl;
        exit(1);
    }
}


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

//Factor         ::=  float | id ('.' id '(' ArgList? ')' | '.' id | '(' ArgList? ')' ) | '(' CExp ')'

Exp* Parser::parseFactor() {
    if (match(Token::TRUE))   
        return new BoolExp(1);
    if (match(Token::FALSE))  
        return new BoolExp(0);
    if (match(Token::NUM))     
        return new NumberExp(stoi(previous->text));
    if (match(Token::NUM_FLOAT))
        return new FloatExp(stof(previous->text));
    if (match(Token::ID)) {
        string name = previous->text;
        Exp* base = new IdentifierExp(name);
        if (match(Token::DOT)) {
            match(Token::ID);
            string member = previous->text;
            if (match(Token::PI)) {
                std::vector<Exp*> args;
                if (!check(Token::PD)) {
                    do {
                        args.push_back(parseCExp());
                    } while (match(Token::COMA));
                }
                match(Token::PD);
                return new MethodCallExp(base, member, args);
            }
            return new RecordTIdentifierExp(name, member);
        }
        else if (match(Token::PI)) {
            auto* call = new FCallExp();
            call->nombre = name;
            if (!check(Token::PD)) {
                call->argumentos.push_back(parseCExp());
                while (match(Token::COMA))
                    call->argumentos.push_back(parseCExp());
            }
            match(Token::PD);
            return call;
        }
        return base;
    }
    if (match(Token::PI)) {
        Exp* e = parseCExp();
        match(Token::PD);
        return e;
    }

    cerr << "Factor inválido: " << *current
         << "  PREV: " << (previous ? previous->text : "<none>") << endl;
    exit(1);
}


Body* Parser::parseBlockOrStmt() {
    if (check(Token::BEGIN_KW))       
        return parseBody();

    auto* v  = new VarDecList();        
    auto* sl = new StatementList();
    sl->add( parseStatement() );         
    return new Body(v, sl);
}






