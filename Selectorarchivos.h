#ifndef SELECTORARCHIVOS_H
#define SELECTORARCHIVOS_H

#include <string>
using namespace std;

class SelectorArchivos {
public:
    SelectorArchivos();
    ~SelectorArchivos();

    //abre el diálogo "Abrir archivo"; devuelve "" si el usuario cancela
    //debe ir en el formato de Windows: "Descripción\0*.ext\0"
    string seleccionarArchivoAbrir(const char* filtro = "Archivos de texto (*.txt)\0*.txt\0Todos los archivos (*.*)\0*.*\0") const;

    //abre el diálogo "Guardar como"; devuelve "" si el usuario cancela
    string seleccionarArchivoGuardar(const char* filtro = "Archivos de texto (*.txt)\0*.txt\0Todos los archivos (*.*)\0*.*\0",
                                     const char* extensionDefault = "txt") const;
};

#endif