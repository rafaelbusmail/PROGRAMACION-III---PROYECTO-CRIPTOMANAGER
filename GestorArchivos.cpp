#include "GestorArchivos.h"
#include <fstream>

GestorArchivos::GestorArchivos() {
}

GestorArchivos::~GestorArchivos() {
    //cada operación abre y cierra su propio stream
}

bool GestorArchivos::archivoExiste(const string& ruta) const {
    ifstream archivo(ruta.c_str());
    return archivo.good();
}

long GestorArchivos::obtenerTamanoArchivo(const string& ruta) const {
    ifstream archivo(ruta.c_str(), ios::binary | ios::ate);
    if (!archivo.is_open()) {
        return -1;
    }
    long tamano = archivo.tellg();
    archivo.close();
    return tamano;
}

string GestorArchivos::leerTexto(const string& ruta) const {
    ifstream archivo(ruta.c_str());
    string contenido = "";

    if (!archivo.is_open()) {
        return contenido;
    }

    string linea;
    bool primera = true;
    while (getline(archivo, linea)) {
        if (!primera) {
            contenido += "\n";
        }
        contenido += linea;
        primera = false;
    }
    archivo.close();
    return contenido;
}

bool GestorArchivos::guardarTexto(const string& ruta, const string& contenido) const {
    ofstream archivo(ruta.c_str(), ios::trunc);
    if (!archivo.is_open()) {
        return false;
    }
    archivo << contenido;
    archivo.close();
    return true;
}

char* GestorArchivos::leerBloqueBinario(const string& ruta, long posicion, int tamanoBloque, int& bytesLeidos) const {
    ifstream archivo(ruta.c_str(), ios::binary);
    bytesLeidos = 0;

    if (!archivo.is_open()) {
        return nullptr;
    }

    char* buffer = new char[tamanoBloque];
    archivo.seekg(posicion);
    archivo.read(buffer, tamanoBloque);
    bytesLeidos = archivo.gcount();
    archivo.close();

    return buffer; //el que llama es responsable de hacer delete[]
}

bool GestorArchivos::escribirBloqueBinario(const string& ruta, const char* datos, int tamano, bool nuevoArchivo) const {
    ios_base::openmode modo = ios::binary;
    modo |= nuevoArchivo ? ios::trunc : ios::app;

    ofstream archivo(ruta.c_str(), modo);
    if (!archivo.is_open()) {
        return false;
    }

    archivo.write(datos, tamano);
    archivo.close();
    return true;
}