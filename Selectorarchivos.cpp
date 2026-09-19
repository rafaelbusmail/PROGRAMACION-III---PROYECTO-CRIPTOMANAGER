//En Windows se usa el diálogo nativo del explorador de archivos (Win32 API).
//Este mismo .exe compilado para Windows funciona en Linux a través de Wine,
//porque Wine implementa estas funciones.
//
//NOTA: windows.h debe incluirse ANTES que cualquier header que traiga
//<cstddef>/std::byte (como <string>); si no, el typedef global "byte" de
//rpcndr.h choca con std::byte de C++17 y la compilación falla.
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
//En Linux/macOS (compilación nativa, sin Wine) no existe el diálogo Win32,
//así que se pide la ruta por consola. El usuario puede escribir la ruta o
//arrastrar el archivo a la terminal.
SelectorArchivos::SelectorArchivos() {
}

SelectorArchivos::~SelectorArchivos() {
}

string SelectorArchivos::seleccionarArchivoAbrir(const char* filtro) const {
    cout << ">> (modo consola) Escriba la ruta del archivo a abrir: ";
    string ruta;
    getline(cin, ruta);
    return ruta;
}

string SelectorArchivos::seleccionarArchivoGuardar(const char* filtro, const char* extensionDefault) const {
    cout << ">> (modo consola) Escriba la ruta donde guardar el archivo: ";
    string ruta;
    getline(cin, ruta);
    return ruta;
}
#endif