cmake_minimum_required(VERSION 3.21)

# Projektname = ID aus mod.json (ohne Punkte/Bindestriche, CMake-kompatibel)
project(SunixWallpaperIOS VERSION 1.0.0 LANGUAGES CXX)

# C++-Standard festlegen
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# EXAKT die Quelldateien angeben – keine GLOBs,
# damit keine veralteten Dateien (z. B. main_old.cpp) in den Build gelangen
set(MOD_SOURCES
    src/main.cpp
)

# Das Mod-Binary (shared library) erstellen
add_library(${PROJECT_NAME} SHARED ${MOD_SOURCES})

# Geode-SDK einbinden – Environment-Variable muss gesetzt sein
if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "GEODE_SDK ist nicht gesetzt! Setze die Umgebungsvariable auf das Geode-SDK-Verzeichnis.")
endif()
add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

# iOS: Code Signing komplett abschalten, damit der Build ohne Developer-Team funktioniert
set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED "NO")
set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED "NO")
set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "")
set(CMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "")

# Optional: CLI-Aufrufe in der CI deaktivieren (verhindert mod-fetch in headless Umgebungen)
# set(GEODE_DISABLE_CLI_CALLS ON)

# Mod einrichten und Geode-Node-IDs-Abhängigkeit als extern deklarieren
# so wird die CLI nicht versuchen, sie zu installieren
setup_geode_mod(${PROJECT_NAME}
    EXTERNALS geode.node-ids:1.21.1
)

