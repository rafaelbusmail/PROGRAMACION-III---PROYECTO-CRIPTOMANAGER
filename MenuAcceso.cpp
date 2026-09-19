#include "MenuAcceso.h"
#include "Utilidades.h"
#include <iostream>

using namespace std;

bool MenuAcceso::ejecutar(GestorUsuarios& usuarios) {
    //devuelve true si se logró iniciar sesión, false si el usuario decide salir
    while (!usuarios.haySesionActiva()) {
        cout << "\n            ACCESO AL SISTEMA            " << endl;
        cout << "=====================================" << endl;
        cout << "1. Iniciar sesión" << endl;
        cout << "2. Registrarse como usuario nuevo" << endl;
        cout << "0. Salir del programa" << endl;
        cout << "=====================================" << endl;

        int opcion = validarEnteroRango("Seleccione una opción: ", 0, 2);

        if (opcion == 0) {
            return false;
        }

        string nombre = leerTextoNoVacio("Usuario: ");
        string clave = leerTextoNoVacio("Contraseña: ");

        if (opcion == 1) {
            if (usuarios.iniciarSesion(nombre, clave)) {
                cout << ">> ¡Bienvenido, " << usuarios.getUsuarioActual()
                     << "! (rol: " << usuarios.getRolActual() << ")" << endl;
            } else {
                cout << ">> ERROR: Usuario o contraseña incorrectos." << endl;
            }
        } else {
            //los usuarios que se registran solos siempre entran con rol USUARIO;
            //solo un ADMIN puede crear otro ADMIN desde el menú principal
            ResultadoRegistro resultado = usuarios.registrar(nombre, clave, "USUARIO");
            if (resultado == ResultadoRegistro::OK) {
                //el usuario recién registrado entra directo al sistema,
                //sin tener que volver a elegir "iniciar sesión"
                usuarios.iniciarSesion(nombre, clave);
                cout << ">> ¡Bienvenido, " << usuarios.getUsuarioActual()
                     << "! (rol: " << usuarios.getRolActual() << ")" << endl;
            } else {
                switch (resultado) {
                case ResultadoRegistro::NOMBRE_CORTO:
                    cout << ">> ERROR: El nombre de usuario debe tener al menos 3 caracteres." << endl;
                    break;
                case ResultadoRegistro::NOMBRE_INVALIDO:
                    cout << ">> ERROR: El nombre de usuario no puede contener el carácter '|'." << endl;
                    break;
                case ResultadoRegistro::CLAVE_DEBIL:
                    cout << ">> ERROR: La contraseña debe tener al menos 5 caracteres e incluir una letra, un número y un carácter especial." << endl;
                    break;
                case ResultadoRegistro::NOMBRE_OCUPADO:
                    cout << ">> ERROR: Ese nombre de usuario ya está ocupado." << endl;
                    break;
                default:
                    break;
                }
            }
        }
    }

    return true;
}