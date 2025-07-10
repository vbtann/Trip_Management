@echo off
setlocal enabledelayedexpansion

echo ================================
echo Building Trip Management System
echo ================================

echo.
echo [1/8] Setting up environment...

REM Add Qt to PATH for this session
set "QT_DIR=C:\msys64\mingw64\bin"
if exist "%QT_DIR%" (
    echo Adding Qt to PATH: %QT_DIR%
    set "PATH=%QT_DIR%;%PATH%"
    echo ✓ Qt added to PATH for this session
) else (
    echo Warning: Qt directory not found at %QT_DIR%
    echo Please check your MSYS2/Qt installation path
)

REM Check if Qt is in PATH
where qmake >nul 2>&1
if %ERRORLEVEL% neq 0 (
    echo ERROR: qmake not found in PATH!
    echo Please check your Qt installation.
    pause
    exit /b 1
)

REM Check if simpleQtApp directory exists
if not exist "simpleQtApp" (
    echo ERROR: simpleQtApp directory not found!
    echo Current directory: %CD%
    pause
    exit /b 1
)

echo [2/8] Entering project directory...
cd simpleQtApp

echo [3/8] Cleaning previous build...
if exist "debug" rmdir /s /q "debug" >nul 2>&1
if exist "release" rmdir /s /q "release" >nul 2>&1
if exist "Makefile*" del "Makefile*" >nul 2>&1

echo [4/8] Generating Makefile with qmake...
qmake simpleQtApp.pro CONFIG+=debug
if %ERRORLEVEL% neq 0 (
    echo ERROR: qmake failed!
    cd ..
    pause
    exit /b 1
)

echo [5/8] Building project with make...
mingw32-make
if %ERRORLEVEL% neq 0 (
    echo ERROR: Build failed!
    cd ..
    pause
    exit /b 1
)

echo [6/8] Setting up Qt platform plugins...
set "QT_PLUGINS=C:\msys64\mingw64\share\qt6\plugins"
if exist "%QT_PLUGINS%\platforms" (
    if not exist "debug\platforms" mkdir "debug\platforms"
    copy "%QT_PLUGINS%\platforms\qwindows.dll" "debug\platforms\" >nul 2>&1
    echo ✓ Platform plugins copied
) else (
    echo Warning: Platform plugins not found
)

echo [7/8] Build completed successfully!

echo.
echo [8/8] Testing application...

if exist "debug\simpleQtApp.exe" (
    echo Found executable: debug\simpleQtApp.exe
    echo Testing application startup...
    
    cd debug
    echo Current directory: %CD%
    echo Running application (console mode)...
    
    REM Run in console mode first to see any errors
    simpleQtApp.exe
    set "EXIT_CODE=%ERRORLEVEL%"
    
    echo Application finished with exit code: %EXIT_CODE%
    
    if %EXIT_CODE% equ 0 (
        echo ✓ Application ran successfully!
    ) else (
        echo ✗ Application failed or crashed (exit code: %EXIT_CODE%)
        echo.
        echo Troubleshooting steps:
        echo 1. Check if all Qt DLLs are available
        echo 2. Check if platform plugins exist
        echo 3. Check application console output above
    )
    
    cd ..
) else (
    echo ERROR: Executable not found!
)

cd ..
echo.
echo Build process completed!
pause