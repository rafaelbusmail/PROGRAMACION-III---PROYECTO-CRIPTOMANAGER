//RAFAEL BUSMAIL - 22541446
//Proyecto Final - Cripto-Manager

#include <iostream>
#include "Utilidades.h"
#include "GestorUsuarios.h"
#include "MenuAcceso.h"
#include "MenuPrincipal.h"

using namespace std;

int main() {
    configurarConsola();

    GestorUsuarios usuarios; //carga usuarios.txt si ya existe

    cout << "===        CRIPTO-MANAGER: SISTEMA DE CIFRADO        ===" << endl;
    cout << "\nNivel 1: Cesar | Nivel 2: XOR | Nivel 3: Vigenere" << endl;
    cout << "----------------------------------------------------------" << endl;

    //no se entra al programa sin iniciar sesión
    MenuAcceso acceso;
    if (!acceso.ejecutar(usuarios)) {
        cout << ">> Saliendo del sistema. ¡Hasta luego!" << endl;
        return 0;
    }

    MenuPrincipal menu(usuarios);
    menu.ejecutar();

    return 0;
}