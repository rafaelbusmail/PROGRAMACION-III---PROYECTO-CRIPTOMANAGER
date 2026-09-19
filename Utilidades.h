#ifndef UTILIDADES_H
#define UTILIDADES_H

#include <string>
using namespace std;

//configura la consola para mostrar correctamente tildes y caracteres UTF-8
void configurarConsola();

int validarEntero(string mensaje);
int validarEnteroRango(string mensaje, int minimo, int maximo);

//lee una línea completa y no acepta que quede vacía
string leerTextoNoVacio(string mensaje);

//devuelve la fecha y hora actual
string obtenerFechaHoraActual();

//cuenta los caracteres VISIBLES de un texto, no los bytes: en UTF-8 una letra
size_t anchoVisible(const string& texto);
string rellenar(const string& texto, size_t ancho);

//muestra en consola el resultado de una operación; si el contenido es muy
//largo solo imprime el inicio para no llenar la pantalla
void mostrarResultadoEnConsola(const string& etiqueta, const string& contenido);

#endif