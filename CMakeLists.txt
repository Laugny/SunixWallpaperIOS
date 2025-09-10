cmake_minimum_required(VERSION 3.21)
project(SunixWallpaper LANGUAGES CXX)

# Stabil über alle Targets
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# iOS: nur arm64 (Xcode/iPhoneOS)
if("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS" OR IOS)
  set(CMAKE_OSX_ARCHITECTURES "arm64")
endif()

# --- Geode SDK einbinden (robuste Suche nach geode.cmake) ---
if (NOT DEFINED ENV{GEODE_SDK})
  message(FATAL_ERROR "GEODE_SDK ist nicht gesetzt (kommt vom CI/Runner).")
endif()
set(_GEODE_ROOT "$ENV{GEODE_SDK}")

# Versuche alle gängigen Orte
find_path(_GEODE_CMAKE_DIR
  geode.cmake
  HINTS
    "${_GEODE_ROOT}"
  PATH_SUFFIXES
    cmake
    geode/cmake
    geode-sdk/cmake       # falls geclontes SDK noch einen Unterordner hat
    geode-sdk/geode/cmake
  NO_DEFAULT_PATH
)

if (NOT _GEODE_CMAKE_DIR)
  message(FATAL_ERROR
    "Konnte geode.cmake nicht finden unter:\n"
    "  ${_GEODE_ROOT}/cmake/geode.cmake\n"
    "  ${_GEODE_ROOT}/geode/cmake/geode.cmake\n"
    "  ${_GEODE_ROOT}/geode-sdk/cmake/geode.cmake\n"
    "  ${_GEODE_ROOT}/geode-sdk/geode/cmake/geode.cmake\n"
    "GEODE_SDK=${_GEODE_ROOT}")
endif()

include("${_GEODE_CMAKE_DIR}/geode.cmake")
# -------------------------------------------------------------

# Nur die aktive Quelle kompilieren
set(MOD_SOURCES
  src/main.cpp
)

# Geode-Target (Makro kommt aus geode.cmake)
geode_add_mod(SunixWallpaper
  SOURCES ${MOD_SOURCES}
)
