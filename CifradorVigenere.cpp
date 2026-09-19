#include "CifradorVigenere.h"
#include <cctype>

CifradorVigenere::CifradorVigenere(const string& claveInicial) {
    clave = claveInicial;
}

CifradorVigenere::~CifradorVigenere() {
    //el string se libera automáticamente
}

void CifradorVigenere::setClave(const string& claveNueva) {
    clave = claveNueva;
}

string CifradorVigenere::getClave() const {
    return clave;
}

char CifradorVigenere::desplazarCaracter(char c, int desplazamiento) const {
    //se normaliza el desplazamiento
    //y se preserva mayúscula/minúscula
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

    return c;
}

string CifradorVigenere::encriptar(const string& textoOriginal) const {
    string resultado = textoOriginal;
    size_t indiceClave = 0;

    for (size_t i = 0; i < resultado.size(); i++) {
        char c = resultado[i];

        //solo las letras avanzan el índice de la clave
        if (isalpha((unsigned char)c)) {
            char letraClave = clave[indiceClave % clave.size()];
            int desplazamiento = toupper((unsigned char)letraClave) - 'A';

            resultado[i] = desplazarCaracter(c, desplazamiento);
            indiceClave++;
        }
    }

    return resultado;
}

string CifradorVigenere::desencriptar(const string& textoCifrado) const {
    string resultado = textoCifrado;
    size_t indiceClave = 0;

    for (size_t i = 0; i < resultado.size(); i++) {
        char c = resultado[i];

        if (isalpha((unsigned char)c)) {
            char letraClave = clave[indiceClave % clave.size()];
            int desplazamiento = toupper((unsigned char)letraClave) - 'A';

            resultado[i] = desplazarCaracter(c, -desplazamiento);
            indiceClave++;
        }
    }

    return resultado;
}