@echo off
echo Cleaning build files...

REM Remove console app executable
if exist main.exe del main.exe

REM Remove Qt build directory
if exist "simpleQtApp\build" (
    rmdir /s /q "simpleQtApp\build"
    echo Removed simpleQtApp\build directory
)

REM Remove old qmake files if they exist
if exist "simpleQtApp\Makefile" del "simpleQtApp\Makefile"
if exist "simpleQtApp\Makefile.Debug" del "simpleQtApp\Makefile.Debug"
if exist "simpleQtApp\Makefile.Release" del "simpleQtApp\Makefile.Release"
if exist "simpleQtApp\release" rmdir /s /q "simpleQtApp\release"
if exist "simpleQtApp\debug" rmdir /s /q "simpleQtApp\debug"

echo Clean complete!
pause