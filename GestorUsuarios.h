#ifndef GESTORUSUARIOS_H
#define GESTORUSUARIOS_H

#include "Usuario.h"
#include "Arreglodinamico.h"
#include "Hash.h"
#include <string>
using namespace std;

//resultado de intentar registrar un usuario; cada valor indica el motivo
//por el que el registro fue rechazado (o que fue aceptado)
enum class ResultadoRegistro {
    OK,
    NOMBRE_CORTO,
    NOMBRE_INVALIDO,
    CLAVE_DEBIL,
    NOMBRE_OCUPADO
};

//usa el mismo arreglo dinámico genérico que el historial y la clase Hash
class GestorUsuarios {
private:
    ArregloDinamico<Usuario> usuarios;
    Hash hasher;
    int sesionActiva; //índice del usuario logueado, o -1 si no hay sesión

    const string NOMBRE_ARCHIVO_USUARIOS = "usuarios.txt";
    const char DELIMITADOR = '|';

    //la contraseña debe tener al menos 5 caracteres e incluir una letra,
    //un número y un carácter especial
    bool claveValida(const string& clave) const;

public:
    GestorUsuarios(int capacidadInicial = 5);
    ~GestorUsuarios();

    //búsqueda lineal por nombre; devuelve la posición o -1 si no existe
    int buscarUsuario(const string& nombre) const;

    //valida nombre y contraseña y registra al usuario; devuelve el motivo
    //del rechazo (o OK si se registró correctamente)
    ResultadoRegistro registrar(const string& nombre, const string& clave, const string& rol);

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