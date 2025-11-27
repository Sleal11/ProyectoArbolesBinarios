#include "reino.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;

Reino::Reino() : ancestro(nullptr), rey_actual(nullptr) {}

Reino::~Reino() { eliminar_todo(); }

void Reino::limpiar() {
    for (auto &p : personas) {
        delete p.second;
    }
    personas.clear();
    ancestro = nullptr;
    rey_actual = nullptr;
}

void Reino::eliminar_todo() { limpiar(); }

list<string> Reino::split_csv_line(const string& linea) {
    list<string> res;
    string cur;
    bool in_quotes = false;
    for (char c : linea) {
        if (c == '"') { in_quotes = !in_quotes; continue; }
        if (c == ',' && !in_quotes) {
            res.push_back(cur);
            cur.clear();
        } else cur.push_back(c);
    }
    res.push_back(cur);
    return res;
}

bool Reino::cargar_csv(const string& ruta) {
    ifstream f(ruta);
    if (!f.is_open()) {
        cerr << "No se pudo abrir " << ruta << '\n';
        return false;
    }
    eliminar_todo();

    string linea;
    bool primera = true;
    while (getline(f, linea)) {
        if (linea.empty()) continue;
        if (primera) { primera = false; continue; } // saltar encabezado
        auto cols_list = split_csv_line(linea);

        string cols_arr[9];
        int idx = 0;
        for (auto &s : cols_list) {
            if (idx < 9) cols_arr[idx++] = s;
        }
        if (idx < 9) continue;

        int id = stoi(cols_arr[0]);
        string nombre = cols_arr[1];
        string apellido = cols_arr[2];
        char genero = 'H';
        for (char c : cols_arr[3]) if (!isspace((unsigned char)c)) { genero = c; break; }
        int edad = stoi(cols_arr[4]);
        int id_padre = stoi(cols_arr[5]);
        bool esta_muerto = (stoi(cols_arr[6]) != 0);
        bool fue_rey = (stoi(cols_arr[7]) != 0);
        bool es_rey = (stoi(cols_arr[8]) != 0);

        Persona* p = new Persona(id, nombre, apellido, genero, edad, id_padre, esta_muerto, fue_rey, es_rey);
        personas[id] = p;
    }
    f.close();
    construir_arbol();
    actualizar_rey_actual();
    return true;
}

bool Reino::guardar_csv(const string& ruta) {
    ofstream f(ruta);
    if (!f.is_open()) return false;
    f << "id,nombre,apellido,genero,edad,id_padre,esta_muerto,fue_rey,es_rey\n";
    for (auto &kv : personas) {
        Persona* p = kv.second;
        f << p->id << "," << p->nombre << "," << p->apellido << "," << p->genero << ","
          << p->edad << "," << p->id_padre << "," << (p->esta_muerto?1:0) << ","
          << (p->fue_rey?1:0) << "," << (p->es_rey?1:0) << "\n";
    }
    f.close();
    return true;
}

void Reino::construir_arbol() {
    for (auto &kv : personas) {
        kv.second->primogenito = nullptr;
        kv.second->segundo = nullptr;
        kv.second->padre = nullptr;
    }

    for (auto &kv : personas) {
        Persona* p = kv.second;
        if (p->id_padre == 0) {
            ancestro = p;
            break;
        }
    }

