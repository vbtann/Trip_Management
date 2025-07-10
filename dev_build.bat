@echo off
REM ==================================================
REM Development Build - Fast compilation for development
REM ==================================================

echo Development build starting...

REM Set environment quickly
set "PATH=C:\msys64\mingw64\bin;%PATH%"

REM Navigate and build
cd simpleQtApp

REM Check if Makefile exists, if not create it
if not exist "Makefile" (
    echo Generating Makefile...
    qmake simpleQtApp.pro CONFIG+=debug
)

REM Fast parallel build
mingw32-make debug -j4 --silent

REM Quick run without copying plugins (assumes they exist)
cd debug
echo Starting app...
simpleQtApp.exe
cd ..\..
