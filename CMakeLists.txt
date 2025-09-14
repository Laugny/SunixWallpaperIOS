cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Projektname = ID aus mod.json (SunixWallpaperIOS)
project(SunixWallpaperIOS VERSION 1.0.0)

# Quellcode einsammeln
file(GLOB_RECURSE MOD_SOURCES CONFIGURE_DEPENDS src/*.cpp)

# Das Mod‑Binary erstellen
add_library(${PROJECT_NAME} SHARED ${MOD_SOURCES})

# Geode SDK einbinden
if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "GEODE_SDK nicht gesetzt")
endif()
add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

set(GEODE_DISABLE_CLI_CALLS ON)

# Mod mit Geode verlinken und Ressourcen packen
setup_geode_mod(${PROJECT_NAME})

