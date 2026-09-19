//RAFAEL BUSMAIL - 22541446
//Proyecto Final - Cripto-Manager

#include <iostream>
#include <iomanip>
#include <fstream>
#ifdef _WIN32
#include <windows.h>
#endif
#include "Utilidades.h"
#include "GestorArchivos.h"
#include "CifradorCesar.h"
#include "CifradorVigenere.h"
#include "CifradorXOR.h"
#include "ConversorBinario.h"
#include "HistorialOperaciones.h"
#include "GestorUsuarios.h"
#include "Selectorarchivos.h"
#include "Hash.h"

using namespace std;

//prototipos
bool menuAcceso(GestorUsuarios& usuarios);
void mostrarMenu(const GestorUsuarios& usuarios);
void imprimirSeparador(const string& titulo);
void imprimirFila(const string& col1, const string& col2, const string& col3);
void opcionCesarTexto(CifradorCesar& cesar, HistorialOperaciones& historial, const string& usuarioActual);
void opcionCesarArchivo(CifradorCesar& cesar, GestorArchivos& gestor, SelectorArchivos& selector, HistorialOperaciones& historial, const string& usuarioActual);
void opcionVigenereTexto(CifradorVigenere& vigenere, HistorialOperaciones& historial, const string& usuarioActual);
void opcionVigenereArchivo(CifradorVigenere& vigenere, GestorArchivos& gestor, SelectorArchivos& selector, HistorialOperaciones& historial, const string& usuarioActual);
void opcionXORTexto(ConversorBinario& conversor, HistorialOperaciones& historial, const string& usuarioActual);
void opcionXORArchivo(GestorArchivos& gestor, SelectorArchivos& selector, ConversorBinario& conversor, HistorialOperaciones& historial, const string& usuarioActual);
void opcionBuscarHistorial(HistorialOperaciones& historial, const GestorUsuarios& usuarios);
void opcionReporte(HistorialOperaciones& historial, SelectorArchivos& selector, const GestorUsuarios& usuarios);
void opcionCalcularHash(GestorArchivos& gestor, SelectorArchivos& selector, Hash& hasher);
void opcionRegistrarUsuario(GestorUsuarios& usuarios);

int main() {
#ifdef _WIN32
    //en Windows se fuerza la consola a UTF-8 para que las tildes se vean bien;
    //en Linux/macOS la consola ya maneja UTF-8 por defecto
    SetConsoleOutputCP(65001);
#endif

    CifradorCesar cesar;
    CifradorVigenere vigenere;
    GestorArchivos gestor;
    SelectorArchivos selector;
    ConversorBinario conversor;
    Hash hasher;
    GestorUsuarios usuarios;       //carga usuarios.txt si ya existe
    HistorialOperaciones historial; //carga historial.txt si ya existe

    cout << "===        CRIPTO-MANAGER: SISTEMA DE CIFRADO        ===" << endl;
    cout << "\nNivel 1: Cesar | Nivel 2: XOR | Nivel 3: Vigenere" << endl;
    cout << "----------------------------------------------------------" << endl;

    //no se entra al programa sin iniciar sesión
    if (!menuAcceso(usuarios)) {
        cout << ">> Saliendo del sistema. ¡Hasta luego!" << endl;
        return 0;
    }

    int opcion = 0;

    do {
        string usuarioActual = usuarios.getUsuarioActual();
        bool esAdmin = usuarios.esAdministrador();

        mostrarMenu(usuarios);
        opcion = validarEntero("Seleccione una opción: ");

        switch (opcion) {
        case 1:
            opcionCesarTexto(cesar, historial, usuarioActual);
            break;
        case 2:
            opcionCesarArchivo(cesar, gestor, selector, historial, usuarioActual);
            break;
        case 3:
            opcionVigenereTexto(vigenere, historial, usuarioActual);
            break;
        case 4:
            opcionVigenereArchivo(vigenere, gestor, selector, historial, usuarioActual);
            break;
        case 5:
            opcionXORTexto(conversor, historial, usuarioActual);
            break;
        case 6:
            opcionXORArchivo(gestor, selector, conversor, historial, usuarioActual);
            break;
        case 7:
            historial.mostrarHistorial(usuarioActual, esAdmin);
            break;
        case 8:
            opcionBuscarHistorial(historial, usuarios);
            break;
        case 9:
            historial.ordenarPorFecha();
            cout << ">> Historial ordenado por fecha/hora." << endl;
            break;
        case 10:
            historial.mostrarEstadisticas(usuarioActual, esAdmin);
            break;
        case 11:
            opcionReporte(historial, selector, usuarios);
            break;
        case 12:
            opcionCalcularHash(gestor, selector, hasher);
            break;
        case 13:
            //cerrar sesión y volver al menú de acceso
            usuarios.cerrarSesion();
            cout << ">> Sesión cerrada." << endl;
            if (!menuAcceso(usuarios)) {
                opcion = 0; //el usuario decidió salir desde el menú de acceso
            }
            break;
        case 14:
            if (!esAdmin) {
                cout << ">> Esta opción es solo para administradores." << endl;
                break;
            }
            usuarios.mostrarUsuarios();
            break;
        case 15:
            if (!esAdmin) {
                cout << ">> Esta opción es solo para administradores." << endl;
                break;
            }
            opcionRegistrarUsuario(usuarios);
            break;
        case 16:
            if (!esAdmin) {
                cout << ">> Esta opción es solo para administradores." << endl;
                break;
            }
            historial.ordenarPorUsuario();
            cout << ">> Historial ordenado por usuario." << endl;
            break;
        case 0:
            cout << ">> Cambios guardados. ¡Hasta luego!" << endl;
            break;
        default:
            cout << ">> ¡Opción inválida!" << endl;
            break;
        }

    } while (opcion != 0);

    return 0;
}

bool menuAcceso(GestorUsuarios& usuarios) {
    //devuelve true si se logró iniciar sesión, false si el usuario decide salir
    while (!usuarios.haySesionActiva()) {
        cout << "\n            ACCESO AL SISTEMA            " << endl;
        cout << "=====================================" << endl;
        cout << "1. Iniciar sesión" << endl;
        cout << "2. Registrarse como usuario nuevo" << endl;
        cout << "0. Salir del programa" << endl;
        cout << "=====================================" << endl;

        int opcion = validarEnteroRango("Seleccione una opción: ", 0, 2);

        if (opcion == 0) {
            return false;
        }

        string nombre = leerTextoNoVacio("Usuario: ");
        string clave = leerTextoNoVacio("Contraseña: ");

        if (opcion == 1) {
            if (usuarios.iniciarSesion(nombre, clave)) {
                cout << ">> ¡Bienvenido, " << usuarios.getUsuarioActual()
                     << "! (rol: " << usuarios.getRolActual() << ")" << endl;
            } else {
                cout << ">> ERROR: Usuario o contraseña incorrectos." << endl;
            }
        } else {
            //los usuarios que se registran solos siempre entran con rol USUARIO;
            //solo un ADMIN puede crear otro ADMIN desde el menú principal
            if (usuarios.registrar(nombre, clave, "USUARIO")) {
                cout << ">> Usuario registrado correctamente. Ya puede iniciar sesión." << endl;
            } else {
                cout << ">> ERROR: Ese nombre de usuario ya está ocupado." << endl;
            }
        }
    }

    return true;
}

//ancho de cada una de las 3 columnas del menú
const size_t ANCHO_COLUMNA = 26;
const size_t ANCHO_MENU = ANCHO_COLUMNA * 3;

void imprimirSeparador(const string& titulo) {
    //línea con el título incrustado
    string encabezado = "--- " + titulo + " ";
    size_t visible = anchoVisible(encabezado);

    cout << encabezado;
    if (visible < ANCHO_MENU) {
        cout << string(ANCHO_MENU - visible, '-');
    }
    cout << endl;
}

void imprimirFila(const string& col1, const string& col2, const string& col3) {
    //se usa rellenar() y no setw() porque las tildes ocupan 2 bytes en UTF-8
    cout << rellenar(col1, ANCHO_COLUMNA)
         << rellenar(col2, ANCHO_COLUMNA)
         << col3 << endl;
}

void mostrarMenu(const GestorUsuarios& usuarios) {
    cout << "\n" << string(ANCHO_MENU, '=') << endl;
    cout << "   MENÚ PRINCIPAL   |   Sesión: " << usuarios.getUsuarioActual()
         << "   |   Rol: " << usuarios.getRolActual() << endl;
    cout << string(ANCHO_MENU, '=') << endl;

    //los tres algoritmos, uno por columna, para no tener 16 opciones en fila
    imprimirSeparador("CIFRADOS");
    imprimirFila("  [César]", "  [Vigenère]", "  [XOR]");
    imprimirFila("   1. Sobre texto", "   3. Sobre texto", "   5. Sobre texto");
    imprimirFila("   2. Sobre archivo", "   4. Sobre archivo", "   6. Sobre archivo");

    cout << endl;
    imprimirSeparador("HISTORIAL Y SISTEMA");
    imprimirFila("   7. Ver historial", "  10. Estadísticas", "  12. Calcular hash");
    imprimirFila("   8. Buscar", "  11. Generar reporte", "  13. Cerrar sesión");
    imprimirFila("   9. Ordenar por fecha", "", "   0. Salir");

    if (usuarios.esAdministrador()) {
        cout << endl;
        imprimirSeparador("ADMINISTRADOR");
        imprimirFila("  14. Ver usuarios", "  15. Registrar usuario", "  16. Ordenar por usuario");
    }

    cout << string(ANCHO_MENU, '=') << endl;
}

void opcionCesarTexto(CifradorCesar& cesar, HistorialOperaciones& historial, const string& usuarioActual) {
    cout << "\n--- Cifrado César sobre texto ---" << endl;
    cout << "1. Encriptar" << endl;
    cout << "2. Desencriptar" << endl;
    int sub = validarEnteroRango("Seleccione una opción: ", 1, 2);

    int desplazamiento = validarEntero("Ingrese el desplazamiento (ej. 3): ");
    cesar.setDesplazamiento(desplazamiento);

    string texto = leerTextoNoVacio("Ingrese el texto: ");

    if (sub == 1) {
        string resultado = cesar.encriptar(texto);
        cout << ">> Resultado encriptado: " << resultado << endl;
        historial.agregarRegistro(usuarioActual, "Cesar-Encriptar", "(texto directo)");
    } else {
        string resultado = cesar.desencriptar(texto);
        cout << ">> Resultado desencriptado: " << resultado << endl;
        historial.agregarRegistro(usuarioActual, "Cesar-Desencriptar", "(texto directo)");
    }
}

void opcionCesarArchivo(CifradorCesar& cesar, GestorArchivos& gestor, SelectorArchivos& selector, HistorialOperaciones& historial, const string& usuarioActual) {
    cout << "\n--- Cifrado César sobre archivo .txt ---" << endl;
    cout << "1. Encriptar" << endl;
    cout << "2. Desencriptar" << endl;
    int sub = validarEnteroRango("Seleccione una opción: ", 1, 2);

    cout << ">> Se abrirá el explorador de archivos. Selecciona el archivo .txt a procesar..." << endl;
    string rutaEntrada = selector.seleccionarArchivoAbrir();

    if (rutaEntrada.empty()) {
        cout << ">> Operación cancelada, no se seleccionó ningún archivo." << endl;
        return;
    }

    int desplazamiento = validarEntero("Ingrese el desplazamiento (ej. 3): ");
    cesar.setDesplazamiento(desplazamiento);

    string contenido = gestor.leerTexto(rutaEntrada);
    string resultado;
    string tipoOperacion;

    if (sub == 1) {
        resultado = cesar.encriptar(contenido);
        tipoOperacion = "Cesar-Encriptar-Archivo";
    } else {
        resultado = cesar.desencriptar(contenido);
        tipoOperacion = "Cesar-Desencriptar-Archivo";
    }

    //se muestra el resultado en pantalla para no tener que abrir el archivo
    mostrarResultadoEnConsola("CONTENIDO ORIGINAL", contenido);
    mostrarResultadoEnConsola("RESULTADO", resultado);

    cout << ">> Ahora elige dónde guardar el resultado..." << endl;
    string rutaSalida = selector.seleccionarArchivoGuardar();

    if (rutaSalida.empty()) {
        cout << ">> Operación cancelada, no se guardó ningún archivo." << endl;
        return;
    }

    if (gestor.guardarTexto(rutaSalida, resultado)) {
        cout << ">> Archivo procesado y guardado correctamente." << endl;
        historial.agregarRegistro(usuarioActual, tipoOperacion, rutaEntrada);
    } else {
        cout << ">> ERROR: No se pudo guardar el archivo de salida." << endl;
    }
}

void opcionVigenereTexto(CifradorVigenere& vigenere, HistorialOperaciones& historial, const string& usuarioActual) {
    cout << "\n--- Cifrado Vigenère sobre texto ---" << endl;
    cout << "1. Encriptar" << endl;
    cout << "2. Desencriptar" << endl;
    int sub = validarEnteroRango("Seleccione una opción: ", 1, 2);

    string clave = leerTextoNoVacio("Ingrese la palabra clave (solo letras, ej. LLAVE): ");
    vigenere.setClave(clave);

    string texto = leerTextoNoVacio("Ingrese el texto: ");

    if (sub == 1) {
        string resultado = vigenere.encriptar(texto);
        cout << ">> Resultado encriptado: " << resultado << endl;
        historial.agregarRegistro(usuarioActual, "Vigenere-Encriptar", "(texto directo)");
    } else {
        string resultado = vigenere.desencriptar(texto);
        cout << ">> Resultado desencriptado: " << resultado << endl;
        historial.agregarRegistro(usuarioActual, "Vigenere-Desencriptar", "(texto directo)");
    }
}

void opcionVigenereArchivo(CifradorVigenere& vigenere, GestorArchivos& gestor, SelectorArchivos& selector, HistorialOperaciones& historial, const string& usuarioActual) {
    cout << "\n--- Cifrado Vigenère sobre archivo .txt ---" << endl;
    cout << "1. Encriptar" << endl;
    cout << "2. Desencriptar" << endl;
    int sub = validarEnteroRango("Seleccione una opción: ", 1, 2);

    cout << ">> Se abrirá el explorador de archivos. Selecciona el archivo .txt a procesar..." << endl;
    string rutaEntrada = selector.seleccionarArchivoAbrir();

    if (rutaEntrada.empty()) {
        cout << ">> Operación cancelada, no se seleccionó ningún archivo." << endl;
        return;
    }

    string clave = leerTextoNoVacio("Ingrese la palabra clave (solo letras, ej. LLAVE): ");
    vigenere.setClave(clave);

    string contenido = gestor.leerTexto(rutaEntrada);
    string resultado;
    string tipoOperacion;

    if (sub == 1) {
        resultado = vigenere.encriptar(contenido);
        tipoOperacion = "Vigenere-Encriptar-Archivo";
    } else {
        resultado = vigenere.desencriptar(contenido);
        tipoOperacion = "Vigenere-Desencriptar-Archivo";
    }

    mostrarResultadoEnConsola("CONTENIDO ORIGINAL", contenido);
    mostrarResultadoEnConsola("RESULTADO", resultado);

    cout << ">> Ahora elige dónde guardar el resultado..." << endl;
    string rutaSalida = selector.seleccionarArchivoGuardar();

    if (rutaSalida.empty()) {
        cout << ">> Operación cancelada, no se guardó ningún archivo." << endl;
        return;
    }

    if (gestor.guardarTexto(rutaSalida, resultado)) {
        cout << ">> Archivo procesado y guardado correctamente." << endl;
        historial.agregarRegistro(usuarioActual, tipoOperacion, rutaEntrada);
    } else {
        cout << ">> ERROR: No se pudo guardar el archivo de salida." << endl;
    }
}

void opcionXORTexto(ConversorBinario& conversor, HistorialOperaciones& historial, const string& usuarioActual) {
    cout << "\n--- Cifrado XOR sobre texto ---" << endl;

    string clave = leerTextoNoVacio("Ingrese la clave secreta: ");
    CifradorXOR cifradorXor(clave);

    string texto = leerTextoNoVacio("Ingrese el texto (XOR es simétrico: el mismo proceso encripta y desencripta): ");

    int longitud = texto.size();
    char* datosOriginales = new char[longitud];
    for (int i = 0; i < longitud; i++) {
        datosOriginales[i] = texto[i];
    }

    char* resultado = cifradorXor.procesarBloque(datosOriginales, longitud);

    //se muestra en binario (0s y 1s) en vez de hexadecimal
    cout << ">> Resultado (binario): " << conversor.aTextoBinario(resultado, longitud) << endl;

    historial.agregarRegistro(usuarioActual, "XOR-Procesar", "(texto directo)");

    delete[] datosOriginales;
    delete[] resultado;
}

void opcionXORArchivo(GestorArchivos& gestor, SelectorArchivos& selector, ConversorBinario& conversor, HistorialOperaciones& historial, const string& usuarioActual) {
    cout << "\n--- Cifrado XOR sobre archivo ---" << endl;
    cout << "1. Encriptar (guarda el resultado como texto binario, legible en Notepad)" << endl;
    cout << "2. Desencriptar (a partir de un archivo de texto binario)" << endl;
    int sub = validarEnteroRango("Seleccione una opción: ", 1, 2);

    cout << ">> Se abrirá el explorador de archivos. Selecciona el archivo a procesar..." << endl;
    string rutaEntrada = selector.seleccionarArchivoAbrir("Todos los archivos (*.*)\0*.*\0");

    if (rutaEntrada.empty()) {
        cout << ">> Operación cancelada, no se seleccionó ningún archivo." << endl;
        return;
    }

    string clave = leerTextoNoVacio("Ingrese la clave secreta: ");

    cout << ">> Ahora elige dónde guardar el resultado..." << endl;
    string rutaSalida = selector.seleccionarArchivoGuardar("Todos los archivos (*.*)\0*.*\0", "");

    if (rutaSalida.empty()) {
        cout << ">> Operación cancelada, no se guardó ningún archivo." << endl;
        return;
    }

    CifradorXOR cifradorXor(clave);
    const int TAMANO_BLOQUE = 256; //procesamiento por bloques, no se carga el archivo completo a la vez

    if (sub == 1) {
        //ENCRIPTAR
        long tamanoArchivo = gestor.obtenerTamanoArchivo(rutaEntrada);

        if (tamanoArchivo <= 0) {
            cout << ">> ERROR: El archivo está vacío o no se pudo leer." << endl;
            return;
        }

        //se escribe bloque por bloque directamente al archivo de salida:
        ofstream salida(rutaSalida.c_str(), ios::trunc);
        if (!salida.is_open()) {
            cout << ">> ERROR: No se pudo crear el archivo de salida." << endl;
            return;
        }

        string vistaPrevia = ""; //solo el inicio, para mostrarlo en consola
        long totalCaracteres = 0;
        long posicion = 0;

        while (posicion < tamanoArchivo) {
            int bytesLeidos = 0;
            char* buffer = gestor.leerBloqueBinario(rutaEntrada, posicion, TAMANO_BLOQUE, bytesLeidos);

            if (buffer == nullptr || bytesLeidos <= 0) {
                delete[] buffer;
                break;
            }

            //se pasa "posicion" para que la clave no reinicie su fase en cada bloque
            char* bloqueProcesado = cifradorXor.procesarBloque(buffer, bytesLeidos, posicion);
            string bloqueBinario = conversor.aTextoBinario(bloqueProcesado, bytesLeidos);

            salida << bloqueBinario;
            totalCaracteres += bloqueBinario.size();

            if (vistaPrevia.size() < 600) {
                vistaPrevia += bloqueBinario;
            }

            posicion += bytesLeidos;

            delete[] buffer;
            delete[] bloqueProcesado;
        }

        salida.close();

        if (totalCaracteres > 600) {
            cout << "\n--- RESULTADO EN BINARIO ---" << endl;
            cout << vistaPrevia << endl;
            cout << "[...] (se muestran los primeros 600 de " << totalCaracteres << " caracteres)" << endl;
            cout << "----------------------------------------" << endl;
        } else {
            mostrarResultadoEnConsola("RESULTADO EN BINARIO", vistaPrevia);
        }

        cout << ">> Archivo encriptado y convertido a texto binario correctamente." << endl;
        historial.agregarRegistro(usuarioActual, "XOR-Encriptar-Archivo", rutaEntrada);

    } else {
        //DESENCRIPTAR
        string contenidoBinario = gestor.leerTexto(rutaEntrada);

        int longitudBytes = 0;
        char* bytesReconstruidos = conversor.aBytes(contenidoBinario, longitudBytes);

        if (longitudBytes <= 0) {
            cout << ">> ERROR: el archivo no contiene texto binario válido." << endl;
            delete[] bytesReconstruidos;
            return;
        }

        char* bloqueProcesado = cifradorXor.procesarBloque(bytesReconstruidos, longitudBytes, 0);

        //el resultado son bytes crudos, así que se arma un string para mostrarlo
        string textoRecuperado(bloqueProcesado, longitudBytes);
        mostrarResultadoEnConsola("TEXTO RECUPERADO", textoRecuperado);

        if (gestor.escribirBloqueBinario(rutaSalida, bloqueProcesado, longitudBytes, true)) {
            cout << ">> Archivo desencriptado y restaurado correctamente." << endl;
            historial.agregarRegistro(usuarioActual, "XOR-Desencriptar-Archivo", rutaEntrada);
        } else {
            cout << ">> ERROR: No se pudo guardar el archivo de salida." << endl;
        }

        delete[] bytesReconstruidos;
        delete[] bloqueProcesado;
    }
}

void opcionBuscarHistorial(HistorialOperaciones& historial, const GestorUsuarios& usuarios) {
    string tipo = leerTextoNoVacio("Ingrese el tipo de operación a buscar (ej. Cesar-Encriptar): ");

    int posicion = historial.buscarPorTipo(tipo, usuarios.getUsuarioActual(), usuarios.esAdministrador());
    if (posicion == -1) {
        cout << ">> No se encontró ninguna operación de ese tipo." << endl;
    } else {
        cout << ">> Encontrado en la posición " << posicion + 1 << " del historial." << endl;
    }
}

void opcionReporte(HistorialOperaciones& historial, SelectorArchivos& selector, const GestorUsuarios& usuarios) {
    cout << "\n--- Generar reporte de uso en archivo ---" << endl;
    cout << ">> Elige dónde guardar el reporte..." << endl;

    string ruta = selector.seleccionarArchivoGuardar();

    if (ruta.empty()) {
        cout << ">> Operación cancelada, no se guardó el reporte." << endl;
        return;
    }

    if (historial.guardarReporte(ruta, usuarios.getUsuarioActual(), usuarios.esAdministrador())) {
        cout << ">> Reporte generado correctamente en: " << ruta << endl;
    } else {
        cout << ">> ERROR: No se pudo generar el reporte." << endl;
    }
}

void opcionCalcularHash(GestorArchivos& gestor, SelectorArchivos& selector, Hash& hasher) {
    cout << "\n--- Calcular hash (verificación de integridad) ---" << endl;
    cout << "1. Hash de un texto" << endl;
    cout << "2. Hash de un archivo" << endl;
    int sub = validarEnteroRango("Seleccione una opción: ", 1, 2);

    if (sub == 1) {
        string texto = leerTextoNoVacio("Ingrese el texto: ");
        cout << ">> Hash (djb2): " << hasher.calcularHashTexto(texto) << endl;
    } else {
        cout << ">> Se abrirá el explorador de archivos. Selecciona el archivo..." << endl;
        string ruta = selector.seleccionarArchivoAbrir("Todos los archivos (*.*)\0*.*\0");

        if (ruta.empty()) {
            cout << ">> Operación cancelada, no se seleccionó ningún archivo." << endl;
            return;
        }

        string contenido = gestor.leerTexto(ruta);
        cout << ">> Hash (djb2) del archivo: " << hasher.calcularHashTexto(contenido) << endl;
        cout << ">> Guarda este valor: si vuelves a calcular el hash y no coincide," << endl;
        cout << "   significa que el contenido del archivo cambió." << endl;
    }
}

void opcionRegistrarUsuario(GestorUsuarios& usuarios) {
    cout << "\n--- Registrar nuevo usuario (administrador) ---" << endl;

    string nombre = leerTextoNoVacio("Nombre del nuevo usuario: ");
    string clave = leerTextoNoVacio("Contraseña: ");

    cout << "1. Rol USUARIO (acceso normal)" << endl;
    cout << "2. Rol ADMIN (acceso total)" << endl;
    int opcionRol = validarEnteroRango("Seleccione el rol: ", 1, 2);

    string rol = (opcionRol == 2) ? "ADMIN" : "USUARIO";

    if (usuarios.registrar(nombre, clave, rol)) {
        cout << ">> Usuario '" << nombre << "' registrado con rol " << rol << "." << endl;
    } else {
        cout << ">> ERROR: Ese nombre de usuario ya está ocupado." << endl;
    }
}