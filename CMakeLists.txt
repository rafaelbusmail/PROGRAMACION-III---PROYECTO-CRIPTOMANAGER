cmake_minimum_required(VERSION 3.16)

project(CriptoManager LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(CriptoManager main.cpp
    Utilidades.cpp
    CifradorCesar.cpp
    CifradorXOR.cpp
    GestorArchivos.cpp
    HistorialOperaciones.cpp
    Utilidades.h
    RegistroOperacion.h
    CifradorCesar.h
    CifradorXOR.h
    GestorArchivos.h
    HistorialOperaciones.h
    Selectorarchivos.h
    Selectorarchivos.cpp
    Arreglodinamico.h
    Hash.h
    Hash.cpp
    ConversorBinario.h
    ConversorBinario.cpp
    CifradorVigenere.cpp
    CifradorVigenere.h
    Usuario.h
    GestorUsuarios.h
    GestorUsuarios.cpp)

target_link_libraries(CriptoManager comdlg32)

include(GNUInstallDirs)
install(TARGETS CriptoManager
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)