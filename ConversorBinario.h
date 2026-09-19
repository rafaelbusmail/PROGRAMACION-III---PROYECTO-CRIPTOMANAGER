#ifndef CONVERSORBINARIO_H
#define CONVERSORBINARIO_H

#include <string>
using namespace std;

//convierte bytes hacia/desde su representación como texto en binario (0s y 1s).
class ConversorBinario {
public:
    ConversorBinario();
    ~ConversorBinario();

    string aTextoBinario(const char* datos, int longitud) const;

    //proceso inverso: toma el texto binario y reconstruye los bytes originales
    //devuelve un puntero a memoria dinámica que el que llama debe liberar con delete[],
    char* aBytes(const string& textoBinario, int& longitudResultante) const;
};

#endif