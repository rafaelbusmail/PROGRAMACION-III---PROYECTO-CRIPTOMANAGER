#ifndef ARREGLODINAMICO_H
#define ARREGLODINAMICO_H

//(template) sirve sin depender de vector
template <typename T>
class ArregloDinamico {
private:
    T* datos;
    int cantidad;
    int capacidad;

    void redimensionar() {
        capacidad *= 2;
        T* nuevoArreglo = new T[capacidad];

        for (int i = 0; i < cantidad; i++) {
            nuevoArreglo[i] = datos[i];
        }

        delete[] datos;
        datos = nuevoArreglo;
    }

public:
    ArregloDinamico(int capacidadInicial = 5) {
        capacidad = capacidadInicial;
        cantidad = 0;
        datos = new T[capacidad];
    }

    //constructor de copia y operador de asignación
    ArregloDinamico(const ArregloDinamico<T>& otro) {
        capacidad = otro.capacidad;
        cantidad = otro.cantidad;
        datos = new T[capacidad];
        for (int i = 0; i < cantidad; i++) {
            datos[i] = otro.datos[i];
        }
    }

    ArregloDinamico<T>& operator=(const ArregloDinamico<T>& otro) {
        if (this == &otro) {
            return *this;
        }

        delete[] datos;
        capacidad = otro.capacidad;
        cantidad = otro.cantidad;
        datos = new T[capacidad];
        for (int i = 0; i < cantidad; i++) {
            datos[i] = otro.datos[i];
        }
        return *this;
    }

    ~ArregloDinamico() {
        delete[] datos;
    }

    void agregar(const T& elemento) {
        if (cantidad == capacidad) {
            redimensionar();
        }
        datos[cantidad] = elemento;
        cantidad++;
    }

    T& obtener(int indice) {
        return datos[indice];
    }

    const T& obtener(int indice) const {
        return datos[indice];
    }

    int getCantidad() const {
        return cantidad;
    }
};

#endif