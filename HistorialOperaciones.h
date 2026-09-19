#ifndef HISTORIALOPERACIONES_H
#define HISTORIALOPERACIONES_H

#include "RegistroOperacion.h"
#include "ArregloDinamico.h"
#include <string>
using namespace std;

//guarda el historial dinámico de operaciones
//el arreglo vive dentro de ArregloDinamico<RegistroOperacion>
class HistorialOperaciones {
private:
    ArregloDinamico<RegistroOperacion> registros;
    int siguienteId;

    const string NOMBRE_ARCHIVO_HISTORIAL = "historial.txt";
    const char DELIMITADOR = '|';

    //true si el registro le corresponde al usuario indicado
    //(o si verTodos está activo, por ser administrador)
    bool registroVisible(const RegistroOperacion& r, const string& usuario, bool verTodos) const;

public:
    HistorialOperaciones(int capacidadInicial = 5);
    ~HistorialOperaciones();

    void agregarRegistro(const string& usuario, const string& tipoOperacion, const string& archivoOrigen);
    void mostrarHistorial(const string& usuario, bool verTodos) const;

    //búsqueda lineal por tipo de operación; devuelve la posición o -1 si no existe
    int buscarPorTipo(const string& tipoOperacion, const string& usuario, bool verTodos) const;

    //ordenamiento burbuja por fecha/hora, igual estilo que ordenarPorID del proyecto anterior
    void ordenarPorFecha();
    void ordenarPorUsuario();

    //Nivel 4: reportes y estadísticas de uso
    void mostrarEstadisticas(const string& usuario, bool verTodos) const;
    bool guardarReporte(const string& ruta, const string& usuario, bool verTodos) const;

    void guardarHistorial() const;
    void cargarHistorial();
};

#endif