#include "CifradorCesar.h"

CifradorCesar::CifradorCesar(int desplazamientoInicial) {
    desplazamiento = desplazamientoInicial;
}

CifradorCesar::~CifradorCesar() {
    //destructor vacío
}

void CifradorCesar::setDesplazamiento(int d) {
    desplazamiento = d;
}

int CifradorCesar::getDesplazamiento() const {
    return desplazamiento;
}

char CifradorCesar::desplazarCaracter(char c, int desplazamiento) const {
    //se normaliza el desplazamiento por si el usuario ingresa negativos o valores mayores a 26
    int d = desplazamiento % 26;
    if (d < 0) {
        d += 26;
    }

    if (c >= 'a' && c <= 'z') {
        return 'a' + (c - 'a' + d) % 26;
    }
    if (c >= 'A' && c <= 'Z') {
        return 'A' + (c - 'A' + d) % 26;
    }

    //números, espacios y símbolos se dejan sin modificar
    return c;
}

string CifradorCesar::encriptar(const string& textoOriginal) const {
    string resultado = textoOriginal;
    for (size_t i = 0; i < resultado.size(); i++) {
        resultado[i] = desplazarCaracter(resultado[i], desplazamiento);
    }
    return resultado;
}

string CifradorCesar::desencriptar(const string& textoCifrado) const {
    string resultado = textoCifrado;
    for (size_t i = 0; i < resultado.size(); i++) {
        resultado[i] = desplazarCaracter(resultado[i], -desplazamiento);
    }
    return resultado;
}