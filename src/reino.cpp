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

    for (auto &kv : personas) {
        Persona* p = kv.second;
        if (p->id_padre != 0) {
            auto it = personas.find(p->id_padre);
            if (it != personas.end()) {
                Persona* padre = it->second;
                p->padre = padre;
                if (!padre->primogenito) padre->primogenito = p;
                else if (!padre->segundo) padre->segundo = p;
                else {
                    cerr << "Advertencia: padre " << padre->id << " ya tiene 2 hijos. Ignorando id " << p->id << "\n";
                }
            } else {
                cerr << "Advertencia: padre id " << p->id_padre << " no encontrado para id " << p->id << '\n';
            }
        }
    }
}

Persona* Reino::buscar_por_id(int id) const {
    auto it = personas.find(id);
    if (it == personas.end()) return nullptr;
    return it->second;
}

void Reino::recolectar_primogenitura(Persona* nodo, list<Persona*>& salida) const {
    if (!nodo) return;
    salida.push_back(nodo);
    recolectar_primogenitura(nodo->primogenito, salida);
    recolectar_primogenitura(nodo->segundo, salida);
}

string Reino::ruta_rama(Persona* p) const {
    if (!p) return "";
    string ruta = "";
    Persona* cur = p;
    while (cur->padre) {
        Persona* par = cur->padre;
        if (par->primogenito == cur) ruta.push_back('L');
        else if (par->segundo == cur) ruta.push_back('R');
        else ruta.push_back('X');
        cur = par;
    }
    reverse(ruta.begin(), ruta.end());
    return ruta;
}

Persona* Reino::elegir_sucesor_desde_lista(const list<Persona*>& candidatos) const {
    for (Persona* c : candidatos) {
        if (!c) continue;
        if (!c->esta_muerto && c->genero == 'H' && c->edad < 70) return c;
    }
    list<Persona*> fems;
    for (Persona* c : candidatos) {
        if (!c) continue;
        if (!c->esta_muerto && c->genero == 'M' && c->edad > 15) fems.push_back(c);
    }
    if (fems.empty()) return nullptr;
    int min_edad = 1000;
    for (Persona* f : fems) if (f->edad < min_edad) min_edad = f->edad;
    list<Persona*> empate;
    for (Persona* f : fems) if (f->edad == min_edad) empate.push_back(f);
    if (empate.size() == 1) return empate.front();
    Persona* mejor = nullptr;
    string mejor_ruta;
    for (Persona* f : empate) {
        string r = ruta_rama(f);
        if (!mejor || r < mejor_ruta) { mejor = f; mejor_ruta = r; }
    }
    return mejor;
}

void Reino::actualizar_rey_actual() {
    rey_actual = nullptr;
    for (auto &kv : personas) {
        if (kv.second->es_rey) { rey_actual = kv.second; return; }
    }
    if (!rey_actual && ancestro) {
        list<Persona*> todos;
        recolectar_primogenitura(ancestro, todos);
        Persona* candidato = elegir_sucesor_desde_lista(todos);
        if (candidato) {
            candidato->es_rey = true;
            candidato->fue_rey = true;
            rey_actual = candidato;
        }
    }
}

Persona* Reino::obtener_rey_actual() const { return rey_actual; }

void Reino::asignar_nuevo_rey_por_muerte_o_edad() {
    if (!ancestro) return;
    if (!rey_actual) actualizar_rey_actual();
    if (!rey_actual) return;

    if (rey_actual->esta_muerto || rey_actual->edad >= 70) {
        if (rey_actual) rey_actual->es_rey = false;

        list<Persona*> todos;
        recolectar_primogenitura(ancestro, todos);
        Persona* sucesor = elegir_sucesor_desde_lista(todos);
        if (sucesor) {
            sucesor->es_rey = true;
            sucesor->fue_rey = true;
            rey_actual = sucesor;
            cout << "Nuevo rey asignado: " << sucesor->nombre_completo() << "\n";
        } else {
            cout << "No se encontró sucesor elegible.\n";
        }
    }
}

void Reino::mostrar_sucesion() const {
    if (!ancestro) { cout << "Arbol vacío.\n"; return; }

    list<Persona*> todos;
    recolectar_primogenitura(ancestro, todos);

    list<Persona*> varones;
    for (Persona* p : todos) {
        if (!p) continue;
        if (!p->esta_muerto && p->genero == 'H' && p->edad < 70) varones.push_back(p);
    }
    cout << "Linea de sucesion actual (solo vivos y aptos):\n";
    if (rey_actual && !rey_actual->esta_muerto && rey_actual->edad < 70) {
        cout << "1) " << rey_actual->nombre_completo() << " [REY]\n";
    } else {
        cout << "1) (sin rey actual apto)\n";
    }

    int idx = 2;
    if (!varones.empty()) {
        for (Persona* p : todos) {
            if (!p) continue;
            if (p == rey_actual) continue;
            if (!p->esta_muerto && p->genero == 'H' && p->edad < 70) {
                cout << idx << ") " << p->nombre_completo() << " (edad " << p->edad << ")\n";
                idx++;
            }
        }
    } else {
        list<Persona*> fems;
        for (Persona* p : todos) {
            if (!p) continue;
            if (!p->esta_muerto && p->genero == 'M' && p->edad > 15) fems.push_back(p);
        }
        if (fems.empty()) {
            cout << "(No hay candidatos aptos)\n";
            return;
        }
        int n = 0;
        for (auto it = fems.begin(); it != fems.end(); ++it) ++n;
        Persona** arr = new Persona*[n];
        int i = 0;
        for (Persona* p : fems) arr[i++] = p;
        for (int a = 0; a < n-1; ++a)
            for (int b = 0; b < n-1-a; ++b) {
                Persona* A = arr[b];
                Persona* B = arr[b+1];
                if (A->edad > B->edad) {
                    Persona* tmp = arr[b]; arr[b] = arr[b+1]; arr[b+1] = tmp;
                } else if (A->edad == B->edad) {
                    if (ruta_rama(A) > ruta_rama(B)) {
                        Persona* tmp = arr[b]; arr[b] = arr[b+1]; arr[b+1] = tmp;
                    }
                }
            }
        for (int k = 0; k < n; ++k) {
            if (arr[k] == rey_actual) continue;
            cout << idx << ") " << arr[k]->nombre_completo() << " (edad " << arr[k]->edad << ")\n";
            idx++;
        }
        delete [] arr;
    }
}
