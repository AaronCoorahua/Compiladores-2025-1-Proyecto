#include "exp.h"
#include "visitor.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <unordered_map>
#include <cmath>

static std::unordered_map<std::string,std::string> literalLabelMap;

using namespace std;


// Expresiones
float BinaryExp ::accept(Visitor* v){ return v->visit(this); }
float NumberExp ::accept(Visitor* v){ return v->visit(this); }
float FloatExp ::accept(Visitor* v){ return v->visit(this); }
float BoolExp ::accept(Visitor* v){ return v->visit(this); }
float IdentifierExp ::accept(Visitor* v){ return v->visit(this); }
float IFExp ::accept(Visitor* v){ return v->visit(this); }
float FCallExp ::accept(Visitor* v){ return v->visit(this); }
float RecordTIdentifierExp::accept(Visitor * v){ return v->visit(this); }
float MethodCallExp::accept(Visitor * v){ return v->visit(this); }


// Sentencias
int AssignStatement ::accept(Visitor* v){ v->visit(this); return 0; }
int PrintStatement ::accept(Visitor* v){ v->visit(this); return 0; }
int IfStatement ::accept(Visitor* v){ v->visit(this); return 0; }
int ForStatement ::accept(Visitor* v){ v->visit(this); return 0; }
int WhileStatement ::accept(Visitor* v){ v->visit(this); return 0; }
int ReturnStatement ::accept(Visitor* v){ v->visit(this); return 0; }
int RecordTAssignStatement::accept(Visitor *v) { v->visit(this); return 0;}

// List | Body
int VarDec ::accept(Visitor* v){ v->visit(this); return 0; }
int VarDecList ::accept(Visitor* v){ v->visit(this); return 0; }
int StatementList::accept(Visitor* v){ v->visit(this); return 0; }
int Body ::accept(Visitor* v){ v->visit(this); return 0; }
int TypeDecList  ::accept(Visitor *v){ v->visit(this); return 0; }

int TypeDec ::accept(Visitor *v){ v->visit(this); return 0; }
int RecordVarDec::accept(Visitor *v){ v->visit(this); return 0; }

// Funciones
int FunDec ::accept(Visitor* v){ v->visit(this); return 0; }
int FunDecList ::accept(Visitor* v){ v->visit(this); return 0; }

// PRINT VISITOR

float PrintVisitor::visit(BinaryExp* e){
    e->left->accept(this);
    cout << ' ' << Exp::binopToChar(e->op) << ' ';
    e->right->accept(this);
    return 0;
}
float PrintVisitor::visit(NumberExp* e){ cout << e->value; return 0; }

float PrintVisitor::visit(FloatExp* e)  { cout << fixed << setprecision(2) << e->value; return e->value; }

float PrintVisitor::visit(BoolExp*   e){ cout << (e->value ? "true":"false"); return 0; }
float PrintVisitor::visit(IdentifierExp* e){ cout << e->name; return 0; }
float PrintVisitor::visit(IFExp* e){
    cout << "ifexp(";
    e->cond ->accept(this); cout << ", ";
    e->left ->accept(this); cout << ", ";
    e->right->accept(this); cout << ")";
    return 0;
}

float PrintVisitor::visit(FCallExp* e) {
    cout << e->nombre << "(";
    for (auto i : e->argumentos){
        i->accept(this);
        cout<<",";
    }
    cout << ")";
    return 0;
}

// Sentencias

void PrintVisitor::visit(AssignStatement* s) {
    if (auto* id = dynamic_cast<IdentifierExp*>(s->lhs)) {
        cout << id->name << " := ";
        s->rhs->accept(this);
        cout << "; ";
    } else {
        cerr << "[ERROR] AssignStatement con lhs no reconocido en PrintVisitor" << endl;
    }
}


void PrintVisitor::visit(PrintStatement* s){
    cout << "writeln("; s->e->accept(this); cout << ");";
}

void PrintVisitor::visit(IfStatement* s){

    cout << getIndent();
    cout << "if "; s->condition->accept(this); cout << " then";

    bool tieneElse = (s->els != nullptr);
    if (tieneElse) cout << " begin" << endl; else cout << ' ';

    nivel_indentacion++;
    s->then->accept(this);
    nivel_indentacion--;

    if (tieneElse){
        cout<<getIndent();
        cout << endl << "end else begin" << endl;
        nivel_indentacion++;
        s->els->accept(this);
        nivel_indentacion--;
        cout << endl << "end";       
    }
}

void PrintVisitor::visit(WhileStatement* s){
    cout << "while "; s->condition->accept(this); cout << " do" << endl;
    s->b->accept(this); cout << "end";
}

void PrintVisitor::visit(ReturnStatement*){}

void PrintVisitor::visit(VarDec* v){
    for (auto it=v->vars.begin(); it!=v->vars.end(); ++it){
        if (it!=v->vars.begin()) cout << ", ";
        cout << *it;
    }
    cout << " : " << v->type << ";";
}

void PrintVisitor::visit(VarDecList* v){
    if (v->vardecs.empty()) return;
    cout << "var" << endl;
    for (auto d:v->vardecs){ cout << "  "; d->accept(this); cout << endl; }
}

void PrintVisitor::visit(StatementList* s){
    for (auto st:s->stms){
        cout << getIndent();
        st->accept(this);
        cout << endl; }
}

void PrintVisitor::visit(Body* b){
    b->vardecs->accept(this);
    if (!b->vardecs->vardecs.empty()) cout << endl;
    b->slist->accept(this);
}

// Funciones

void PrintVisitor::visit(FunDec* f){
    cout << "function " << f->nombre << "(";
    for (auto t=f->tipos.begin(), p=f->parametros.begin();
         t!=f->tipos.end(); ++t,++p){
        if (t!=f->tipos.begin()) cout << "; ";
        cout << *p << " : " << *t;
    }
    cout << ") : " << f->tipo << ";" << endl;
    cout << "begin" << endl;
    f->cuerpo->accept(this);
    cout << endl << "end;";
}
void PrintVisitor::visit(FunDecList* l){
    for (auto f:l->Fundecs){ f->accept(this); cout << endl << endl; }
}

// Programa

void PrintVisitor::visit(Program* p){ imprimir(p); }
void PrintVisitor::imprimir(Program* p){
    cout << "program out;" << endl << endl;
    if (p->typeDecList) {p->typeDecList->accept(this);
        cout << endl;}
    p->vardecs->accept(this);
    if (!p->vardecs->vardecs.empty()) cout << endl;
    p->fundecs->accept(this);
    cout << "begin" << endl;
    p->mainBody->accept(this);
    cout << "end." << endl;
}

// EVAL VISITOR

float EVALVisitor::visit(BinaryExp* e){
    float v1=e->left->accept(this), v2=e->right->accept(this);
    switch(e->op){
        case PLUS_OP : return v1+v2;
        case MINUS_OP: return v1-v2;
        case MUL_OP : return v1*v2;
        case DIV_OP : return v2? v1/v2 : (cerr<<"Div/0\n",0);
        case LT_OP : return v1 < v2;
        case LE_OP : return v1 <= v2;
        case EQ_OP : return v1 == v2;
        case GT_OP : return v1 >  v2;
        case GE_OP : return v1 >= v2;
        default : return 0;
    }
}
float EVALVisitor::visit(NumberExp* e){ return e->value; }

float EVALVisitor::visit(FloatExp* e)  { return (e->value); }
float EVALVisitor::visit(BoolExp*   e){ return e->value; }

float EVALVisitor::visit(IdentifierExp* e){
    if (!env.check(e->name)){ cerr<<"Var no declarada "<<e->name<<endl; return 0; }
    return env.lookup(e->name);
}
float EVALVisitor::visit(IFExp* e){
    return e->cond->accept(this)? e->left->accept(this)
                                : e->right->accept(this);
}
float EVALVisitor::visit(FCallExp* e) {
    if (!fdecs.count(e->nombre)) {
        cerr << "Func no def: " << e->nombre << endl;
        exit(1);
    }

    FunDec* f = fdecs[e->nombre];

    if (f->parametros.size() != e->argumentos.size()) {
        cerr << "Aridad incorrecta en " << e->nombre << endl;
        exit(1);
    }

    env.add_level();
    env.add_var(f->nombre, 0.0f, f->tipo);  
    string savedFun = currFun;
    currFun = f->nombre;

     auto pit = f->parametros.begin(), tit = f->tipos.begin();
    auto ait = e->argumentos.begin();
    for (; pit != f->parametros.end(); ++pit, ++tit, ++ait)
        env.add_var(*pit, (*ait)->accept(this), *tit);

    retcall = false;
    f->cuerpo->accept(this);

    if (!retcall) {
        cerr << "Func " << e->nombre << " sin return()\n";
        exit(1);
    }

    float ret = retval;

    env.remove_level();
    currFun = savedFun;

    return ret;
}


void EVALVisitor::visit(PrintStatement* s){ cout << s->e->accept(this) << endl; }
void EVALVisitor::visit(IfStatement* s){
    if (s->condition->accept(this))
        s->then->accept(this);
    else if (s->els)
        s->els->accept(this);
}
void EVALVisitor::visit(WhileStatement* s){
    while (s->condition->accept(this))
        s->b->accept(this);
}
void EVALVisitor::visit(ReturnStatement* s){
    retval  = s->e->accept(this);
    retcall = true;
}


void EVALVisitor::visit(VarDec* v) {
    for (auto& id : v->vars) {
        if (type_registry.count(v->type)) {
            env.add_record(id, type_registry[v->type], v->type);
        } else {
            env.add_var(id, v->type);
        }
    }
}

void EVALVisitor::visit(VarDecList* v){ for (auto d:v->vardecs) d->accept(this); }
void EVALVisitor::visit(StatementList* s){ for (auto st:s->stms) st->accept(this); }
void EVALVisitor::visit(Body* b){
    env.add_level();
    b->vardecs->accept(this);
    b->slist ->accept(this);
    env.remove_level();
}

void EVALVisitor::visit(FunDec* f){ fdecs[f->nombre]=f; }
void EVALVisitor::visit(FunDecList* l){ for (auto f:l->Fundecs) visit(f); }

void EVALVisitor::visit(Program* p){
    env.add_level();
    p->typeDecList->accept(this);
    p->vardecs->accept(this);
    p->fundecs->accept(this);
    p->mainBody->accept(this);
    env.remove_level();
}

void EVALVisitor::ejecutar(Program* p){ visit(p); }


void PrintVisitor::visit(ForStatement* s){
    cout << "for " << s->id << " := ";
    s->start->accept(this);
    cout << (s->downto? " downto " : " to ");
    s->end->accept(this);
    cout << " do" << endl;
    s->body->accept(this);
    cout << "end";
}

float PrintVisitor::visit(RecordTIdentifierExp* re) {
    cout << re->base << "." << re->field;
    return 0;
}


void PrintVisitor::visit(RecordTAssignStatement* rs) {
    cout << rs->base << "." << rs->field << " := ";
    rs->val->accept(this);
    cout << ";";
}


void PrintVisitor::visit(TypeDecList* tdl) {
    cout << "type"<<endl;
    for (auto i:tdl->typedecs){
        i->accept(this);
    }
}

void PrintVisitor::visit(TypeDec* td) {
    cout << td->name << " = record" << endl;
    for (auto i : td->atributs) {
        i->accept(this);
    }
    cout << "end;" << endl << endl;
}

void PrintVisitor::visit(RecordVarDec *rv) {
    cout<<"  "<<rv->atribute << " : " << rv->type << ";" << endl;
}

void EVALVisitor::visit(ForStatement* s){
    int ini  = s->start->accept(this);
    int fin  = s->end  ->accept(this);

    env.add_level();               
    env.add_var(s->id, ini, "integer");

    if (!s->downto){
        while (env.lookup(s->id) <= fin){
            s->body->accept(this);
            env.update(s->id, env.lookup(s->id)+1);
        }
    }else{
        while (env.lookup(s->id) >= fin){
            s->body->accept(this);
            env.update(s->id, env.lookup(s->id)-1);
        }
    }
    env.remove_level();
}


float EVALVisitor::visit(RecordTIdentifierExp* e) {
    if (!env.has_field(e->base, e->field)) {
        cerr << "Campo no declarado: " << e->base << "." << e->field << endl;
        exit(1);
    }
    return env.get_field(e->base, e->field);
}
void EVALVisitor::visit(RecordTAssignStatement* s) {
    if (!env.has_field(s->base, s->field)) {
        cerr << "[ERROR] Campo no declarado: " << s->base << "." << s->field << endl;
        exit(1);

    }
    double val = s->val->accept(this);
    env.set_field(s->base, s->field, val);
}

void EVALVisitor::visit(TypeDecList* l) {
    for (auto i :l->typedecs){
        i->accept(this);
    }
}

void EVALVisitor::visit(TypeDec* t) {
    type_registry[t->name] = t->atributs;
}

void EVALVisitor::visit(RecordVarDec* rv) {
    cerr << "[ERROR] visit(RecordVarDec*) no deberia ser llamado en EVALVisitor\n";
    exit(1);
}

void EVALVisitor::visit(AssignStatement* stm) {
    float val = stm->rhs->accept(this); // Evalua RHS

    if (auto* id = dynamic_cast<IdentifierExp*>(stm->lhs)) {
        string var = id->name;

        if (!currFun.empty() && var == currFun) {
            retval = val;
            retcall = true;
        }

        if (!env.check(var)) {
            cerr << "Var no declarada: " << var << endl;
            return;
        }

        env.update(var, val);
        return;
    }

    if (auto* field = dynamic_cast<RecordTIdentifierExp*>(stm->lhs)) {
        if (!env.has_field(field->base, field->field)) {
            cerr << "Campo no declarado: " << field->base << "." << field->field << endl;
            return;
        }
        env.set_field(field->base, field->field, val);
        return;
    }


    cerr << "[ERROR] AssignStatement con lhs no reconocido" << endl;
}

//  VISITOR TYPECHECKER

float TYPEVisitor::visit(BinaryExp* e) {
    e->left->accept(this);
    e->right->accept(this);

    string t1 = e->left->type;
    string t2 = e->right->type;
    switch (e->op) {
        case PLUS_OP:
        case MINUS_OP:
        case MUL_OP:
        case DIV_OP:
            if ((t1 == "integer" || t1 == "real") && (t2 == "integer" || t2 == "real")) {
                e->type = (t1 == "real" || t2 == "real") ? "real" : "integer";
            } else {
                cerr << "[TYPE ERROR] Operacion aritmetica invalida entre '" << t1 << "' y '" << t2 << "'\n";
                exit(1);
            }
            break;

        case LT_OP:
        case LE_OP:
        case EQ_OP:
        case GT_OP:
        case GE_OP:
            if ((t1 == "integer" || t1 == "real") && (t2 == "integer" || t2 == "real")) {
                e->type = "boolean";
            } else {
                cerr << "[TYPE ERROR] Comparacion invalida entre '" << t1 << "' y '" << t2 << "'\n";
                exit(1);
            }
            break;

        default:
            cerr << "[TYPE ERROR] Operador desconocido en BinaryExp\n";
            exit(1);
    }

    return 0;
}





void TYPEVisitor::visit(AssignStatement* stm) {
    auto* id = dynamic_cast<IdentifierExp*>(stm->lhs);

    if (!id) {
        cerr << "[TYPE ERROR] Asignacion invalida: el lado izquierdo no es una variable simple\n";
        exit(1);
    }

    stm->rhs->accept(this);
    if (id->name == currFun) {
        if (fdecs.count(currFun) == 0) {
            cerr << "[TYPE ERROR] Funcion no declarada: " << currFun << "\n";
            exit(1);
        }
        string expected = fdecs[currFun]->tipo;
        if (stm->rhs->type != expected) {
            cerr << "[TYPE ERROR] Tipo incorrecto al asignar retorno de " << currFun
                 << ": se esperaba " << expected << ", se recibio " << stm->rhs->type << "\n";
            exit(1);
        }
        return;
    }

    if (!env.check(id->name)) {
        cerr << "[TYPE ERROR] Variable no declarada: " << id->name << "\n";
        exit(1);
    }

    if (env.lookup_type(id->name) != stm->rhs->type) {
        cerr << "[TYPE ERROR] Tipos incompatibles en asignacion a " << id->name << "\n";
        exit(1);
    }
}



void TYPEVisitor::visit(Program * p) {
    env.add_level();
    if(p->typeDecList){
        p->typeDecList->accept(this);
    }
    if(p->vardecs){
        p->vardecs->accept(this);
    }
    if(p->fundecs){
        p->fundecs->accept(this);
    }
    if (p->mainBody) {
        p->mainBody->accept(this);
    }
    env.remove_level();
}

float TYPEVisitor::visit(NumberExp* e) {
    e->type = "integer";
    return 0;
}


float TYPEVisitor::visit(FloatExp *e) {
    e->type = "real";
    return 0;
}

float TYPEVisitor::visit(BoolExp *e) {
    e->type = "boolean";
    return 0;
}

float TYPEVisitor::visit(IdentifierExp* e) {
    if (!env.check(e->name)) {
        cout<<"IdentifierExp"<<endl;
        cerr << "[TYPE ERROR] Variable no declarada: " << e->name << endl;
        exit(1);
    }
    e->type = env.lookup_type(e->name);
    return 0;
}


float TYPEVisitor::visit(IFExp* e) {
    e->cond->accept(this);
    e->left->accept(this);
    e->right->accept(this);
    if (e->cond->type != "boolean") {
        cerr << "[TYPE ERROR] La condicion de ifexp debe ser boolean\n";
        exit(1);
    }

    if (e->left->type != e->right->type) {
        cerr << "[TYPE ERROR] Los valores de then y else deben tener el mismo tipo\n";
        exit(1);
    }

    e->type = e->left->type;
    return 0;
}


float TYPEVisitor::visit(FCallExp* e) {
    if (!fdecs.count(e->nombre)) {
        cerr << "[TYPE ERROR] Funcion no definida: " << e->nombre << endl;
        exit(1);
    }

    FunDec* f = fdecs[e->nombre];

    if (f->parametros.size() != e->argumentos.size()) {
        cerr << "[TYPE ERROR] Numero incorrecto de argumentos en llamada a " << e->nombre << endl;
        exit(1);
    }
    auto arg_it = e->argumentos.begin();
    auto type_it = f->tipos.begin();
    for (; arg_it != e->argumentos.end(); ++arg_it, ++type_it) {
        (*arg_it)->accept(this); 
        if ((*arg_it)->type != *type_it) {
            cerr << "[TYPE ERROR] Tipo incorrecto en llamada a " << e->nombre
                 << ": se esperaba '" << *type_it << "', se recibio '" << (*arg_it)->type << "'\n";
            exit(1);
        }
    }
    e->type = f->tipo;
    return 0;
}


float TYPEVisitor::visit(RecordTIdentifierExp* e) {
    if (!env.has_field(e->base, e->field)) {
        cerr << "[TYPE ERROR] Campo no declarado: " << e->base << "." << e->field << endl;
        exit(1);
    }

    e->type = env.get_field_type(e->base, e->field);
    return 0;
}


void TYPEVisitor::visit(PrintStatement* s) {
    s->e->accept(this);  

    if (s->e->type != "integer" && s->e->type != "real" && s->e->type != "boolean") {
        cerr << "[TYPE ERROR] writeln(...) solo acepta int, float o boolean\n";
        exit(1);
    }
}


void TYPEVisitor::visit(IfStatement* s) {
    s->condition->accept(this);
    if (s->condition->type != "boolean") {
        cerr << "[TYPE ERROR] La condicion del if debe ser de tipo boolean\n";
        exit(1);
    }

    s->then->accept(this);
    if (s->els) s->els->accept(this);
}


void TYPEVisitor::visit(ForStatement* s) {
    s->start->accept(this);
    s->end->accept(this);

    if (s->start->type != "integer" || s->end->type != "integer") {
        cerr << "[TYPE ERROR] Las expresiones de inicio y fin del for deben ser int\n";
        exit(1);
    }

    env.add_level();  
    env.add_var(s->id, "integer");
    s->body->accept(this);
    env.remove_level();
}


void TYPEVisitor::visit(WhileStatement* s) {
    s->condition->accept(this);

    if (s->condition->type != "boolean") {
        cerr << "[TYPE ERROR] La condicion del while debe ser boolean\n";
        exit(1);
    }

    s->b->accept(this);
}


void TYPEVisitor::visit(ReturnStatement* s) {
    if (!s->e) return;

    s->e->accept(this);

    if (currFun.empty()) {
        cerr << "[TYPE ERROR] return() fuera de funcion\n";
        exit(1);
    }

    string expected = fdecs[currFun]->tipo;
    string actual   = s->e->type;

    if (expected != actual) {
        cerr << "[TYPE ERROR] return() incompatible en funcion " << currFun
             << ": se esperaba " << expected << ", se obtuvo " << actual << "\n";
        exit(1);
    }
}


void TYPEVisitor::visit(RecordTAssignStatement* s) {
    if (!env.check(s->base)) {
        cout<<"RecordTAssignStatement"<<endl;
        cerr << "[TYPE ERROR] Variable no declarada: " << s->base << "\n";
        exit(1);
    }

    if (!env.has_field(s->base, s->field)) {
        cerr << "[TYPE ERROR] Campo no declarado: " << s->base << "." << s->field << "\n";
        exit(1);
    }

    s->val->accept(this);

    string expected = env.get_field_type(s->base, s->field);
    string actual = s->val->type;

    if (actual.empty()) {
        cerr << "[TYPE ERROR] La expresion del valor no tiene tipo asignado\n";
        exit(1);
    }

    if (expected != actual) {
        cerr << "[TYPE ERROR] Asignacion incompatible a campo " << s->base << "." << s->field
             << ": se esperaba " << expected << ", se recibio " << actual << "\n";
        exit(1);
    }
}



void TYPEVisitor::visit(VarDec* vd) {
    for (auto& nombre : vd->vars) {
        if (type_registry.count(vd->type)) {
            env.add_record(nombre, type_registry[vd->type],  vd->type);
        } else {
            env.add_var(nombre,  vd->type);
        }
    }
}





void TYPEVisitor::visit(VarDecList* vdl) {
    for (auto* vd : vdl->vardecs) {
        vd->accept(this);
    }
}




void TYPEVisitor::visit(StatementList* sl) {
    for (auto* s : sl->stms) {
        s->accept(this);
    }
}


void TYPEVisitor::visit(Body* b) {
    env.add_level();
    if (b->vardecs) b->vardecs->accept(this);
    if (b->slist) b->slist->accept(this);
    env.remove_level();
}


void TYPEVisitor::visit(TypeDecList* tdl) {
    for (auto* td : tdl->typedecs) {
        td->accept(this);
    }
}


void TYPEVisitor::visit(TypeDec* td) {
    current_fields.clear();
    for (auto* campo : td->atributs) {
        campo->accept(this);
    }

    type_registry[td->name] = td->atributs;
}

void TYPEVisitor::visit(RecordVarDec* rv) {
    string tipo = rv->type;


    if (current_fields.count(rv->atribute)) {
        cerr << "[TYPE ERROR] Campo redeclarado en record: " << rv->atribute << "\n";
        exit(1);
    }

    current_fields[rv->atribute] = tipo;
}



void TYPEVisitor::visit(FunDec* f) {
    fdecs[f->nombre] = f;
    env.add_level();

    auto pit = f->parametros.begin();
    auto tit = f->tipos.begin();

    for (; pit != f->parametros.end(); ++pit, ++tit) {
        env.add_var(*pit, *tit);
    }

    currFun = f->nombre;
    f->cuerpo->accept(this);
    currFun = "";
    env.remove_level();
}


void TYPEVisitor::visit(FunDecList* fdl) {

    for (auto* fd : fdl->Fundecs) {
        if (fdecs.count(fd->nombre)) {
            cerr << "[TYPE ERROR] Funcion redeclarada: " << fd->nombre << "\n";
            exit(1);
        }
        fdecs[fd->nombre] = fd;
    }

    for (auto* fd : fdl->Fundecs) {
        fd->accept(this);
    }
}



ConstCollector::ConstCollector(std::map<std::string, double>& fc,int& cnt): floatConsts(fc), floatLabelCount(cnt) {}

float ConstCollector::visit(FCallExp* e)
{
    for (auto* arg : e->argumentos)
        arg->accept(this);       
    return 0;
}


float ConstCollector::visit(FloatExp* e){
    std::string key = std::to_string(e->value);         
    if(!floatConsts.count(key)){                       
        std::string lbl = "LC" + std::to_string(floatLabelCount++);
        floatConsts[key] = e->value;
        literalLabelMap[key] = lbl;                    
    }
    return 0;
}

float ConstCollector::visit(BinaryExp* e) {
    e->left->accept(this);
    e->right->accept(this);
    return 0;
}

void ConstCollector::visit(AssignStatement* s) {
    s->rhs->accept(this);
}

void ConstCollector::visit(PrintStatement* s) {
    s->e->accept(this);
}

void ConstCollector::visit(StatementList* s) {
    for (auto* st : s->stms)
        st->accept(this);
}

void ConstCollector::visit(Body* b) {
    b->slist->accept(this);
}

void ConstCollector::visit(Program* p) {
    p->vardecs->accept(this);
    p->mainBody->accept(this);
}

void ConstCollector::visit(RecordTAssignStatement* s) {
    s->val->accept(this);
}

CodeGenVisitor::CodeGenVisitor(std::ostream& output): out(output), floatLabelCount(0) {}



void CodeGenVisitor::generate(Program* p)
{
    registrarVariables(p);                  

    if (p->typeDecList) p->typeDecList->accept(this);
    ConstCollector collector(floatConsts, floatLabelCount);
    collector.visit(p);                    
    out << ".data\n";
    out << "print_int_fmt:   .string \"%ld\\n\"\n";
    out << "print_float_fmt: .string \"%f\\n\"\n";

    for (auto& kv : floatConsts) {
        const std::string& lbl = ::literalLabelMap[kv.first];
        out << lbl << ": .double " << kv.second << "\n";
    }

    if (p->vardecs) p->vardecs->accept(this);

    for (auto& [name,type] : varTypes) {
        if (recordFieldTypes.count(type)) continue;
        out << name << (isFloatVar[name] ? ": .double 0.0\n"
                                         : ": .quad   0\n");
    }

    out << ".text\n";
    if (p->fundecs) p->fundecs->accept(this);

    out << ".globl main\n"
        << "main:\n"
        << "  pushq %rbp\n"
        << "  movq  %rsp, %rbp\n";

    p->mainBody->accept(this);

    out << "  movq $0, %rax\n"
        << "  popq %rbp\n"
        << "  ret\n";
}

void CodeGenVisitor::registrarVariables(Program* p) {
    if (p->fundecs) {
        for (auto* f : p->fundecs->Fundecs) {
            for (auto pit = f->parametros.begin(), tit = f->tipos.begin();
                 pit != f->parametros.end(); ++pit, ++tit) {
                varTypes[*pit] = *tit;
                isFloatVar[*pit] = (*tit == "real");
                 }

            std::string retvar = "__ret_" + f->nombre;
            varTypes[retvar] = f->tipo;
            isFloatVar[retvar] = (f->tipo == "real");
        }
    }
}



void CodeGenVisitor::visit(TypeDecList* tdl) {
    for (auto* td : tdl->typedecs)
        td->accept(this);
}

void CodeGenVisitor::visit(TypeDec* td) {
    int offset = 0;
    for (auto* field : td->atributs) {
        recordFieldTypes[td->name][field->atribute] = field->type;
        recordLayouts[td->name][field->atribute] = offset;
        offset += 8;
    }
}

void CodeGenVisitor::visit(RecordVarDec* rv) {

}

float CodeGenVisitor::visit(RecordTIdentifierExp* e) {
    std::string recType = varTypes[e->base];
    auto& layout = recordLayouts[recType];
    std::string lbl = e->base + "." + e->field;

    if (isFloatVar[lbl]) {
        out << "movsd " << lbl << "(%rip), %xmm0\n";
    } else {
        out << "movq  " << lbl << "(%rip), %rax\n";
    }
    return 0;
}

void CodeGenVisitor::visit(RecordTAssignStatement* s) {
    std::string recType = varTypes[s->base];
    std::string lbl = s->base + "." + s->field;
    bool vf = isFloatVar[lbl];
    s->val->accept(this);
    if (vf) out << "movsd %xmm0, " << lbl << "(%rip)\n";
    else    out << "movq  %rax,  " << lbl << "(%rip)\n";
}
void CodeGenVisitor::visit(VarDecList* v) {
    for(auto* vd: v->vardecs) vd->accept(this);
}

void CodeGenVisitor::visit(VarDec* v) {

    if(recordFieldTypes.count(v->type)) {
        for(auto& name: v->vars) {
            varTypes[name] = v->type;
            for(auto& [field, ftype]: recordFieldTypes[v->type]) {
                std::string lbl = name + "." + field;
                isFloatVar[lbl] = (ftype == "real");
                if(ftype == "real")
                    out<< lbl <<": .double 0.0\n";
                else
                    out<< lbl <<": .quad   0\n";
            }
        }
        return;
    }
    bool vf = (v->type == "real");
    for (auto& name : v->vars) {
        varTypes[name]   = v->type;
        isFloatVar[name] = vf;


    }
}

float CodeGenVisitor::visit(IdentifierExp* e) {
    if (isFloatVar[e->name]) {
        out << "movsd " << e->name << "(%rip), %xmm0\n";
    } else {
        out << "movq  " << e->name << "(%rip), %rax\n";
    }
    return 0;
}



void CodeGenVisitor::visit(StatementList* s) {
    for(auto* st: s->stms) st->accept(this);
}

void CodeGenVisitor::visit(Body* b) {
    b->slist->accept(this);
}

float CodeGenVisitor::visit(NumberExp* e) {
    out<<"movq $"<<e->value<<", %rax\n"

    <<"cvtsi2sd %rax, %xmm0\n";

    return 0;
}

float CodeGenVisitor::visit(FloatExp* e)
{
    std::string key = std::to_string(e->value);
    out << "movsd " << literalLabelMap[key] << "(%rip), %xmm0\n";
    return 0;
}


float CodeGenVisitor::visit(FCallExp* e) {
    int i = 0;
    for (auto* arg : e->argumentos) {
        const char* reg = (i == 0 ? "%rdi" : "%rsi");
        if (auto ne = dynamic_cast<NumberExp*>(arg)) {
            out << "  movq $" << ne->value << ", " << reg << "\n";
        }
        else if (auto id = dynamic_cast<IdentifierExp*>(arg)) {
            if (isFloatVar[id->name]) {
                out << "  movsd " << id->name << "(%rip), %xmm" << i << "\n";
            } else {
                out << "  movq " << id->name << "(%rip), " << reg << "\n";
            }
        }
        else if (auto rec = dynamic_cast<RecordTIdentifierExp*>(arg)) {
            std::string lbl = rec->base + "." + rec->field;
            if (isFloatVar[lbl]) {
                out << "  movsd " << lbl << "(%rip), %xmm" << i << "\n";
            } else {
                out << "  movq " << lbl << "(%rip), " << reg << "\n";
            }
        }
        else {
            arg->accept(this);  
        }
        ++i;
    }

    out << "  call " << e->nombre << "\n";

    return 0;
}



float CodeGenVisitor::visit(BinaryExp* e)
{
    auto isFloatOperand = [&](Exp* x) -> bool {
        if (dynamic_cast<FloatExp*>(x)) return true;
        if (auto id = dynamic_cast<IdentifierExp*>(x)) return isFloatVar[id->name];
        if (auto rec = dynamic_cast<RecordTIdentifierExp*>(x)) {
            return isFloatVar[rec->base + "." + rec->field];
        }
        return false;
    };

    bool leftF  = isFloatOperand(e->left);
    bool rightF = isFloatOperand(e->right);

    if (leftF || rightF)
    {
        // cargar operando izquierdo en xmm0
        if (auto* fe = dynamic_cast<FloatExp*>(e->left)) {
            std::string lbl = literalLabelMap[std::to_string(fe->value)];
            out << "movsd " << lbl << "(%rip), %xmm0\n";
        }
        else if (auto* id = dynamic_cast<IdentifierExp*>(e->left)) {
            if (isFloatVar[id->name]) {
                out << "movsd " << id->name << "(%rip), %xmm0\n";
            } else {
                out << "movq " << id->name << "(%rip), %rax\n";
                out << "cvtsi2sd %rax, %xmm0\n";
            }
        }
        else if (auto* rec = dynamic_cast<RecordTIdentifierExp*>(e->left)) {
            std::string lbl = rec->base + "." + rec->field;
            if (isFloatVar[lbl]) {
                out << "movsd " << lbl << "(%rip), %xmm0\n";
            } else {
                out << "movq " << lbl << "(%rip), %rax\n";
                out << "cvtsi2sd %rax, %xmm0\n";
            }
        }
        else {
            e->left->accept(this);
            if (!leftF) {
                out << "cvtsi2sd %rax, %xmm0\n";
            }
        }

        // cargar operando derecho en xmm1
        if (auto* fe = dynamic_cast<FloatExp*>(e->right)) {
            std::string lbl = literalLabelMap[std::to_string(fe->value)];
            out << "movsd " << lbl << "(%rip), %xmm1\n";
        }
        else if (auto* id = dynamic_cast<IdentifierExp*>(e->right)) {
            if (isFloatVar[id->name]) {
                out << "movsd " << id->name << "(%rip), %xmm1\n";
            } else {
                out << "movq " << id->name << "(%rip), %rax\n";
                out << "cvtsi2sd %rax, %xmm1\n";
            }
        }
        else if (auto* rec = dynamic_cast<RecordTIdentifierExp*>(e->right)) {
            std::string lbl = rec->base + "." + rec->field;
            if (isFloatVar[lbl]) {
                out << "movsd " << lbl << "(%rip), %xmm1\n";
            } else {
                out << "movq " << lbl << "(%rip), %rax\n";
                out << "cvtsi2sd %rax, %xmm1\n";
            }
        }
        else {
            e->right->accept(this);
            if (!rightF) {
                out << "cvtsi2sd %rax, %xmm1\n";
            }
        }

        switch (e->op) {
            case PLUS_OP:
                out << "addsd %xmm1, %xmm0\n";
                break;
            case MINUS_OP:
                out << "subsd %xmm1, %xmm0\n";
                break;
            case MUL_OP:
                out << "mulsd %xmm1, %xmm0\n";
                break;
            case DIV_OP:
                out << "divsd %xmm1, %xmm0\n";
                break;
            case LT_OP:
                out << "ucomisd %xmm1, %xmm0\n";
                out << "setb %al\n";
                out << "movzbq %al, %rax\n";
                break;
            case GT_OP:
                out << "ucomisd %xmm1, %xmm0\n";
                out << "seta %al\n";
                out << "movzbq %al, %rax\n";
                break;
            case LE_OP:
                out << "ucomisd %xmm1, %xmm0\n";
                out << "setbe %al\n";
                out << "movzbq %al, %rax\n";
                break;
            case GE_OP:
                out << "ucomisd %xmm1, %xmm0\n";
                out << "setae %al\n";
                out << "movzbq %al, %rax\n";
                break;
            case EQ_OP:
                out << "ucomisd %xmm1, %xmm0\n";
                out << "sete %al\n";
                out << "movzbq %al, %rax\n";
                break;
            default:
                break;
        }
        return 0;
    }

    // enteros
    e->left->accept(this);
    out << "pushq %rax\n";
    e->right->accept(this);
    out << "movq %rax, %rbx\n";
    out << "popq %rax\n";

    switch (e->op) {
        case PLUS_OP:
            out << "addq %rbx, %rax\n";
            break;
        case MINUS_OP:
            out << "subq %rbx, %rax\n";
            break;
        case MUL_OP:
            out << "imulq %rbx, %rax\n";
            break;
        case DIV_OP:
            out << "cqto\n";
            out << "idivq %rbx\n";
            break;
        case LT_OP:
            out << "cmpq %rbx, %rax\n";
            out << "setl %al\n";
            out << "movzbq %al, %rax\n";
            break;
        case GT_OP:
            out << "cmpq %rbx, %rax\n";
            out << "setg %al\n";
            out << "movzbq %al, %rax\n";
            break;
        case LE_OP:
            out << "cmpq %rbx, %rax\n";
            out << "setle %al\n";
            out << "movzbq %al, %rax\n";
            break;
        case GE_OP:
            out << "cmpq %rbx, %rax\n";
            out << "setge %al\n";
            out << "movzbq %al, %rax\n";
            break;
        case EQ_OP:
            out << "cmpq %rbx, %rax\n";
            out << "sete %al\n";
            out << "movzbq %al, %rax\n";
            break;
        default:
            break;
    }
    return 0;
}


void CodeGenVisitor::visit(AssignStatement* s) {
    auto idExp = dynamic_cast<IdentifierExp*>(s->lhs);
    if (!idExp) return;

    if (!currFun.empty() && idExp->name == currFun) {
        idExp->name = "__ret_" + currFun;
    }

    bool lhsIsFloat = isFloatVar[idExp->name];

    if (lhsIsFloat) {
        if (auto fe = dynamic_cast<FloatExp*>(s->rhs)) {

            fe->accept(this);
        }
        else if (auto ne = dynamic_cast<NumberExp*>(s->rhs)) {
            out << "movq $" << ne->value << ", %rax\n";
            out << "cvtsi2sd %rax, %xmm0\n";

        }
        else if (auto ie = dynamic_cast<IdentifierExp*>(s->rhs)) {
            if (isFloatVar[ie->name]) {

                out << "movsd " << ie->name << "(%rip), %xmm0\n";
            } else {
                out << "movq " << ie->name << "(%rip), %rax\n";
                out << "cvtsi2sd %rax, %xmm0\n";

            }
        }
        else {
            s->rhs->accept(this);
        }

        out << "movsd %xmm0, " << idExp->name << "(%rip)\n";
    } else {


        if (auto ne = dynamic_cast<NumberExp*>(s->rhs)) {
            out << "movq $" << ne->value << ", %rax\n";
        }
        else if (auto ie = dynamic_cast<IdentifierExp*>(s->rhs)) {
            out << "movq " << ie->name << "(%rip), %rax\n";
        }
        else {
            s->rhs->accept(this);
        }
        out << "movq %rax, " << idExp->name << "(%rip)\n";
    }
}

#include <functional>

void CodeGenVisitor::visit(PrintStatement* s)
{
    std::function<bool(Exp*)> isFloatExpr;

    isFloatExpr = [&](Exp* x) -> bool
    {
        if (dynamic_cast<FloatExp*>(x)) return true;
        if (auto id  = dynamic_cast<IdentifierExp*>(x)) return isFloatVar[id->name];
        if (auto rec = dynamic_cast<RecordTIdentifierExp*>(x))
            return isFloatVar[rec->base + "." + rec->field];
        if (auto be  = dynamic_cast<BinaryExp*>(x))
            return isFloatExpr(be->left) || isFloatExpr(be->right);
        if (auto fc  = dynamic_cast<FCallExp*>(x))
            return isFloatVar["__ret_" + fc->nombre];
        return false;
    };

    bool vf = isFloatExpr(s->e);

    s->e->accept(this);

    if (vf) {
        out << "leaq print_float_fmt(%rip), %rdi\n";
        out << "movb $1, %al\n";
    } else {
        out << "movq %rax, %rsi\n";
        out << "leaq print_int_fmt(%rip), %rdi\n";
        out << "movb $0, %al\n";
    }
    out << "call printf@PLT\n";
}
void CodeGenVisitor::visit(FunDecList* fl) {
    for (auto* f : fl->Fundecs) {
        f->accept(this);
        out<<"\n";
    }
}



void CodeGenVisitor::visit(FunDec* f)
{
    out << ".globl " << f->nombre << "\n";
    out << f->nombre << ":\n";
    out << "  pushq %rbp\n";
    out << "  movq  %rsp, %rbp\n";

    currFun = f->nombre;

    int i = 0;
    for (auto pit = f->parametros.begin(), tit = f->tipos.begin();
         pit != f->parametros.end(); ++pit, ++tit, ++i)
    {
        std::string name = *pit;
        std::string type = *tit;
        varTypes[name] = type;
        isFloatVar[name] = (type == "real");

        if (type == "real")
            out << "  movsd %xmm" << i << ", " << name << "(%rip)\n";
        else
            out << "  movq  %"   << (i == 0 ? "rdi" : "rsi") << ", "
                << name << "(%rip)\n";
    }

    std::string retvar = "__ret_" + f->nombre;
    varTypes[retvar] = f->tipo;
    isFloatVar[retvar] = (f->tipo == "real");

    f->cuerpo->accept(this);

    out << ".Lend_" << f->nombre << ":\n";
    if (f->tipo == "real")
        out << "  movsd " << retvar << "(%rip), %xmm0\n";
    else
        out << "  movq  " << retvar << "(%rip), %rax\n";

    out << "  popq %rbp\n";
    out << "  ret\n";

    currFun.clear();
}

void CodeGenVisitor::visit(ReturnStatement* s)
{
    if (s->e) s->e->accept(this);

    std::string retvar = "__ret_" + currFun;
    if (isFloatVar[retvar])
        out << "  movsd %xmm0, " << retvar << "(%rip)\n";
    else
        out << "  movq  %rax,  " << retvar << "(%rip)\n";

    out << "  jmp .Lend_" << currFun << "\n";
}

void CodeGenVisitor::visit(WhileStatement* s) {
    static int labelCount = 0;
    int id = labelCount++;

    std::string startLabel = "while_start_" + std::to_string(id);
    std::string endLabel   = "while_end_" + std::to_string(id);

    out << startLabel << ":\n";
    s->condition->accept(this);
    out << "  cmpq $0, %rax\n";
    out << "  je " << endLabel << "\n";

    s->b->accept(this);
    out << "  jmp " << startLabel << "\n";
    out << endLabel << ":\n";
}

void CodeGenVisitor::visit(IfStatement* s) {
    static int labelCount = 0;
    int id = labelCount++;

    std::string elseLabel = "else_" + std::to_string(id);
    std::string endLabel  = "endif_" + std::to_string(id);

    s->condition->accept(this);  
    out << "  cmpq $0, %rax\n";
    out << "  je " << elseLabel << "\n";

    s->then->accept(this);
    out << "  jmp " << endLabel << "\n";

    out << elseLabel << ":\n";
    if (s->els) s->els->accept(this);
    out << endLabel << ":\n";
}

void CodeGenVisitor::visit(ForStatement* s) {
    static int labelCount = 0;
    int id = labelCount++;

    std::string loopLabel = "for_loop_" + std::to_string(id);
    std::string endLabel  = "for_end_" + std::to_string(id);
 
    s->start->accept(this);
    out << "  movq %rax, " << s->id << "(%rip)\n";

    std::string cmpOp = s->downto ? "jl" : "jg";
    std::string incOp = s->downto ? "subq $1" : "addq $1";

    out << loopLabel << ":\n";
    out << "  movq " << s->id << "(%rip), %rax\n";
    s->end->accept(this);
    out << "  cmpq %rax, " << s->id << "(%rip)\n";
    out << "  " << cmpOp << " " << endLabel << "\n";

    s->body->accept(this);

    out << "  " << incOp << ", " << s->id << "(%rip)\n";
    out << "  jmp " << loopLabel << "\n";
    out << endLabel << ":\n";
}

void CodeGenVisitor::visit(Program* p) {
    generate(p);
}

float PrintVisitor::visit(MethodCallExp* e) {
    e->base->accept(this);
    std::cout << "." << e->method << "(";
    for (size_t i = 0; i < e->args.size(); ++i) {
        if (i) std::cout << ", ";
        e->args[i]->accept(this);
    }
    std::cout << ")";
    return 0;
}

float EVALVisitor::visit(MethodCallExp* e) {
    float v = e->base->accept(this);
    return std::floor(v + 0.5f);
}

float TYPEVisitor::visit(MethodCallExp* e) {
    e->base->accept(this);
    e->type = "integer";
    return 0;
}

// Convert with Truncation Scalar Double-Precision Floating-Point to Signed Integer
float CodeGenVisitor::visit(MethodCallExp* e) {
    e->base->accept(this);
    out << "  roundsd $0, %xmm0, %xmm0\n";
    out << "  cvttsd2si %xmm0, %rax\n";
    return 0;
}

