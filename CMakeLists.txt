cmake_minimum_required(VERSION 3.21)
project(SunixWallpaperIOS LANGUAGES C CXX)

# GEODE_SDK aus -DGEODE_SDK oder ENV übernehmen
if(NOT DEFINED GEODE_SDK AND DEFINED ENV{GEODE_SDK})
    set(GEODE_SDK "$ENV{GEODE_SDK}")
endif()

if(NOT GEODE_SDK OR NOT EXISTS "${GEODE_SDK}/cmake/geode.cmake")
    message(FATAL_ERROR
      "GEODE_SDK ist nicht gesetzt oder ungültig: '${GEODE_SDK}'. "
      "Setze GEODE_SDK auf den Geode-SDK-Pfad (geode sdk install / set-path).")
endif()

include("${GEODE_SDK}/cmake/geode.cmake")

# iOS als Zielplattform (Groß-/Kleinschreibung beachten)
set(GEODE_TARGET_PLATFORM iOS CACHE STRING "Geode target platform")

# Quellen & Ressourcen
set(MOD_SOURCES
    src/main.cpp
)
set(MOD_RESOURCES
    res
)

# Mod bauen
geode_add_mod(
    SOURCES   ${MOD_SOURCES}
    RESOURCES ${MOD_RESOURCES}
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
