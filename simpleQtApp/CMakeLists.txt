cmake_minimum_required(VERSION 3.16)
project(simpleQtApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt settings
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/UI
    ${CMAKE_CURRENT_SOURCE_DIR}/Models
    ${CMAKE_CURRENT_SOURCE_DIR}/Managers
)

# Explicitly list sources instead of using GLOB for better dependency tracking
set(UI_SOURCES
    UI/MainWindow.cpp
    UI/AddTripDialog.cpp
    UI/FilterTripDialog.cpp
    UI/ViewTripDialog.cpp
    UI/EditTripDialog.cpp
    UI/AddPersonDialog.cpp
    UI/ManagePeopleDialog.cpp
    UI/EditPeopleDialog.cpp
)

set(MODELS_SOURCES
    Models/Date.cpp
    Models/Gender.cpp
    Models/Host.cpp
    Models/Person.cpp
    Models/Member.cpp
    Models/Status.cpp
    Models/Trip.cpp
    Models/Utility_Functions.cpp
)

set(MANAGERS_SOURCES
    Managers/Observer.cpp
    Managers/TripManager.cpp
    Managers/FileManager.cpp
    Managers/TripFactory.cpp
    Managers/PersonFactory.cpp
    Managers/PersonManager.cpp
)

set(UI_HEADERS
    UI/MainWindow.h
    UI/AddTripDialog.h
    UI/FilterTripDialog.h
    UI/ViewTripDialog.h
    UI/EditTripDialog.h
    UI/AddPersonDialog.h
    UI/ManagePeopleDialog.h
    UI/EditPeopleDialog.h
)

set(MODELS_HEADERS
    Models/header.h
)

set(MANAGERS_HEADERS
    Managers/FileManager.h
    Managers/TripManager.h
    Managers/Observer.h
    Managers/TripFactory.h
    Managers/PersonFactory.h
    Managers/PersonManager.h
)

# Combine all sources
set(SOURCES 
    main.cpp
    ${UI_SOURCES}
    ${MODELS_SOURCES}
    ${MANAGERS_SOURCES}
)

set(HEADERS
    ${UI_HEADERS}
    ${MODELS_HEADERS}
    ${MANAGERS_HEADERS}
)

# Print found files for debugging
message(STATUS "Main source: main.cpp")
message(STATUS "UI Sources: ${UI_SOURCES}")
message(STATUS "Models Sources: ${MODELS_SOURCES}")
message(STATUS "Managers Sources: ${MANAGERS_SOURCES}")

# Check if all source files exist
foreach(SOURCE_FILE ${SOURCES})
    if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_FILE}")
        message(WARNING "Source file not found: ${SOURCE_FILE}")
    endif()
endforeach()

# Create executable
add_executable(simpleQtApp ${SOURCES} ${HEADERS})

# Link Qt libraries
target_link_libraries(simpleQtApp
    Qt6::Core
    Qt6::Widgets
)

# FIX: Set simpler output directory structure
set_target_properties(simpleQtApp PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/debug"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/release"
)

# Enable debug information
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(simpleQtApp PRIVATE DEBUG)
    message(STATUS "Building in Debug mode")
else()
    message(STATUS "Building in Release mode")
endif()

# Add compiler warnings
if(MSVC)
    target_compile_options(simpleQtApp PRIVATE /W4)
else()
    target_compile_options(simpleQtApp PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Print where executable will be created
message(STATUS "Executable will be created in: ${CMAKE_BINARY_DIR}")
