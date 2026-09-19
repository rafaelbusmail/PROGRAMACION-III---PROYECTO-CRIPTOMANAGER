#ifndef MENUPRINCIPAL_H
#define MENUPRINCIPAL_H

#include "GestorUsuarios.h"
#include "CifradorCesar.h"
#include "CifradorVigenere.h"
#include "CifradorXOR.h"
#include "GestorArchivos.h"
#include "Selectorarchivos.h"
#include "ConversorBinario.h"
#include "Hash.h"
#include "HistorialOperaciones.h"
#include <cstddef>

//menú principal del sistema: cifrados, historial, reportes y administración.
class MenuPrincipal {
private:
    GestorUsuarios& usuarios;
    CifradorCesar cesar;
    CifradorVigenere vigenere;
    GestorArchivos gestor;
    SelectorArchivos selector;
    ConversorBinario conversor;
    Hash hasher;
    HistorialOperaciones historial;

    //ancho de cada una de las 3 columnas del menú
    static constexpr size_t ANCHO_COLUMNA = 26;
    static constexpr size_t ANCHO_MENU = ANCHO_COLUMNA * 3;

    void mostrarMenu() const;
    void imprimirSeparador(const string& titulo) const;
    void imprimirFila(const string& col1, const string& col2, const string& col3) const;

    void opcionCesarTexto();
    void opcionCesarArchivo();
    void opcionVigenereTexto();
    void opcionVigenereArchivo();
    void opcionXORTexto();
    void opcionXORArchivo();
    void opcionBuscarHistorial();
    void opcionReporte();
    void opcionCalcularHash();
    void opcionRegistrarUsuario();

public:
    MenuPrincipal(GestorUsuarios& usuarios);
    void ejecutar();
};

#endif