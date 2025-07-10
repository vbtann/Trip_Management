@echo off
REM ==================================================
REM Trip Management System - Quick Build Script
REM ==================================================

echo Quick building Trip Management System...
echo.

REM Set up Qt environment
set "QT_DIR=C:\msys64\mingw64\bin"
set "PATH=%QT_DIR%;%PATH%"

REM Navigate to project directory
cd simpleQtApp

REM Quick incremental build (no clean)
echo Performing incremental build...
mingw32-make debug -j4
if %ERRORLEVEL% neq 0 (
    echo ✗ Quick build failed, trying full rebuild...
    mingw32-make clean
    qmake simpleQtApp.pro CONFIG+=debug
    mingw32-make debug -j4
)

echo ✓ Quick build completed!
echo Starting application...

REM Run immediately after build
cd debug
simpleQtApp.exe
cd ..\..

pause
