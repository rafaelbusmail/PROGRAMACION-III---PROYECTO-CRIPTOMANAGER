#include "CifradorXOR.h"

CifradorXOR::CifradorXOR(const string& claveInicial) {
    clave = claveInicial;
}

CifradorXOR::~CifradorXOR() {
    //el string se libera automáticamente
}

void CifradorXOR::setClave(const string& claveNueva) {
    clave = claveNueva;
}

string CifradorXOR::getClave() const {
    return clave;
}

char* CifradorXOR::procesarBloque(const char* datosEntrada, int longitud, long posicionInicial) const {
    //el bloque resultante se reserva aquí y el que llama debe hacer delete[]
    char* resultado = new char[longitud];

    for (int i = 0; i < longitud; i++) {
        char letraClave = clave[(posicionInicial + i) % clave.size()];
        resultado[i] = datosEntrada[i] ^ letraClave;
    }

    return resultado;
}