@echo off
REM ==================================================
REM Trip Management System - Build Script
REM ==================================================

echo Building Trip Management System...
echo.

REM Set up Qt environment
set "QT_DIR=C:\msys64\mingw64\bin"
if exist "%QT_DIR%" (
    set "PATH=%QT_DIR%;%PATH%"
    echo ✓ Qt environment configured
) else (
    echo ✗ Warning: Qt directory not found at %QT_DIR%
    echo Please check your Qt installation path
)

REM Navigate to project directory
if not exist "simpleQtApp" (
    echo ✗ Error: simpleQtApp directory not found!
    pause
    exit /b 1
)

cd simpleQtApp

REM Clean previous build
echo Cleaning previous build...
if exist "debug" rmdir /s /q "debug" >nul 2>&1
if exist "release" rmdir /s /q "release" >nul 2>&1
if exist "Makefile*" del "Makefile*" >nul 2>&1

REM Generate Makefile
echo Generating Makefile...
qmake simpleQtApp.pro CONFIG+=debug
if %ERRORLEVEL% neq 0 (
    echo ✗ Error: qmake failed!
    cd ..
    pause
    exit /b 1
)

REM Build project with parallel compilation
echo Building project with parallel compilation...
mingw32-make debug -j4
if %ERRORLEVEL% neq 0 (
    echo ✗ Error: Build failed!
    cd ..
    pause
    exit /b 1
)

REM Copy platform plugins
echo Setting up platform plugins...
if not exist "debug\platforms" mkdir "debug\platforms"
if exist "%QT_DIR%\..\plugins\platforms\qwindows.dll" (
    copy "%QT_DIR%\..\plugins\platforms\qwindows.dll" "debug\platforms\" >nul
    echo ✓ Platform plugins copied
) else (
    echo ⚠ Warning: Platform plugins not found
)

cd ..

echo.
echo ================================================== 
echo ✓ Build completed successfully!
echo ==================================================
echo.
echo To run the application:
echo   1. Navigate to: simpleQtApp\debug\
echo   2. Run: simpleQtApp.exe
echo.
echo Or use the run script: run.bat
echo ==================================================