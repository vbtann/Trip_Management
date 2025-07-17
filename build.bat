@echo off
echo ========================================
echo Trip Management System - Build Script
echo ========================================
echo.

cd /d "%~dp0\simpleQtApp"

echo Checking Qt installation...
where qmake >nul 2>&1
if %errorlevel% neq 0 (
    echo ERROR: qmake not found! Please install Qt and add it to PATH
    echo.
    echo Download Qt from: https://www.qt.io/download-qt-installer
    echo Make sure to add Qt's bin directory to your system PATH
    pause
    exit /b 1
)

echo Qt found! Building project...
echo.

echo Cleaning previous build...
if exist Makefile del Makefile
if exist Makefile.Debug del Makefile.Debug
if exist Makefile.Release del Makefile.Release
if exist .qmake.stash del .qmake.stash
if exist debug rmdir /s /q debug
if exist release rmdir /s /q release

echo Generating Makefile with qmake...
qmake simpleQtApp.pro
if %errorlevel% neq 0 (
    echo ERROR: qmake failed!
    pause
    exit /b 1
)

echo Building application...
where mingw32-make >nul 2>&1
if %errorlevel% equ 0 (
    mingw32-make
) else (
    where nmake >nul 2>&1
    if %errorlevel% equ 0 (
        nmake
    ) else (
        where make >nul 2>&1
        if %errorlevel% equ 0 (
            make
        ) else (
            echo ERROR: No build tool found!
            echo Please ensure you have one of the following:
            echo   - mingw32-make (comes with Qt MinGW)
            echo   - nmake (comes with Visual Studio)
            echo   - make (from MSYS2 or other tools)
            echo.
            echo Try opening this project in Qt Creator instead.
            pause
            exit /b 1
        )
    )
)
if %errorlevel% neq 0 (
    echo ERROR: Build failed!
    pause
    exit /b 1
)
if %errorlevel% neq 0 (
    echo ERROR: Build failed!
    pause
    exit /b 1
)

echo.
echo ========================================
echo BUILD SUCCESSFUL!
echo ========================================
echo.
echo Executable location:
if exist debug\simpleQtApp.exe (
    echo   debug\simpleQtApp.exe
) else if exist release\simpleQtApp.exe (
    echo   release\simpleQtApp.exe
) else (
    echo   Build completed but executable not found
)

echo.
echo You can now run the application!
pause
