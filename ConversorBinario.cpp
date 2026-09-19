#include "ConversorBinario.h"

ConversorBinario::ConversorBinario() {
    //constructor vacío
}

ConversorBinario::~ConversorBinario() {
    //destructor vacío
}

string ConversorBinario::aTextoBinario(const char* datos, int longitud) const {
    string resultado = "";

    for (int i = 0; i < longitud; i++) {
        unsigned char valor = (unsigned char)datos[i];

        //se recorre cada bit del byte, del más significativo (bit 7) al menos
        //significativo (bit 0), y se agrega '1' o '0' según corresponda
        for (int b = 7; b >= 0; b--) {
            resultado += ((valor >> b) & 1) ? '1' : '0';
        }

        resultado += " "; //separador; se ignora al momento de leer de vuelta
    }

    return resultado;
}

char* ConversorBinario::aBytes(const string& textoBinario, int& longitudResultante) const {
    //primero se filtran solo los caracteres '0'/'1' (se ignoran espacios,
    //saltos de línea, etc., por si el archivo se editó o se re-guardó)
    string soloBits = "";
    for (size_t i = 0; i < textoBinario.size(); i++) {
        if (textoBinario[i] == '0' || textoBinario[i] == '1') {
            soloBits += textoBinario[i];
        }
    }

    longitudResultante = soloBits.size() / 8;
    char* resultado = new char[longitudResultante];

    for (int i = 0; i < longitudResultante; i++) {
        unsigned char valor = 0;
        for (int b = 0; b < 8; b++) {
            valor = (valor << 1) | (soloBits[i * 8 + b] - '0');
        }
        resultado[i] = (char)valor;
    }

    return resultado;
}