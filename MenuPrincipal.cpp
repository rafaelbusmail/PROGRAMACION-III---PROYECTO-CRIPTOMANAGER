#include "MenuPrincipal.h"
#include "MenuAcceso.h"
#include "Utilidades.h"
#include <iostream>
#include <fstream>
#include <cctype>

using namespace std;

//compara dos rutas de archivo: en Windows se ignoran mayúsculas/minúsculas
//(el sistema de archivos no las distingue); en Linux se compara exacto
static bool mismasRutas(const string& a, const string& b) {
#ifdef _WIN32
    if (a.size() != b.size()) {
        return false;
    }
    for (size_t i = 0; i < a.size(); i++) {
        if (tolower((unsigned char)a[i]) != tolower((unsigned char)b[i])) {
            return false;
        }
    }
    return true;
#else
    return a == b;
#endif
}

MenuPrincipal::MenuPrincipal(GestorUsuarios& usuarios)
    : usuarios(usuarios) {
    //los demás miembros (cifradores, gestor, selector, etc) se
    //construyen con su constructor por defecto
}

void MenuPrincipal::ejecutar() {
    int opcion = 0;

    do {
        bool esAdmin = usuarios.esAdministrador();

        mostrarMenu();
        opcion = validarEntero("Seleccione una opción: ");

        switch (opcion) {
        case 1:
            opcionCesarTexto();
            break;
        case 2:
            opcionCesarArchivo();
            break;
        case 3:
            opcionVigenereTexto();
            break;
        case 4:
            opcionVigenereArchivo();
            break;
        case 5:
            opcionXORTexto();
            break;
        case 6:
            opcionXORArchivo();
            break;
        case 7:
            historial.mostrarHistorial(usuarios.getUsuarioActual(), esAdmin);
            break;
        case 8:
            opcionBuscarHistorial();
            break;
        case 9:
            historial.ordenarPorFecha();
            cout << ">> Historial ordenado por fecha/hora." << endl;
            break;
        case 10:
            historial.mostrarEstadisticas(usuarios.getUsuarioActual(), esAdmin);
            break;
        case 11:
            opcionReporte();
            break;
        case 12:
            opcionCalcularHash();
            break;
        case 13:
            //cerrar sesión y volver al menú de acceso
            usuarios.cerrarSesion();
            cout << ">> Sesión cerrada." << endl;
            {
                MenuAcceso acceso;
                if (!acceso.ejecutar(usuarios)) {
                    opcion = 0; //el usuario decidió salir desde el menú de acceso
                }
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
            opcionRegistrarUsuario();
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
}

void MenuPrincipal::imprimirSeparador(const string& titulo) const {
    //línea con el título incrustado
    string encabezado = "--- " + titulo + " ";
    size_t visible = anchoVisible(encabezado);

    cout << encabezado;
    if (visible < ANCHO_MENU) {
        cout << string(ANCHO_MENU - visible, '-');
    }
    cout << endl;
}

void MenuPrincipal::imprimirFila(const string& col1, const string& col2, const string& col3) const {
    //se usa rellenar() y no setw() porque las tildes ocupan 2 bytes en UTF-8
    cout << rellenar(col1, ANCHO_COLUMNA)
         << rellenar(col2, ANCHO_COLUMNA)
         << col3 << endl;
}

void MenuPrincipal::mostrarMenu() const {
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

void MenuPrincipal::opcionCesarTexto() {
    string usuarioActual = usuarios.getUsuarioActual();

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

void MenuPrincipal::opcionCesarArchivo() {
    string usuarioActual = usuarios.getUsuarioActual();

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

void MenuPrincipal::opcionVigenereTexto() {
    string usuarioActual = usuarios.getUsuarioActual();

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

void MenuPrincipal::opcionVigenereArchivo() {
    string usuarioActual = usuarios.getUsuarioActual();

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

void MenuPrincipal::opcionXORTexto() {
    string usuarioActual = usuarios.getUsuarioActual();

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

void MenuPrincipal::opcionXORArchivo() {
    string usuarioActual = usuarios.getUsuarioActual();

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

        //si entrada y salida son el mismo archivo, abrir la salida con ios::trunc
        //borraría la entrada antes de leerla y el resultado quedaría vacío
        if (mismasRutas(rutaEntrada, rutaSalida)) {
            cout << ">> ERROR: El archivo de entrada y el de salida son el mismo." << endl;
            cout << "   Elige una ruta de salida diferente para conservar el original." << endl;
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

void MenuPrincipal::opcionBuscarHistorial() {
    string tipo = leerTextoNoVacio("Ingrese el tipo de operación a buscar (ej. Cesar-Encriptar): ");

    int posicion = historial.buscarPorTipo(tipo, usuarios.getUsuarioActual(), usuarios.esAdministrador());
    if (posicion == -1) {
        cout << ">> No se encontró ninguna operación de ese tipo." << endl;
    } else {
        cout << ">> Encontrado en la posición " << posicion + 1 << " del historial." << endl;
    }
}

void MenuPrincipal::opcionReporte() {
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

void MenuPrincipal::opcionCalcularHash() {
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

void MenuPrincipal::opcionRegistrarUsuario() {
    cout << "\n--- Registrar nuevo usuario (administrador) ---" << endl;

    string nombre = leerTextoNoVacio("Nombre del nuevo usuario: ");
    string clave = leerTextoNoVacio("Contraseña: ");

    cout << "1. Rol USUARIO (acceso normal)" << endl;
    cout << "2. Rol ADMIN (acceso total)" << endl;
    int opcionRol = validarEnteroRango("Seleccione el rol: ", 1, 2);

    string rol = (opcionRol == 2) ? "ADMIN" : "USUARIO";

    ResultadoRegistro resultado = usuarios.registrar(nombre, clave, rol);
    if (resultado == ResultadoRegistro::OK) {
        cout << ">> Usuario '" << nombre << "' registrado con rol " << rol << "." << endl;
    } else {
        switch (resultado) {
        case ResultadoRegistro::NOMBRE_CORTO:
            cout << ">> ERROR: El nombre de usuario debe tener al menos 3 caracteres." << endl;
            break;
        case ResultadoRegistro::NOMBRE_INVALIDO:
            cout << ">> ERROR: El nombre de usuario no puede contener el carácter '|'." << endl;
            break;
        case ResultadoRegistro::CLAVE_DEBIL:
            cout << ">> ERROR: La contraseña debe tener al menos 5 caracteres e incluir una letra, un número y un carácter especial." << endl;
            break;
        case ResultadoRegistro::NOMBRE_OCUPADO:
            cout << ">> ERROR: Ese nombre de usuario ya está ocupado." << endl;
            break;
        default:
            break;
        }
    }
}