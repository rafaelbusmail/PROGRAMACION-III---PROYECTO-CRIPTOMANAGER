#include "GestorUsuarios.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include "Utilidades.h"
#include <algorithm>

GestorUsuarios::GestorUsuarios(int capacidadInicial)
    : usuarios(capacidadInicial) {
    sesionActiva = -1;

    //al iniciar se carga lo que ya existía en archivo
    cargarUsuarios();


    if (usuarios.getCantidad() == 0) {
        registrar("admin", "admin123!", "ADMIN");
        cout << ">> No había usuarios registrados." << endl;
        cout << ">> Se creó el usuario administrador por defecto: admin / admin123!" << endl;
    }
}

GestorUsuarios::~GestorUsuarios() {
    //el destructor de ArregloDinamico se encarga del delete[] internamente
}

int GestorUsuarios::buscarUsuario(const string& nombre) const {
    for (int i = 0; i < usuarios.getCantidad(); i++) {
        if (usuarios.obtener(i).nombre == nombre) {
            return i;
        }
    }
    return -1;
}

bool GestorUsuarios::claveValida(const string& clave) const {
    if (clave.size() < 5) {
        return false;
    }

    //se comparan caracteres ASCII a propósito
    bool tieneLetra = false;
    bool tieneNumero = false;
    bool tieneEspecial = false;

    for (size_t i = 0; i < clave.size(); i++) {
        unsigned char c = static_cast<unsigned char>(clave[i]);
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
            tieneLetra = true;
        } else if (c >= '0' && c <= '9') {
            tieneNumero = true;
        } else {
            tieneEspecial = true;
        }
    }

    return tieneLetra && tieneNumero && tieneEspecial;
}

ResultadoRegistro GestorUsuarios::registrar(const string& nombre, const string& clave, const string& rol) {
    if (nombre.size() < 3) {
        return ResultadoRegistro::NOMBRE_CORTO;
    }

    //el '|' es el delimitador del archivo usuarios.txt: si se guardara en el
    //nombre, el archivo quedaría corrupto al volver a cargarlo
    if (nombre.find(DELIMITADOR) != string::npos) {
        return ResultadoRegistro::NOMBRE_INVALIDO;
    }

    if (!claveValida(clave)) {
        return ResultadoRegistro::CLAVE_DEBIL;
    }

    if (buscarUsuario(nombre) != -1) {
        return ResultadoRegistro::NOMBRE_OCUPADO;
    }

    Usuario nuevo;
    nuevo.nombre = nombre;
    nuevo.claveHash = hasher.calcularHashTexto(clave);
    nuevo.rol = rol;

    usuarios.agregar(nuevo);
    guardarUsuarios();
    return ResultadoRegistro::OK;
}

bool GestorUsuarios::iniciarSesion(const string& nombre, const string& clave) {
    int posicion = buscarUsuario(nombre);
    if (posicion == -1) {
        return false;
    }

    //nunca se compara la contraseña directamente: se hashea la que el usuario
    //escribió y se compara ese hash contra el que está guardado
    string hashIngresado = hasher.calcularHashTexto(clave);

    if (usuarios.obtener(posicion).claveHash == hashIngresado) {
        sesionActiva = posicion;
        return true;
    }

    return false;
}

void GestorUsuarios::cerrarSesion() {
    sesionActiva = -1;
}

bool GestorUsuarios::haySesionActiva() const {
    return sesionActiva != -1;
}

string GestorUsuarios::getUsuarioActual() const {
    if (sesionActiva == -1) {
        return "";
    }
    return usuarios.obtener(sesionActiva).nombre;
}

string GestorUsuarios::getRolActual() const {
    if (sesionActiva == -1) {
        return "";
    }
    return usuarios.obtener(sesionActiva).rol;
}

bool GestorUsuarios::esAdministrador() const {
    return getRolActual() == "ADMIN";
}

int GestorUsuarios::getCantidadUsuarios() const {
    return usuarios.getCantidad();
}

void GestorUsuarios::mostrarUsuarios() const {
    if (usuarios.getCantidad() == 0) {
        cout << ">> No hay usuarios registrados." << endl;
        return;
    }

    //mismo criterio de anchos dinámicos que el historial
    size_t anchoNombre = anchoVisible("USUARIO");
    size_t anchoRol = anchoVisible("ROL");

    for (int i = 0; i < usuarios.getCantidad(); i++) {
        anchoNombre = max(anchoNombre, anchoVisible(usuarios.obtener(i).nombre));
        anchoRol = max(anchoRol, anchoVisible(usuarios.obtener(i).rol));
    }
    anchoNombre += 2;
    anchoRol += 2;

    cout << "\n"
         << rellenar("USUARIO", anchoNombre)
         << rellenar("ROL", anchoRol)
         << "CLAVE (HASH)" << endl;

    size_t anchoTotal = anchoNombre + anchoRol + string("CLAVE (HASH)").size();
    cout << string(anchoTotal, '-') << endl;

    for (int i = 0; i < usuarios.getCantidad(); i++) {
        const Usuario& u = usuarios.obtener(i);
        cout << rellenar(u.nombre, anchoNombre)
             << rellenar(u.rol, anchoRol)
             << u.claveHash << endl;
    }
    cout << string(anchoTotal, '-') << endl;
}

void GestorUsuarios::guardarUsuarios() const {
    ofstream archivo(NOMBRE_ARCHIVO_USUARIOS.c_str(), ios::trunc);
    if (!archivo.is_open()) {
        return;
    }

    for (int i = 0; i < usuarios.getCantidad(); i++) {
        const Usuario& u = usuarios.obtener(i);
        archivo << u.nombre << DELIMITADOR
                << u.claveHash << DELIMITADOR
                << u.rol << "\n";
    }
    archivo.close();
}

void GestorUsuarios::cargarUsuarios() {
    ifstream archivo(NOMBRE_ARCHIVO_USUARIOS.c_str());
    if (!archivo.is_open()) {
        return;
    }

    string linea;
    while (getline(archivo, linea)) {
        if (linea.empty()) continue;

        size_t p1 = linea.find(DELIMITADOR);
        size_t p2 = (p1 == string::npos) ? string::npos : linea.find(DELIMITADOR, p1 + 1);

        if (p1 == string::npos || p2 == string::npos) continue;

        Usuario u;
        u.nombre = linea.substr(0, p1);
        u.claveHash = linea.substr(p1 + 1, p2 - (p1 + 1));
        u.rol = linea.substr(p2 + 1);

        usuarios.agregar(u);
    }
    archivo.close();
}