#ifndef MENUACCESO_H
#define MENUACCESO_H

#include "GestorUsuarios.h"

//menú de acceso al sistema: iniciar sesión o registrarse como usuario nuevo
class MenuAcceso {
public:
    //devuelve true si se logró iniciar sesión, false si el usuario decide salir
    bool ejecutar(GestorUsuarios& usuarios);
};

#endif