#include "visitor.h"
#include "exp.h"
#include <iostream>
#include <unordered_map>
#include <string>
#include <iomanip>
using namespace std;

/* ─────────────────── Despachos accept() generados ──────────────────── */
/* expresiones */
float BinaryExp     ::accept(Visitor* v){ return v->visit(this); }
float NumberExp     ::accept(Visitor* v){ return v->visit(this); }

float FloatExp     ::accept(Visitor* v){ return v->visit(this); }

float BoolExp       ::accept(Visitor* v){ return v->visit(this); }
float IdentifierExp ::accept(Visitor* v){ return v->visit(this); }
float IFExp         ::accept(Visitor* v){ return v->visit(this); }
float FCallExp      ::accept(Visitor* v){ return v->visit(this); }
float RecordTypeAccessExp ::accept(Visitor *v){return  v->visit(this); }
/* sentencias */
int AssignStatement ::accept(Visitor* v){ v->visit(this); return 0; }
int PrintStatement  ::accept(Visitor* v){ v->visit(this); return 0; }
int IfStatement     ::accept(Visitor* v){ v->visit(this); return 0; }
int ForStatement    ::accept(Visitor* v){ v->visit(this); return 0; }
int WhileStatement  ::accept(Visitor* v){ v->visit(this); return 0; }
int ReturnStatement ::accept(Visitor* v){ v->visit(this); return 0; }
/* listas / cuerpos */
int VarDec       ::accept(Visitor* v){ v->visit(this); return 0; }
int VarDecList   ::accept(Visitor* v){ v->visit(this); return 0; }
int StatementList::accept(Visitor* v){ v->visit(this); return 0; }
int Body         ::accept(Visitor* v){ v->visit(this); return 0; }
int TypeDecList  ::accept(Visitor *v){ v->visit(this); return 0; }
int TypeDec      ::accept(Visitor *v){ v->visit(this);return 0; }


/* funciones */
int FunDec     ::accept(Visitor* v){ v->visit(this); return 0; }
int FunDecList ::accept(Visitor* v){ v->visit(this); return 0; }

/* ===================================================================== */
/*                          PrintVisitor                                 */
/* ===================================================================== */
float PrintVisitor::visit(BinaryExp* e){
    e->left->accept(this);
    cout << ' ' << Exp::binopToChar(e->op) << ' ';
    e->right->accept(this);
    return 0;
}
float PrintVisitor::visit(NumberExp* e){ cout << e->value; return 0; }

float PrintVisitor::visit(FloatExp* e)  { cout << fixed << setprecision(2) << e->value; return e->value; }


int PrintVisitor::visit(BoolExp*   e){ cout << (e->value ? "true":"false"); return 0; }
float PrintVisitor::visit(IdentifierExp* e){ cout << e->name; return 0; }
float PrintVisitor::visit(IFExp* e){
    cout << "ifexp(";
    e->cond ->accept(this); cout << ", ";
    e->left ->accept(this); cout << ", ";
    e->right->accept(this); cout << ")";
    return 0;
}
float PrintVisitor::visit(FCallExp*){ return 0; }          /* simplificado */

/* sentencias */
void PrintVisitor::visit(AssignStatement* s){
    cout << s->lhs->accept(this) << " := "; s->rhs->accept(this); cout << ";";
}
void PrintVisitor::visit(PrintStatement* s){
    cout << "writeln("; s->e->accept(this); cout << ");";
}

// Se evita el punto y coma antes de ELSE usando bloques begin..end
void PrintVisitor::visit(IfStatement* s){
    cout << "if "; s->condition->accept(this); cout << " then";

    bool tieneElse = (s->els != nullptr);
    if (tieneElse) cout << " begin" << endl; else cout << ' ';

    s->then->accept(this);

    if (tieneElse){
        cout << endl << "end else begin" << endl;
        s->els->accept(this);
        cout << endl << "end";        // cierra bloque if con else
    }
}
void PrintVisitor::visit(WhileStatement* s){
    cout << "while "; s->condition->accept(this); cout << " do" << endl;
    s->b->accept(this); cout << "end";
}
void PrintVisitor::visit(ReturnStatement*){}

/* bloques y listas */
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
    for (auto st:s->stms){ st->accept(this); cout << endl; }
}
void PrintVisitor::visit(Body* b){
    b->vardecs->accept(this);
    if (!b->vardecs->vardecs.empty()) cout << endl;
    b->slist->accept(this);
}

/* funciones */
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

/* programa completo */
void PrintVisitor::visit(Program* p){ imprimir(p); }
void PrintVisitor::imprimir(Program* p){
    cout << "program out;" << endl << endl;
    p->vardecs->accept(this);
    if (!p->vardecs->vardecs.empty()) cout << endl;
    p->fundecs->accept(this);
    cout << "begin" << endl;
    p->mainBody->accept(this);
    cout << "end." << endl;
}

/* ===================================================================== */
/*                          EVALVisitor                                  */
/* ===================================================================== */

// Se asume que EVALVisitor ahora tiene un miembro std::string currFun;
// definido y puesto a "" en el constructor.

float EVALVisitor::visit(BinaryExp* e){
    float v1=e->left->accept(this), v2=e->right->accept(this);
    switch(e->op){
        case PLUS_OP : return v1+v2;
        case MINUS_OP: return v1-v2;
        case MUL_OP  : return v1*v2;
        case DIV_OP  : return v2? v1/v2 : (cerr<<"Div/0\n",0);
        case LT_OP   : return v1 < v2;
        case LE_OP   : return v1 <= v2;
        case EQ_OP   : return v1 == v2;
        case GT_OP : return v1 >  v2;
        case GE_OP : return v1 >= v2;
        default      : return 0;
    }
}
float EVALVisitor::visit(NumberExp* e){ return e->value; }

float EVALVisitor::visit(FloatExp* e)  { return (e->value); }
int EVALVisitor::visit(BoolExp*   e){ return e->value; }

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

    // Crear nuevo scope
    env.add_level();

    // ❶ Registrar variable resultado como float
    env.add_var(f->nombre, 0.0f, f->tipo);  // ← asegurarse que es float

    // Guardar el nombre de la función actual
    string savedFun = currFun;
    currFun = f->nombre;

    // Cargar parámetros
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

    float ret = retval;  // ← este 'retval' debe ser declarado como float

    env.remove_level();
    currFun = savedFun;

    return ret;
}


/* sentencias */
void EVALVisitor::visit(AssignStatement* s) {
    float val = s->rhs->accept(this);

    // Asignar directamente si es un identificador simple
    if (auto* id = static_cast<IdentifierExp*>(s->lhs)) {
        if (!currFun.empty() && id->name == currFun) {
            retval = val;
            retcall = true;
        }
        if (!env.check(id->name)) {
            cerr << "Var no declarada " << id->name << endl;
            return;
        }
        env.update(id->name, val);
        return;
    }

    // Si es acceso a campo: d.x1
    auto* field = static_cast<RecordTypeAccessExp*>(s->lhs);
    auto* baseId = static_cast<IdentifierExp*>(field->base);
    if (!env.check(baseId->name, field->field)) {
        cerr << "Campo no declarado: " << field->field << " en " << baseId->name << endl;
        return;
    }
    env.update(baseId->name, field->field, val);
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


/* bloques */
void EVALVisitor::visit(VarDec* v){ for (auto& id:v->vars) env.add_var(id,v->type); }
void EVALVisitor::visit(VarDecList* v){ for (auto d:v->vardecs) d->accept(this); }
void EVALVisitor::visit(StatementList* s){ for (auto st:s->stms) st->accept(this); }
void EVALVisitor::visit(Body* b){
    env.add_level();
    b->vardecs->accept(this);
    b->slist ->accept(this);
    env.remove_level();
}

/* funciones */
void EVALVisitor::visit(FunDec* f){ fdecs[f->nombre]=f; }
void EVALVisitor::visit(FunDecList* l){ for (auto f:l->Fundecs) visit(f); }

/* programa */
void EVALVisitor::visit(Program* p){
    env.add_level();
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

int PrintVisitor::visit(RecordTypeAccessExp *) {
    return 0;
}

int PrintVisitor::visit(TypeDec *) {
    return 0;
}

int PrintVisitor::visit(TypeDecList *) {
    return 0;
}

void EVALVisitor::visit(ForStatement* s){
    int ini  = s->start->accept(this);
    int fin  = s->end  ->accept(this);

    env.add_level();                 // nivel local del bucle
    env.add_var(s->id, ini, "int");

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

int EVALVisitor::visit(RecordTypeAccessExp *) {
    return 0;
}

int EVALVisitor::visit(TypeDec *) {
    return 0;
}

int EVALVisitor::visit(TypeDecList *) {
    return 0;
}
