#ifndef HASH_H
#define HASH_H

#include <string>
#include <cstdint>
using namespace std;

//algoritmo djb2
//uint32_t (siempre 32 bits) para linux en vez de unsigned long
class Hash {
public:
    Hash();
    ~Hash();

    uint32_t calcularHash(const string& contenido) const;

    //devuelve el resultado en hexadecimal para
    //mostrarlo o guardarlo como texto
    string calcularHashTexto(const string& contenido) const;
};

#endif