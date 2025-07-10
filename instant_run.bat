@echo off
REM ==================================================
REM FASTEST Build & Run - Use for development only
REM ==================================================

REM Set environment in one line
set "PATH=C:\msys64\mingw64\bin;%PATH%" && cd simpleQtApp

REM Check if we need to regenerate Makefile
if not exist "Makefile" qmake simpleQtApp.pro CONFIG+=debug

REM Ultra-fast parallel build with minimal output
mingw32-make debug -j8 --silent -O

REM Instant run (no echo, no checks)
cd debug && simpleQtApp.exe
