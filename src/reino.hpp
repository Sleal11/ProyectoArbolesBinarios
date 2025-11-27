#ifndef REINO_HPP
#define REINO_HPP

#include <string>
#include <map>
#include <list>

using namespace std;

struct Persona {
    int id;
    string nombre;
    string apellido;
    char genero; // 'H' hombre, 'M' mujer
    int edad;
    int id_padre;
    bool esta_muerto;
    bool fue_rey;
    bool es_rey;

    Persona* padre;
    Persona* primogenito;
    Persona* segundo;

    Persona(int _id, const string& _nombre, const string& _apellido, char _genero, int _edad,
            int _id_padre, bool _esta_muerto, bool _fue_rey, bool _es_rey)
        : id(_id), nombre(_nombre), apellido(_apellido), genero(_genero), edad(_edad),
          id_padre(_id_padre), esta_muerto(_esta_muerto), fue_rey(_fue_rey), es_rey(_es_rey),
          padre(nullptr), primogenito(nullptr), segundo(nullptr) {}

    string nombre_completo() const {
        return nombre + " " + apellido;
    }
};

class Reino {
private:
    map<int, Persona*> personas;
    Persona* ancestro;
    Persona* rey_actual;

    void limpiar();
    void eliminar_todo();
    list<string> split_csv_line(const string& linea);
    void construir_arbol();
    void recolectar_primogenitura(Persona* nodo, list<Persona*>& salida) const;
    string ruta_rama(Persona* p) const;
    Persona* elegir_sucesor_desde_lista(const list<Persona*>& candidatos) const;

