#include "HistorialOperaciones.h"
#include "Utilidades.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>

HistorialOperaciones::HistorialOperaciones(int capacidadInicial)
    : registros(capacidadInicial) {
    siguienteId = 1;

    //al iniciar se carga lo que ya existía en archivo
    cargarHistorial();
}

HistorialOperaciones::~HistorialOperaciones() {
    //el destructor de ArregloDinamico se encarga del delete[] internamente
}

bool HistorialOperaciones::registroVisible(const RegistroOperacion& r, const string& usuario, bool verTodos) const {
    return verTodos || r.usuario == usuario;
}

void HistorialOperaciones::agregarRegistro(const string& usuario, const string& tipoOperacion, const string& archivoOrigen) {
    RegistroOperacion nuevo;
    nuevo.id = siguienteId;
    nuevo.usuario = usuario;
    nuevo.tipoOperacion = tipoOperacion;
    nuevo.archivoOrigen = archivoOrigen;
    nuevo.fechaHora = obtenerFechaHoraActual();

    registros.agregar(nuevo);
    siguienteId++;

    guardarHistorial();
}

void HistorialOperaciones::mostrarHistorial(const string& usuario, bool verTodos) const {
    //primero se cuenta cuántos registros son visibles para este usuario
    int visibles = 0;
    for (int i = 0; i < registros.getCantidad(); i++) {
        if (registroVisible(registros.obtener(i), usuario, verTodos)) {
            visibles++;
        }
    }

    if (visibles == 0) {
        cout << ">> El historial está vacío." << endl;
        return;
    }

    //se calcula el ancho de cada columna en base al contenido más largo que
    //realmente existe
    size_t anchoId = anchoVisible("ID");
    size_t anchoUsuario = anchoVisible("USUARIO");
    size_t anchoOperacion = anchoVisible("OPERACIÓN");
    size_t anchoArchivo = anchoVisible("ARCHIVO");
    size_t anchoFecha = anchoVisible("FECHA/HORA");

    for (int i = 0; i < registros.getCantidad(); i++) {
        const RegistroOperacion& r = registros.obtener(i);
        if (!registroVisible(r, usuario, verTodos)) continue;

        anchoId = max(anchoId, anchoVisible(to_string(r.id)));
        anchoUsuario = max(anchoUsuario, anchoVisible(r.usuario));
        anchoOperacion = max(anchoOperacion, anchoVisible(r.tipoOperacion));
        anchoArchivo = max(anchoArchivo, anchoVisible(r.archivoOrigen));
        anchoFecha = max(anchoFecha, anchoVisible(r.fechaHora));
    }

    //espacio extra entre columnas para que no queden pegadas
    anchoId += 2;
    anchoUsuario += 2;
    anchoOperacion += 2;
    anchoArchivo += 2;
    anchoFecha += 2;

    cout << "\n"
         << rellenar("ID", anchoId)
         << rellenar("USUARIO", anchoUsuario)
         << rellenar("OPERACIÓN", anchoOperacion)
         << rellenar("ARCHIVO", anchoArchivo)
         << "FECHA/HORA" << endl;

    size_t anchoTotal = anchoId + anchoUsuario + anchoOperacion + anchoArchivo + anchoFecha;
    cout << string(anchoTotal, '-') << endl;

    for (int i = 0; i < registros.getCantidad(); i++) {
        const RegistroOperacion& r = registros.obtener(i);
        if (!registroVisible(r, usuario, verTodos)) continue;

        cout << rellenar(to_string(r.id), anchoId)
             << rellenar(r.usuario, anchoUsuario)
             << rellenar(r.tipoOperacion, anchoOperacion)
             << rellenar(r.archivoOrigen, anchoArchivo)
             << r.fechaHora << endl;
    }
    cout << string(anchoTotal, '-') << endl;
}

int HistorialOperaciones::buscarPorTipo(const string& tipoOperacion, const string& usuario, bool verTodos) const {
    for (int i = 0; i < registros.getCantidad(); i++) {
        const RegistroOperacion& r = registros.obtener(i);
        if (!registroVisible(r, usuario, verTodos)) continue;

        if (r.tipoOperacion == tipoOperacion) {
            return i;
        }
    }
    return -1;
}

void HistorialOperaciones::ordenarPorFecha() {
    int n = registros.getCantidad();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (registros.obtener(j).fechaHora > registros.obtener(j + 1).fechaHora) {
                RegistroOperacion temp = registros.obtener(j);
                registros.obtener(j) = registros.obtener(j + 1);
                registros.obtener(j + 1) = temp;
            }
        }
    }
    guardarHistorial();
}

void HistorialOperaciones::ordenarPorUsuario() {
    //mismo burbuja, pero comparando el nombre del usuario; si dos registros
    //son del mismo usuario, se desempata por fecha/hora
    int n = registros.getCantidad();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            const RegistroOperacion& a = registros.obtener(j);
            const RegistroOperacion& b = registros.obtener(j + 1);

            bool debeIntercambiar = false;
            if (a.usuario > b.usuario) {
                debeIntercambiar = true;
            } else if (a.usuario == b.usuario && a.fechaHora > b.fechaHora) {
                debeIntercambiar = true;
            }

            if (debeIntercambiar) {
                RegistroOperacion temp = registros.obtener(j);
                registros.obtener(j) = registros.obtener(j + 1);
                registros.obtener(j + 1) = temp;
            }
        }
    }
    guardarHistorial();
}

void HistorialOperaciones::mostrarEstadisticas(const string& usuario, bool verTodos) const {
    int total = 0;
    int cesar = 0, vigenere = 0, xorCount = 0;
    int encriptados = 0, desencriptados = 0;
    int desdeTexto = 0, desdeArchivo = 0;

    for (int i = 0; i < registros.getCantidad(); i++) {
        const RegistroOperacion& r = registros.obtener(i);
        if (!registroVisible(r, usuario, verTodos)) continue;

        total++;

        //se clasifica según lo que contenga el nombre de la operación
        if (r.tipoOperacion.find("Cesar") != string::npos) cesar++;
        if (r.tipoOperacion.find("Vigenere") != string::npos) vigenere++;
        if (r.tipoOperacion.find("XOR") != string::npos) xorCount++;

        if (r.tipoOperacion.find("Desencriptar") != string::npos) {
            desencriptados++;
        } else if (r.tipoOperacion.find("Encriptar") != string::npos ||
                   r.tipoOperacion.find("Procesar") != string::npos) {
            encriptados++;
        }

        if (r.tipoOperacion.find("Archivo") != string::npos) {
            desdeArchivo++;
        } else {
            desdeTexto++;
        }
    }

    if (total == 0) {
        cout << ">> No hay operaciones registradas para generar estadísticas." << endl;
        return;
    }

    cout << "\n--- ESTADÍSTICAS DE USO ---" << endl;
    if (verTodos) {
        cout << "Alcance: TODOS los usuarios del sistema" << endl;
    } else {
        cout << "Alcance: usuario " << usuario << endl;
    }
    cout << "-----------------------------------" << endl;
    cout << rellenar("Total de operaciones", 28) << ": " << total << endl;
    cout << rellenar("  Encriptados", 28) << ": " << encriptados << endl;
    cout << rellenar("  Desencriptados", 28) << ": " << desencriptados << endl;
    cout << rellenar("Uso de César", 28) << ": " << cesar << endl;
    cout << rellenar("Uso de Vigenère", 28) << ": " << vigenere << endl;
    cout << rellenar("Uso de XOR", 28) << ": " << xorCount << endl;
    cout << rellenar("Desde texto directo", 28) << ": " << desdeTexto << endl;
    cout << rellenar("Desde archivo", 28) << ": " << desdeArchivo << endl;
    cout << "-----------------------------------" << endl;
}

bool HistorialOperaciones::guardarReporte(const string& ruta, const string& usuario, bool verTodos) const {
    ofstream archivo(ruta.c_str(), ios::trunc);
    if (!archivo.is_open()) {
        return false;
    }

    int total = 0;
    int cesar = 0, vigenere = 0, xorCount = 0;
    int encriptados = 0, desencriptados = 0;
    int desdeTexto = 0, desdeArchivo = 0;

    for (int i = 0; i < registros.getCantidad(); i++) {
        const RegistroOperacion& r = registros.obtener(i);
        if (!registroVisible(r, usuario, verTodos)) continue;

        total++;
        if (r.tipoOperacion.find("Cesar") != string::npos) cesar++;
        if (r.tipoOperacion.find("Vigenere") != string::npos) vigenere++;
        if (r.tipoOperacion.find("XOR") != string::npos) xorCount++;

        if (r.tipoOperacion.find("Desencriptar") != string::npos) {
            desencriptados++;
        } else if (r.tipoOperacion.find("Encriptar") != string::npos ||
                   r.tipoOperacion.find("Procesar") != string::npos) {
            encriptados++;
        }

        if (r.tipoOperacion.find("Archivo") != string::npos) {
            desdeArchivo++;
        } else {
            desdeTexto++;
        }
    }

    archivo << "=========================================\n";
    archivo << "   CRIPTO-MANAGER - REPORTE DE USO\n";
    archivo << "=========================================\n";
    archivo << "Generado por : " << usuario << "\n";
    archivo << "Fecha        : " << obtenerFechaHoraActual() << "\n";
    archivo << "Alcance      : " << (verTodos ? "Todos los usuarios" : "Solo este usuario") << "\n";
    archivo << "-----------------------------------------\n";
    archivo << "RESUMEN\n";
    archivo << "  Total de operaciones : " << total << "\n";
    archivo << "  Encriptados          : " << encriptados << "\n";
    archivo << "  Desencriptados       : " << desencriptados << "\n";
    archivo << "  Uso de Cesar         : " << cesar << "\n";
    archivo << "  Uso de Vigenere      : " << vigenere << "\n";
    archivo << "  Uso de XOR           : " << xorCount << "\n";
    archivo << "  Desde texto directo  : " << desdeTexto << "\n";
    archivo << "  Desde archivo        : " << desdeArchivo << "\n";
    archivo << "-----------------------------------------\n";
    archivo << "DETALLE DE OPERACIONES\n";

    for (int i = 0; i < registros.getCantidad(); i++) {
        const RegistroOperacion& r = registros.obtener(i);
        if (!registroVisible(r, usuario, verTodos)) continue;

        archivo << "  [" << r.id << "] " << r.fechaHora
                << " | " << r.usuario
                << " | " << r.tipoOperacion
                << " | " << r.archivoOrigen << "\n";
    }

    archivo << "=========================================\n";
    archivo.close();
    return true;
}

void HistorialOperaciones::guardarHistorial() const {
    ofstream archivo(NOMBRE_ARCHIVO_HISTORIAL.c_str(), ios::trunc);
    if (!archivo.is_open()) {
        return;
    }

    for (int i = 0; i < registros.getCantidad(); i++) {
        const RegistroOperacion& r = registros.obtener(i);
        archivo << r.id << DELIMITADOR
                << r.usuario << DELIMITADOR
                << r.tipoOperacion << DELIMITADOR
                << r.archivoOrigen << DELIMITADOR
                << r.fechaHora << "\n";
    }
    archivo.close();
}

void HistorialOperaciones::cargarHistorial() {
    ifstream archivo(NOMBRE_ARCHIVO_HISTORIAL.c_str());
    if (!archivo.is_open()) {
        return;
    }

    string linea;
    while (getline(archivo, linea)) {
        if (linea.empty()) continue;

        size_t p1 = linea.find(DELIMITADOR);
        size_t p2 = (p1 == string::npos) ? string::npos : linea.find(DELIMITADOR, p1 + 1);
        size_t p3 = (p2 == string::npos) ? string::npos : linea.find(DELIMITADOR, p2 + 1);
        size_t p4 = (p3 == string::npos) ? string::npos : linea.find(DELIMITADOR, p3 + 1);

        if (p1 == string::npos || p2 == string::npos || p3 == string::npos) continue;

        try {
            RegistroOperacion r;
            r.id = stoi(linea.substr(0, p1));

            if (p4 == string::npos) {
                //formato viejo (sin columna de usuario): se conserva el registro
                //pero se marca como heredado para no perder el historial anterior
                r.usuario = "(sin usuario)";
                r.tipoOperacion = linea.substr(p1 + 1, p2 - (p1 + 1));
                r.archivoOrigen = linea.substr(p2 + 1, p3 - (p2 + 1));
                r.fechaHora = linea.substr(p3 + 1);
            } else {
                r.usuario = linea.substr(p1 + 1, p2 - (p1 + 1));
                r.tipoOperacion = linea.substr(p2 + 1, p3 - (p2 + 1));
                r.archivoOrigen = linea.substr(p3 + 1, p4 - (p3 + 1));
                r.fechaHora = linea.substr(p4 + 1);
            }

            registros.agregar(r);

            //el siguiente id que se genere debe continuar después del más alto que ya existía
            if (r.id >= siguienteId) {
                siguienteId = r.id + 1;
            }
        } catch (...) {
            continue;
        }
    }
    archivo.close();
}