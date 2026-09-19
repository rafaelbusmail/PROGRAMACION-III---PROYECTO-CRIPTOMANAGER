#ifndef CIFRADORVIGENERE_H
#define CIFRADORVIGENERE_H

#include <string>
using namespace std;

//cada letra del texto se desplaza según la letra correspondiente de la clave
class CifradorVigenere {
private:
    string clave;

    char desplazarCaracter(char c, int desplazamiento) const;

public:
    CifradorVigenere(const string& claveInicial = "clave");
    ~CifradorVigenere();

    void setClave(const string& claveNueva);
    string getClave() const;

    string encriptar(const string& textoOriginal) const;
    string desencriptar(const string& textoCifrado) const;
};

#endif