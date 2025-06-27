#ifndef ENV
#define ENV

#include <unordered_map>
#include <list>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

class RecordVarDec;

class Environment {
private:
    vector<unordered_map<string, float>> levels;  // Almacena valores de variables
    vector<unordered_map<string, string>> type_levels;  // Almacena tipos de variables

    unordered_map<string, unordered_map<string, double>> record_values;
    unordered_map<string, unordered_map<string, string>> record_types;

    // Busca el nivel en el que está una variable
    int search_rib(string var) {
        int idx = levels.size() - 1;
        while (idx >= 0) {
            if (levels[idx].find(var) != levels[idx].end()) {
                return idx;
            }
            idx--;
        }
        return -1;
    }

public:
    Environment() {}

    void clear() {
        levels.clear();
        type_levels.clear();
    }

    void add_level() {
        unordered_map<string, float> l;
        unordered_map<string, string> t;
        levels.push_back(l);
        type_levels.push_back(t);
    }

    void add_var(string var, float value, string type) {
        if (levels.empty()) {
            cout << "Environment sin niveles: no se pueden agregar variables" << endl;
            exit(0);
        }
        levels.back()[var] = value;
        type_levels.back()[var] = type;
    }

    void add_var(string var, string type) {
        levels.back()[var] = 0;
        type_levels.back()[var] = type;
    }

    bool remove_level() {
        if (!levels.empty()) {
            levels.pop_back();
            type_levels.pop_back();
            return true;
        }
        return false;
    }

    bool update(string x, float v) {
        int idx = search_rib(x);
        if (idx < 0) return false;
        levels[idx][x] = v;
        return true;
    }

    bool check(string x) {
        return search_rib(x) >= 0;
    }

    float lookup(string x) {
        int idx = search_rib(x);
        if (idx < 0) {
            cout << "Variable no declarada: " << x << endl;
            exit(0);
        }
        return levels[idx][x];
    }

    string lookup_type(string x) {
        int idx = search_rib(x);
        if (idx < 0) {
            cout << "Variable no declarada: " << x << endl;
            exit(0);
        }
        return type_levels[idx][x];
    }

    bool typecheck(string var, string expected_type) {
        string actual_type = lookup_type(var);
        if (actual_type != expected_type) {
            cout << "Error de tipo: se esperaba " << expected_type << " pero se encontró " << actual_type << " para la variable " << var << endl;
            return false;
        }
        return true;
    }

    //----- STRUCTS NUEVOS -----//
    void add_record(string name, const vector<RecordVarDec*>& fields, string record_type) {
        for (auto* f : fields) {
            record_values[name][f->atribute] = 0.0;
            record_types[name][f->atribute] = f->type;
        }
        add_var(name, record_type);
    }

    bool has_field(string record, string field) {
        return record_values.count(record) &&
               record_values[record].count(field);
    }

    double get_field(string record, string field) {
        if (!has_field(record, field)) {
            cerr << "Campo no encontrado: " << record << "." << field << endl;
            exit(1);
        }
        return record_values[record][field];
    }

    void set_field(string record, string field, double value) {
        if (!has_field(record, field)) {
            cerr << "Campo no encontrado: " << record << "." << field << endl;
            exit(1);
        }
        record_values[record][field] = value;
    }

    string get_field_type(string record, string field) {
        if (!has_field(record, field)) {
            cerr << "Campo no encontrado: " << record << "." << field << endl;
            exit(1);
        }
        return record_types[record][field];
    }

    void debug_print() {
        cout << "---- ENTORNO ACTUAL ----\n";
        for (int i = levels.size()-1; i >= 0; i--) {
            cout << "Nivel " << i << ":\n";
            for (auto& [var, val] : levels[i])
                cout << "  " << var << " = " << val << " (" << type_levels[i][var] << ")\n";
        }
        cout << "\n--- RECORDS ---\n";
        for (auto& [record, fields] : record_values) {
            cout << record << ":\n";
            for (auto& [f, v] : fields)
                cout << "  ." << f << " = " << v << " (" << record_types[record][f] << ")\n";
        }
    }
};

#endif
