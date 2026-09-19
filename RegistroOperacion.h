#ifndef REGISTROOPERACION_H
#define REGISTROOPERACION_H

#include <string>
using namespace std;

struct RegistroOperacion {
    int id;
    string usuario;
    string tipoOperacion;
    string archivoOrigen;
    string fechaHora;
};

#endif