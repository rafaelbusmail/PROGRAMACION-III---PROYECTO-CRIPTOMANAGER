#ifndef GESTORARCHIVOS_H
#define GESTORARCHIVOS_H

#include <string>
using namespace std;


class GestorArchivos {
public:
    GestorArchivos();
    ~GestorArchivos();

    bool archivoExiste(const string& ruta) const;
    long obtenerTamanoArchivo(const string& ruta) const;

    string leerTexto(const string& ruta) const;
    bool guardarTexto(const string& ruta, const string& contenido) const;

    //lectura de un bloque binario a partir de una posición del archivo;
    char* leerBloqueBinario(const string& ruta, long posicion, int tamanoBloque, int& bytesLeidos) const;

    bool escribirBloqueBinario(const string& ruta, const char* datos, int tamano, bool nuevoArchivo) const;
};

#endif