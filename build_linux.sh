#!/usr/bin/env bash
# ============================================================
#  CriptoManager - Script de compilacion (Linux)
#
#  Uso:
#    ./build_linux.sh          -> compila el .exe de Windows (cross) con mingw-w64
#    ./build_linux.sh run      -> compila el .exe y lo ejecuta con Wine
#    ./build_linux.sh native   -> compila version nativa de Linux
#                                 (sin dialogo Win32: pide rutas por consola)
#
#  Requisitos:
#    cross/run: g++-mingw-w64-x86-64, cmake, wine (solo para "run")
#    native   : g++, cmake
# ============================================================
set -e

ROOT="$(cd "$(dirname "$0")" && pwd)"
MODE="${1:-cross}"

case "$MODE" in
  cross|run)
    BUILD_DIR="$ROOT/build/linux-cross"
    command -v x86_64-w64-mingw32-g++ >/dev/null 2>&1 || {
      echo "[ERROR] Falta el compilador cruzado mingw-w64."
      echo "        Instalalo, por ejemplo:"
      echo "          Debian/Ubuntu: sudo apt install g++-mingw-w64-x86-64 cmake"
      echo "          Fedora:        sudo dnf install mingw64-gcc-c++ cmake"
      echo "          Arch:          sudo pacman -S mingw-w64-gcc cmake"
      exit 1
    }
    echo "[*] Cross-compilando CriptoManager.exe (Windows) en Linux..."
    cmake -S "$ROOT" -B "$BUILD_DIR" \
          -DCMAKE_SYSTEM_NAME=Windows \
          -DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++ \
          -DCMAKE_BUILD_TYPE=Release
    cmake --build "$BUILD_DIR" --config Release
    EXE="$BUILD_DIR/CriptoManager.exe"
    echo "[OK] Ejecutable generado: $EXE"
    if [ "$MODE" = "run" ]; then
      command -v wine >/dev/null 2>&1 || {
        echo "[ERROR] Falta wine. Instalalo: sudo apt install wine"
        exit 1
      }
      echo "[*] Ejecutando con Wine..."
      wine "$EXE"
    fi
    ;;
  native)
    BUILD_DIR="$ROOT/build/linux-native"
    echo "[*] Compilando version nativa de Linux..."
    cmake -S "$ROOT" -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Release
    cmake --build "$BUILD_DIR" --config Release
    echo "[OK] Ejecutable generado: $BUILD_DIR/CriptoManager"
    ;;
  *)
    echo "Uso: $0 [cross|run|native]"
    exit 1
    ;;
esac