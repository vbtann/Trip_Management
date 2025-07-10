QT += core widgets gui

CONFIG += c++17 debug_and_release

# Speed optimizations for development
CONFIG += precompile_header
CONFIG += ccache  # If ccache is available

TARGET = simpleQtApp
TEMPLATE = app

INCLUDEPATH += UI Models Managers

# Enable parallel compilation
QMAKE_CXXFLAGS += -j4

# Precompiled header for faster compilation
PRECOMPILED_HEADER = Models/header.h

# Main application file
SOURCES += main.cpp

# UI files
SOURCES += UI/MainWindow.cpp \
    UI/AddTripDialog.cpp

# Model files  
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

# Header files
HEADERS += UI/MainWindow.h \
    UI/AddTripDialog.h \
    Models/header.h \
    Managers/FileManager.h

# Compiler definitions
DEFINES += QT_DEPRECATED_WARNINGS

# Speed up compilation
QMAKE_CXXFLAGS_DEBUG += -O1  # Light optimization for debug builds

# Debug configuration
CONFIG(debug, debug|release) {
    DESTDIR = debug
    TARGET = simpleQtApp
    # Faster debug linking
    QMAKE_LFLAGS_DEBUG += -Wl,--no-keep-memory
}

# Release configuration  
CONFIG(release, debug|release) {
    DESTDIR = release
    TARGET = simpleQtApp
}
