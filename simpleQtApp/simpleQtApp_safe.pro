QT += core widgets gui

CONFIG += c++17 debug_and_release

TARGET = simpleQtApp_safe
TEMPLATE = app

INCLUDEPATH += UI Models Managers

# Main application file (safe version)
SOURCES += main_safe.cpp

# Model files only (no UI to avoid MainWindow issues)
SOURCES += Models/Date.cpp \
    Models/Gender.cpp \
    Models/Host.cpp \
    Models/Human.cpp \
    Models/Member.cpp \
    Models/Status.cpp \
    Models/Trip.cpp \
    Models/Utility_Functions.cpp

# Manager files
SOURCES += Managers/FileManager.cpp

# Header files (no UI headers)
HEADERS += Models/header.h \
    Managers/FileManager.h

# Compiler definitions
DEFINES += QT_DEPRECATED_WARNINGS

# Debug configuration
CONFIG(debug, debug|release) {
    DESTDIR = debug
    TARGET = simpleQtApp_safe
}

# Release configuration  
CONFIG(release, debug|release) {
    DESTDIR = release
    TARGET = simpleQtApp_safe
}
