#include "Utilidades.h"
#include <iostream>
#include <limits>
#include <ctime>
#include <sstream>
#include <iomanip>

using namespace std;

int validarEntero(string mensaje) {
    int n;
    cout << mensaje;
    while (!(cin >> n)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << ">> ERROR: Ingrese un número entero válido." << endl;
        cout << mensaje;
    }

    //se descarta lo que quede de la línea (el Enter y cualquier basura),
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return n;
}

int validarEnteroRango(string mensaje, int minimo, int maximo) {
    int n = validarEntero(mensaje);
    while (n < minimo || n > maximo) {
        cout << ">> ERROR: Ingrese un valor entre " << minimo << " y " << maximo << "." << endl;
        n = validarEntero(mensaje);
    }
    return n;
}

string leerTextoNoVacio(string mensaje) {
    string texto;
    do {
        cout << mensaje;
        getline(cin, texto);

        if (texto.empty()) {
            cout << ">> ERROR: Este campo no puede quedar vacío." << endl;
        }
    } while (texto.empty());

    return texto;
}

string obtenerFechaHoraActual() {
    time_t ahora = time(nullptr);
    tm* tiempoLocal = localtime(&ahora);

    ostringstream oss;
    oss << put_time(tiempoLocal, "%d/%m/%Y %H:%M:%S");
    return oss.str();
}

size_t anchoVisible(const string& texto) {
    size_t contador = 0;

    for (size_t i = 0; i < texto.size(); i++) {
        //en UTF-8 los bytes de continuación siempre empiezan con los bits 10xxxxxx;
        //esos no cuentan como carácter nuevo, son parte del carácter anterior
        if ((texto[i] & 0xC0) != 0x80) {
            contador++;
        }
    }

    return contador;
}

string rellenar(const string& texto, size_t ancho) {
    size_t visible = anchoVisible(texto);

    if (visible >= ancho) {
        return texto;
    }

    return texto + string(ancho - visible, ' ');
}

void mostrarResultadoEnConsola(const string& etiqueta, const string& contenido) {
    const size_t LIMITE = 600; //cuántos caracteres se muestran como máximo

    cout << "\n--- " << etiqueta << " ---" << endl;

    if (contenido.size() <= LIMITE) {
        cout << contenido << endl;
    } else {
        cout << contenido.substr(0, LIMITE) << endl;
        cout << "[...] (se muestran los primeros " << LIMITE
             << " de " << contenido.size() << " caracteres)" << endl;
    }

    cout << "----------------------------------------" << endl;
}