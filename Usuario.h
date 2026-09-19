#ifndef USUARIO_H
#define USUARIO_H

#include <string>
using namespace std;

//estructura simple para cada usuario registrado del sistema.
//la clave. se guarda su hash
struct Usuario {
    string nombre;
    string claveHash;
    string rol; //"ADMIN" o "USUARIO"
};

#endif