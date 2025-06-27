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
float RecordTIdentifierExp::accept(Visitor * v){ return v->visit(this); }

/* sentencias */
int AssignStatement ::accept(Visitor* v){ v->visit(this); return 0; }
int PrintStatement  ::accept(Visitor* v){ v->visit(this); return 0; }
int IfStatement     ::accept(Visitor* v){ v->visit(this); return 0; }
int ForStatement    ::accept(Visitor* v){ v->visit(this); return 0; }
int WhileStatement  ::accept(Visitor* v){ v->visit(this); return 0; }
int ReturnStatement ::accept(Visitor* v){ v->visit(this); return 0; }
int RecordTAssignStatement::accept(Visitor *v) { v->visit(this); return 0;}
/* listas / cuerpos */
int VarDec       ::accept(Visitor* v){ v->visit(this); return 0; }
int VarDecList   ::accept(Visitor* v){ v->visit(this); return 0; }
int StatementList::accept(Visitor* v){ v->visit(this); return 0; }
int Body         ::accept(Visitor* v){ v->visit(this); return 0; }
int TypeDecList  ::accept(Visitor *v){ v->visit(this); return 0; }

int TypeDec      ::accept(Visitor *v){ v->visit(this); return 0; }
int RecordVarDec::accept(Visitor *v){ v->visit(this); return 0; }
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

/* sentencias */
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

// Se evita el punto y coma antes de ELSE usando bloques begin..end
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
    if (p->typeDecList) {p->typeDecList->accept(this);
        cout << endl;}
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

/* funciones */
void EVALVisitor::visit(FunDec* f){ fdecs[f->nombre]=f; }
void EVALVisitor::visit(FunDecList* l){ for (auto f:l->Fundecs) visit(f); }

/* programa */
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


float EVALVisitor::visit(RecordTIdentifierExp* e) {
    if(!env.has_field(e->base,e->field)){
        cerr << "Campo no declarado: " << e->base << "." << e->field << endl;
        exit(1);
    }
    return (int) env.get_field(e->base, e->field);
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
    cerr << "[ERROR] visit(RecordVarDec*) no debería ser llamado en EVALVisitor\n";
    exit(1);
}

void EVALVisitor::visit(AssignStatement* stm) {
    float val = stm->rhs->accept(this); // Evalúa RHS

    if (auto* id = dynamic_cast<IdentifierExp*>(stm->lhs)) {
        string var = id->name;

        // Detectar si estamos en una función y estamos haciendo: nombre_funcion := valor;
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

    // En caso se use otra clase para acceso (como RecordTIdentifierExp), también manejarla
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
