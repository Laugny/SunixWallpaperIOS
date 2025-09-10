cmake_minimum_required(VERSION 3.21)

# Spracheinstellungen
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Projektname = exakt die ID aus mod.json (CMake-kompatibel, keine Punkte/Bindestriche!)
project(laugny_sunix_wallpaper VERSION 1.0.0)

# Source-Dateien einsammeln
file(GLOB_RECURSE MOD_SOURCES CONFIGURE_DEPENDS src/*.cpp)

# -------------------------------------------------------------
# Geode SDK einbinden
# -------------------------------------------------------------
# Erwartet, dass die Umgebungsvariable GEODE_SDK gesetzt ist.
# Im Workflow sollte das passieren über:
#   geode sdk install
#   geode sdk install-binaries --platform ios
#   echo "GEODE_SDK=$(geode sdk path)" >> $GITHUB_ENV
# -------------------------------------------------------------
if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode.")
else()
    message(STATUS "Found Geode SDK at: $ENV{GEODE_SDK}")
endif()

# Geode-SDK ins Projekt einbinden
add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

# -------------------------------------------------------------
# Mod definieren
# -------------------------------------------------------------
# setup_geode_mod legt automatisch das CMake-Target an,
# verlinkt die SDK-Libs und integriert Ressourcen.
# -------------------------------------------------------------
setup_geode_mod(${PROJECT_NAME}
    SOURCES   ${MOD_SOURCES}
    RESOURCES res
)
