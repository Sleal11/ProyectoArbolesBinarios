#include "reino.hpp"
#include <iostream>
#include <string>

using namespace std;

void mostrar_arbol(Persona* p, int nivel = 0) {
    if (!p) return;
    for (int i = 0; i < nivel; ++i) cout << "  "; 
    cout << "- " << p->nombre_completo() 
         << " (Edad: " << p->edad 
         << ", " << (p->genero == 'H' ? "H" : "M")
         << ", " << (p->esta_muerto ? "Muerto" : "Vivo") 
         << ", " << (p->es_rey ? "REY" : "") << ")\n";
    mostrar_arbol(p->primogenito, nivel + 1);
    mostrar_arbol(p->segundo, nivel + 1);
}

int main() {
    Reino reino;

    if (!reino.cargar_csv("bin/datos.csv")) {
        cerr << "Error al cargar el CSV.\n";
        return 1;
    }

    int opcion = 0;
    while (true) {
        cout << "\n--- MENU DEL REINO ---\n";
        cout << "1) Mostrar linea de sucesion actual\n";
        cout << "2) Marcar persona como muerta\n";
        cout << "3) Editar persona\n";
        cout << "4) Mostrar arbol genealogico completo\n";
        cout << "5) Guardar cambios en CSV\n";
        cout << "6) Salir\n";
        cout << "Ingrese una opcion: ";
        cin >> opcion;
        cin.ignore();

        if (opcion == 1) {
            reino.mostrar_sucesion();
        }
        else if (opcion == 2) {
            int id;
            cout << "Ingrese ID de la persona a marcar como muerta: ";
            cin >> id;
            cin.ignore();
            reino.marcar_muerto(id);
        }
        else if (opcion == 3) {
            int id, edad;
            char genero;
            string nombre, apellido;
            bool esta_muerto, fue_rey, es_rey;

            cout << "Ingrese ID de la persona a editar: ";
            cin >> id;
            cin.ignore();

            cout << "Nuevo nombre: ";
            getline(cin, nombre);
            cout << "Nuevo apellido: ";
            getline(cin, apellido);
            cout << "Genero (H/M): ";
            cin >> genero;
            cout << "Edad: ";
            cin >> edad;
            cout << "Esta muerto? 0=No 1=Si: ";
            cin >> esta_muerto;
            cout << "Fue rey? 0=No 1=Si: ";
            cin >> fue_rey;
            cout << "Es rey? 0=No 1=Si: ";
            cin >> es_rey;
            cin.ignore();
