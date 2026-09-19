#ifndef CIFRADORCESAR_H
#define CIFRADORCESAR_H

#include <string>
using namespace std;

//Nivel 1
class CifradorCesar {
private:
    int desplazamiento;

    char desplazarCaracter(char c, int desplazamiento) const;

public:
    CifradorCesar(int desplazamientoInicial = 3);
    ~CifradorCesar();

    void setDesplazamiento(int d);
    int getDesplazamiento() const;

    //funciona igual para texto en memoria y para el contenido leído de un archivo .txt
    string encriptar(const string& textoOriginal) const;
    string desencriptar(const string& textoCifrado) const;
};

#endif