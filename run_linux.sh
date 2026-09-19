#!/usr/bin/env bash
# ============================================================
#  CriptoManager - Ejecutar el .exe de Windows en Linux con Wine
#
#  Uso: ./run_linux.sh
#  Requisitos: wine instalado y el .exe ya compilado
#  (build_windows.bat en Windows, o ./build_linux.sh en Linux)
# ============================================================
set -e

ROOT="$(cd "$(dirname "$0")" && pwd)"

EXE=""
for candidato in "$ROOT/build/Release/CriptoManager.exe" \
                 "$ROOT/build/linux-cross/CriptoManager.exe" \
                 "$ROOT/build/Desktop_Qt_6_11_1_MinGW_64_bit_Debug/CriptoManager.exe"; do
  if [ -f "$candidato" ]; then
    EXE="$candidato"
    break
  fi
done

if [ -z "$EXE" ]; then
  echo "[ERROR] No se encontro CriptoManager.exe."
  echo "        Compilalo primero: build_windows.bat (Windows) o ./build_linux.sh (Linux)."
  exit 1
fi

command -v wine >/dev/null 2>&1 || {
  echo "[ERROR] Falta wine. Instalalo, por ejemplo: sudo apt install wine"
  exit 1
}

echo "[*] Ejecutando: $EXE"
echo "    (los archivos usuarios.txt e historial.txt se crean en la carpeta actual)"
wine "$EXE"