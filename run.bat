@echo off
echo ========================================
echo Trip Management System - Quick Run
echo ========================================
echo.

cd /d "%~dp0\simpleQtApp"

echo Looking for executable...
if exist debug\simpleQtApp.exe (
    echo Found debug version, launching...
    debug\simpleQtApp.exe
) else if exist release\simpleQtApp.exe (
    echo Found release version, launching...
    release\simpleQtApp.exe
) else (
    echo ERROR: No executable found!
    echo Please run build.bat first to compile the application.
    echo.
    pause
    exit /b 1
)
