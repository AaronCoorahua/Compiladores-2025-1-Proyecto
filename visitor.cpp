#include "exp.h"
#include "visitor.h"
#include <iostream>
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

    // Guardar el nombre de la funcion actual
    string savedFun = currFun;
    currFun = f->nombre;

    // Cargar parametros
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
    cerr << "[ERROR] visit(RecordVarDec*) no deberia ser llamado en EVALVisitor\n";
    exit(1);
}

void EVALVisitor::visit(AssignStatement* stm) {
    float val = stm->rhs->accept(this); // Evalua RHS

    if (auto* id = dynamic_cast<IdentifierExp*>(stm->lhs)) {
        string var = id->name;

        // Detectar si estamos en una funcion y estamos haciendo: nombre_funcion := valor;
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



//  --------TYPECHECKER_VISITOR---------

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
                e->type = "bool";
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

    // Caso especial: asignacion al nombre de la funcion → es valor de retorno
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
    e->type = "bool";
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
    if (e->cond->type != "bool") {
        cerr << "[TYPE ERROR] La condicion de ifexp debe ser bool\n";
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
        (*arg_it)->accept(this);  // Evalua tipo del argumento
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
    s->e->accept(this);  // Verifica la expresion

    if (s->e->type != "integer" && s->e->type != "real" && s->e->type != "bool") {
        cerr << "[TYPE ERROR] writeln(...) solo acepta int, float o bool\n";
        exit(1);
    }
}


void TYPEVisitor::visit(IfStatement* s) {
    s->condition->accept(this);
    if (s->condition->type != "bool") {
        cerr << "[TYPE ERROR] La condicion del if debe ser de tipo bool\n";
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

    env.add_level();  // variable local del for
    env.add_var(s->id, "integer");
    s->body->accept(this);
    env.remove_level();
}


void TYPEVisitor::visit(WhileStatement* s) {
    s->condition->accept(this);

    if (s->condition->type != "bool") {
        cerr << "[TYPE ERROR] La condicion del while debe ser bool\n";
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
    // Verificar que la variable del record existe
    if (!env.check(s->base)) {
        cout<<"RecordTAssignStatement"<<endl;
        cerr << "[TYPE ERROR] Variable no declarada: " << s->base << "\n";
        exit(1);
    }

    // Verificar que el campo existe dentro del record
    if (!env.has_field(s->base, s->field)) {
        cerr << "[TYPE ERROR] Campo no declarado: " << s->base << "." << s->field << "\n";
        exit(1);
    }

    // Evaluar la expresion del valor
    s->val->accept(this);

    string expected = env.get_field_type(s->base, s->field);
    string actual = s->val->type;


    // Validar que se haya asignado el tipo correctamente
    if (actual.empty()) {
        cerr << "[TYPE ERROR] La expresion del valor no tiene tipo asignado\n";
        exit(1);
    }

    // Comparar tipos
    if (expected != actual) {
        cerr << "[TYPE ERROR] Asignacion incompatible a campo " << s->base << "." << s->field
             << ": se esperaba " << expected << ", se recibio " << actual << "\n";
        exit(1);
    }
}



void TYPEVisitor::visit(VarDec* vd) {
    for (auto& nombre : vd->vars) {
        if (type_registry.count(vd->type)) {
            // Es un tipo RECORD
            env.add_record(nombre, type_registry[vd->type],  vd->type);
        } else {
            // Variable simple
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

    // registrar parametros
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

float ConstCollector::visit(FloatExp* e) {
    std::string lbl = "LC" + std::to_string(floatLabelCount++);
    floatConsts[lbl] = e->value;
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


CodeGenVisitor::CodeGenVisitor(std::ostream& output): out(output), floatLabelCount(0) {}

void CodeGenVisitor::generate(Program* p) {

    ConstCollector collector(floatConsts, floatLabelCount);
    collector.visit(p);

    // 2) Ahora sí imprimimos .data con formatos, variables y literales
    out<<".data\n";
    out<<"print_int_fmt: .string \"%ld\\n\"\n";
    out<<"print_float_fmt: .string \"%f\\n\"\n";
    p->vardecs->accept(this);
    for(auto& kv: floatConsts)
        out<<kv.first<<": .double "<<kv.second<<"\n";

    // 3) Construye el map valor→etiqueta
    for(auto& kv: floatConsts)
        literalLabelMap[kv.second] = kv.first;

    // 4) Sección .text
    out<<".text\n"
       <<".globl main\n"
       <<"main:\n"
       <<"pushq %rbp\n"
       <<"movq %rsp, %rbp\n";
    p->mainBody->accept(this);
    out<<"movq $0, %rax\n"
       <<"popq %rbp\n"
       <<"ret\n";
}

void CodeGenVisitor::visit(VarDecList* v) {
    for(auto* vd: v->vardecs) vd->accept(this);
}

void CodeGenVisitor::visit(VarDec* v) {
    bool vf = (v->type=="real");
    for(auto& name: v->vars) {
        isFloatVar[name] = vf;
        if(vf) out<<name<<": .double 0.0\n";
        else  out<<name<<": .quad 0\n";
    }
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

float CodeGenVisitor::visit(FloatExp* e) {
    auto lbl = literalLabelMap[e->value];
    out<<"movsd "<<lbl<<"(%rip), %xmm0\n";
    return 0;
}

float CodeGenVisitor::visit(IdentifierExp* e) {
    if(isFloatVar[e->name])
        out<<"movsd "<<e->name<<"(%rip), %xmm0\n";
    else
        out<<"movq "<<e->name<<"(%rip), %rax\n";
    return 0;
}

float CodeGenVisitor::visit(BinaryExp* e) {
    // Detectar si alguno es real
    auto isFloat = [&](Exp* x){
        return dynamic_cast<FloatExp*>(x)
               || (dynamic_cast<IdentifierExp*>(x)
                   && isFloatVar[static_cast<IdentifierExp*>(x)->name]);
    };
    bool leftF  = isFloat(e->left);
    bool rightF = isFloat(e->right);

    if (leftF || rightF) {
        // Cargar operando izquierdo en xmm0 (con conversión si es entero)
        if (auto fe = dynamic_cast<FloatExp*>(e->left)) {
            fe->accept(this);
        } else if (auto ie = dynamic_cast<IdentifierExp*>(e->left)) {
            if (isFloatVar[ie->name])
                out<<"movsd "<<ie->name<<"(%rip), %xmm0\n";
            else {
                out<<"movq "<<ie->name<<"(%rip), %rax\n"
                   <<"cvtsi2sd %rax, %xmm0\n";
            }
        } else /* NumberExp */ {
            auto ne = static_cast<NumberExp*>(e->left);
            out<<"movq $"<<ne->value<<", %rax\n"
               <<"cvtsi2sd %rax, %xmm0\n";
        }

        // Cargar operando derecho en xmm1
        if (auto fe = dynamic_cast<FloatExp*>(e->right)) {
            fe->accept(this);
            out<<"movsd %xmm0, %xmm1\n";
        } else if (auto ie = dynamic_cast<IdentifierExp*>(e->right)) {
            if (isFloatVar[ie->name])
                out<<"movsd "<<ie->name<<"(%rip), %xmm1\n";
            else {
                out<<"movq "<<ie->name<<"(%rip), %rax\n"
                   <<"cvtsi2sd %rax, %xmm1\n";
            }
        } else {
            auto ne = static_cast<NumberExp*>(e->right);
            out<<"movq $"<<ne->value<<", %rax\n"
               <<"cvtsi2sd %rax, %xmm1\n";
        }

        // Aplicar la operación
        switch(e->op) {
            case PLUS_OP:  out<<"addsd %xmm1, %xmm0\n"; break;
            case MINUS_OP: out<<"subsd %xmm1, %xmm0\n"; break;
            case MUL_OP:   out<<"mulsd %xmm1, %xmm0\n"; break;
            case DIV_OP:   out<<"divsd %xmm1, %xmm0\n"; break;
            default: break;
        }
        return 0;
    }

    // Caso entero puro
    e->left->accept(this);
    out<<"pushq %rax\n";
    e->right->accept(this);
    out<<"movq %rax, %rbx\n"
       <<"popq %rax\n";
    switch(e->op) {
        case PLUS_OP:  out<<"addq %rbx, %rax\n"; break;
        case MINUS_OP: out<<"subq %rbx, %rax\n"; break;
        case MUL_OP:   out<<"imulq %rbx, %rax\n"; break;
        case DIV_OP:   out<<"cqto\nidivq %rbx\n"; break;
        default: break;
    }
    return 0;
}


void CodeGenVisitor::visit(AssignStatement* s) {
    auto idExp = dynamic_cast<IdentifierExp*>(s->lhs);
    bool lhsIsFloat = isFloatVar[idExp->name];

    if (lhsIsFloat) {
        // 1) Evaluar RHS siempre en %xmm0 como float
        if (auto fe = dynamic_cast<FloatExp*>(s->rhs)) {
            // literal real
            fe->accept(this);                // movsd LCx, %xmm0
        }
        else if (auto ne = dynamic_cast<NumberExp*>(s->rhs)) {
            // literal entero → cvtsi2sd
            out<<"movq $"<<ne->value<<", %rax\n";
            out<<"cvtsi2sd %rax, %xmm0\n";
        }
        else if (auto ie = dynamic_cast<IdentifierExp*>(s->rhs)) {
            // variable
            if (isFloatVar[ie->name]) {
                out<<"movsd "<<ie->name<<"(%rip), %xmm0\n";
            } else {
                out<<"movq "<<ie->name<<"(%rip), %rax\n";
                out<<"cvtsi2sd %rax, %xmm0\n";
            }
        }
        else {
            // expresiones compuestas (BinaryExp, etc.)
            s->rhs->accept(this);            // las BinaryExp ya dejan el resultado en %xmm0
        }

        // 2) Guardar en la variable float
        out<<"movsd %xmm0, "<<idExp->name<<"(%rip)\n";
    }
    else {
        // LHS es entero: todo va por %rax
        if (auto ne = dynamic_cast<NumberExp*>(s->rhs)) {
            out<<"movq $"<<ne->value<<", %rax\n";
        }
        else if (auto ie = dynamic_cast<IdentifierExp*>(s->rhs)) {
            out<<"movq "<<ie->name<<"(%rip), %rax\n";
        }
        else {
            s->rhs->accept(this);            // BinaryExp deja en %rax para enteros
        }
        out<<"movq %rax, "<<idExp->name<<"(%rip)\n";
    }
}


void CodeGenVisitor::visit(PrintStatement* s) {
    bool vf = false;
    if (dynamic_cast<FloatExp*>(s->e)) {
        vf = true;
    } else if (auto id = dynamic_cast<IdentifierExp*>(s->e)) {
        vf = isFloatVar[id->name];
    }
    if (vf) {
        s->e->accept(this);
        out<<"leaq print_float_fmt(%rip), %rdi\n";
        out<<"movb $1, %al\n";
        out<<"call printf@PLT\n";
    } else {
        s->e->accept(this);
        out<<"movq %rax, %rsi\n";
        out<<"leaq print_int_fmt(%rip), %rdi\n";
        out<<"movb $1, %al\n";
        out<<"call printf@PLT\n";
    }
}

void CodeGenVisitor::visit(Program* p) {
    generate(p);
}
