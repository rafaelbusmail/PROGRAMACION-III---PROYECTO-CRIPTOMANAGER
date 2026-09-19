#ifndef GESTORUSUARIOS_H
#define GESTORUSUARIOS_H

#include "Usuario.h"
#include "ArregloDinamico.h"
#include "Hash.h"
#include <string>
using namespace std;

//usa el mismo arreglo dinámico genérico que el historial y la clase Hash
class GestorUsuarios {
private:
    ArregloDinamico<Usuario> usuarios;
    Hash hasher;
    int sesionActiva; //índice del usuario logueado, o -1 si no hay sesión

    const string NOMBRE_ARCHIVO_USUARIOS = "usuarios.txt";
    const char DELIMITADOR = '|';

public:
    GestorUsuarios(int capacidadInicial = 5);
    ~GestorUsuarios();

    //búsqueda lineal por nombre; devuelve la posición o -1 si no existe
    int buscarUsuario(const string& nombre) const;

    //devuelve false si el nombre ya está ocupado
    bool registrar(const string& nombre, const string& clave, const string& rol);

    //compara el hash de la clave ingresada contra el almacenado
    bool iniciarSesion(const string& nombre, const string& clave);
    void cerrarSesion();

    bool haySesionActiva() const;
    string getUsuarioActual() const;
    string getRolActual() const;
    bool esAdministrador() const;

    int getCantidadUsuarios() const;
    void mostrarUsuarios() const; //solo tiene sentido para el rol ADMIN

    void guardarUsuarios() const;
    void cargarUsuarios();
};

#endif