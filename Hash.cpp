#include "Hash.h"
#include <sstream>
#include <iomanip>

Hash::Hash() {
    //constructor vacío
}

Hash::~Hash() {
    //destructor vacío
}

uint32_t Hash::calcularHash(const string& contenido) const {
    //djb2: se recorre cada byte y se combina con el hash acumulado
    //multiplicando por 33 (hash << 5 + hash) y sumando el carácter
    uint32_t hash = 5381;

    for (size_t i = 0; i < contenido.size(); i++) {
        hash = ((hash << 5) + hash) + (unsigned char)contenido[i];
    }

    return hash;
}

string Hash::calcularHashTexto(const string& contenido) const {
    uint32_t valor = calcularHash(contenido);

    ostringstream oss;
    oss << hex << uppercase << setfill('0') << setw(8) << valor;
    return oss.str();
}