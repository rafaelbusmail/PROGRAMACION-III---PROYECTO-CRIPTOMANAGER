#ifndef CIFRADORXOR_H
#define CIFRADORXOR_H

#include <string>
using namespace std;

//Nivel 2
class CifradorXOR {
private:
    string clave;

public:
    CifradorXOR(const string& claveInicial = "");
    ~CifradorXOR();

    void setClave(const string& claveNueva);
    string getClave() const;

    //función sirve tanto para encriptar como para desencriptar;
    char* procesarBloque(const char* datosEntrada, int longitud, long posicionInicial = 0) const;
};

#endif