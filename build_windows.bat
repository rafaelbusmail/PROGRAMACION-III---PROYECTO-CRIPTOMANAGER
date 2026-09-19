@echo off
REM ============================================================
REM  CriptoManager - Script de compilacion (Windows)
REM  Genera build\Release\CriptoManager.exe (autocontenido)
REM  Requisitos: CMake + MinGW (los busca en C:\Qt\Tools o en PATH)
REM ============================================================
setlocal enabledelayedexpansion

set "ROOT=%~dp0"
REM quitar la barra final de %~dp0 (si no, \" rompe las comillas de los argumentos)
if "%ROOT:~-1%"=="\" set "ROOT=%ROOT:~0,-1%"
set "BUILD_DIR=%ROOT%\build\Release"

REM --- localizar CMake (prioridad: el que trae Qt, luego PATH) ---
set "CMAKE_EXE=cmake"
if exist "C:\Qt\Tools\CMake_64\bin\cmake.exe" set "CMAKE_EXE=C:\Qt\Tools\CMake_64\bin\cmake.exe"

REM --- localizar MinGW (prioridad: el que trae Qt, luego PATH) ---
set "MINGW_BIN="
for %%D in ("C:\Qt\Tools\mingw1310_64\bin" "C:\Qt\Tools\mingw_64\bin" "C:\mingw64\bin" "C:\msys64\mingw64\bin") do (
    if exist "%%~D\g++.exe" set "MINGW_BIN=%%~D"
)

if defined MINGW_BIN (
    set "PATH=%MINGW_BIN%;%PATH%"
) else (
    where g++ >nul 2>nul
    if errorlevel 1 (
        echo [ERROR] No se encontro MinGW ^(g++^). Instalalo o usa el que trae Qt.
        exit /b 1
    )
)

echo [*] CMake : %CMAKE_EXE%
echo [*] MinGW : %MINGW_BIN%
echo [*] Build : %BUILD_DIR%
echo.

REM --- configurar y compilar ---
"%CMAKE_EXE%" -S "%ROOT%" -B "%BUILD_DIR%" -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=g++
if errorlevel 1 (
    echo [ERROR] Fallo la configuracion de CMake.
    exit /b 1
)

"%CMAKE_EXE%" --build "%BUILD_DIR%" --config Release
if errorlevel 1 (
    echo [ERROR] Fallo la compilacion.
    exit /b 1
)

echo.
echo [OK] Ejecutable generado: %BUILD_DIR%\CriptoManager.exe
echo     - Windows: copialo a cualquier PC y ejecutalo directo (no necesita DLLs extra).
echo     - Linux  : correlo con Wine:  wine CriptoManager.exe
endlocal