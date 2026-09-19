#ifdef _WIN32
#include <windows.h>
#include <commdlg.h>
#endif

#include "Selectorarchivos.h"
#include <iostream>

#ifdef _WIN32

SelectorArchivos::SelectorArchivos() {
    //constructor vacío, la clase no necesita atributos propios
}

SelectorArchivos::~SelectorArchivos() {
    //destructor vacío
}

string SelectorArchivos::seleccionarArchivoAbrir(const char* filtro) const {
    char rutaSeleccionada[MAX_PATH] = "";

    OPENFILENAMEA ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = GetConsoleWindow(); //la ventana "dueña" del diálogo es la consola
    ofn.lpstrFilter = filtro;
    ofn.lpstrFile = rutaSeleccionada;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrTitle = "Selecciona el archivo a procesar";
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetOpenFileNameA(&ofn)) {
        return string(rutaSeleccionada);
    }

    return ""; //el usuario cerró o canceló el diálogo
}

string SelectorArchivos::seleccionarArchivoGuardar(const char* filtro, const char* extensionDefault) const {
    char rutaSeleccionada[MAX_PATH] = "";

    OPENFILENAMEA ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = GetConsoleWindow();
    ofn.lpstrFilter = filtro;
    ofn.lpstrFile = rutaSeleccionada;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrTitle = "Elige dónde guardar el resultado";
    ofn.lpstrDefExt = (extensionDefault != nullptr && extensionDefault[0] != '\0') ? extensionDefault : nullptr;
    ofn.Flags = OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

    if (GetSaveFileNameA(&ofn)) {
        return string(rutaSeleccionada);
    }

    return "";
}

#else
SelectorArchivos::SelectorArchivos() {
    //constructor vacío, la clase no necesita atributos propios
}

SelectorArchivos::~SelectorArchivos() {
    //destructor vacío
}

string SelectorArchivos::seleccionarArchivoAbrir(const char*) const {
    //el parámetro filtro es específico del diálogo de Windows; aquí se ignora
    cout << ">> Escriba la ruta del archivo a abrir (Enter para cancelar): ";
    string ruta;
    getline(cin, ruta);
    return ruta;
}

string SelectorArchivos::seleccionarArchivoGuardar(const char*, const char*) const {
    //los parámetros son específicos del diálogo de Windows; aquí se ignoran
    cout << ">> Escriba la ruta donde guardar el archivo (Enter para cancelar): ";
    string ruta;
    getline(cin, ruta);
    return ruta;
}

#endif