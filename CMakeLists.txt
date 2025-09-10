cmake_minimum_required(VERSION 3.21)
project(SunixWallpaperIOS LANGUAGES C CXX)

# -------- Geode SDK einbinden --------
# GEODE_SDK aus Umgebung übernehmen, falls nicht als -DGEODE_SDK=... übergeben
if(NOT DEFINED GEODE_SDK AND DEFINED ENV{GEODE_SDK})
    set(GEODE_SDK "$ENV{GEODE_SDK}")
endif()

if(NOT GEODE_SDK OR NOT EXISTS "${GEODE_SDK}/cmake/geode.cmake")
    message(FATAL_ERROR
      "GEODE_SDK ist nicht gesetzt oder ungültig: '${GEODE_SDK}'. "
      "Setze GEODE_SDK auf den Geode-SDK-Pfad (geode sdk install / set-path).")
endif()

# Geode-CMake-Funktionen laden (definiert u.a. geode_add_mod)
include("${GEODE_SDK}/cmake/geode.cmake")

# -------- Plattform-Target (wichtig für iOS) --------
# Groß-/Kleinschreibung beachten: iOS
set(GEODE_TARGET_PLATFORM iOS CACHE STRING "Geode target platform")

# -------- Dein Mod bauen --------
# Quellen & Ressourcen anpassen wie in deinem Repo
set(MOD_SOURCES
    src/main.cpp
    # ggf. weitere .cpps hinzufügen
)

# Falls du einen res/ Ordner hast, gib ihn hier an
set(MOD_RESOURCES
    res
)

# Das eigentliche Mod-Target
geode_add_mod(
    SOURCES     ${MOD_SOURCES}
    RESOURCES   ${MOD_RESOURCES}
    # LINKS       <weitere libs falls nötig>
    # INCLUDES    <zusätzliche include-dirs>
)

# Optional, aber praktisch
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
