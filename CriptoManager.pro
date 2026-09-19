# ============================================================
#  CriptoManager - Archivo de proyecto qmake (.pro)
#  Alternativa a CMakeLists.txt: permite compilar con qmake
#  (qmake && mingw32-make, o abriendo el .pro en Qt Creator)
#
#  IMPORTANTE: el proyecto NO usa Qt (es C++ puro + Win32 API),
#  por eso se quitan los módulos core/gui: si se dejaran, qmake
#  enlazaría Qt6Core.dll y el .exe dejaría de ser portable.
# ============================================================

QT -= core gui

TEMPLATE = app
TARGET = CriptoManager

CONFIG += console
CONFIG -= app_bundle
CONFIG += c++17

SOURCES += \
    main.cpp \
    Utilidades.cpp \
    CifradorCesar.cpp \
    CifradorVigenere.cpp \
    CifradorXOR.cpp \
    ConversorBinario.cpp \
    GestorArchivos.cpp \
    GestorUsuarios.cpp \
    Hash.cpp \
    HistorialOperaciones.cpp \
    Selectorarchivos.cpp

HEADERS += \
    Utilidades.h \
    RegistroOperacion.h \
    CifradorCesar.h \
    CifradorVigenere.h \
    CifradorXOR.h \
    ConversorBinario.h \
    GestorArchivos.h \
    GestorUsuarios.h \
    Hash.h \
    HistorialOperaciones.h \
    Selectorarchivos.h \
    Arreglodinamico.h \
    Usuario.h

win32 {
    # diálogo de archivos nativo de Windows (GetOpenFileNameA / GetSaveFileNameA)
    LIBS += -lcomdlg32

    # enlazado estático del runtime de MinGW: el .exe queda autocontenido y
    # corre en cualquier Windows (y en Linux vía Wine) sin DLLs adicionales
    QMAKE_LFLAGS += -static -static-libgcc -static-libstdc++
}