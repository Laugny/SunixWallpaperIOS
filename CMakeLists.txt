cmake_minimum_required(VERSION 3.21)
project(SunixWallpaper LANGUAGES CXX)

# Stabil: C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# iOS: nur arm64
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS" OR IOS)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
endif()

# --- Geode SDK einbinden (beide üblichen Pfade checken) ---
if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "GEODE_SDK ist nicht gesetzt – wird vom CI gesetzt/geklont.")
endif()
set(_GEODE_ROOT "$ENV{GEODE_SDK}")

if (EXISTS "${_GEODE_ROOT}/cmake/geode.cmake")
    include("${_GEODE_ROOT}/cmake/geode.cmake")
elseif (EXISTS "${_GEODE_ROOT}/geode/cmake/geode.cmake")
    include("${_GEODE_ROOT}/geode/cmake/geode.cmake")
else()
    message(FATAL_ERROR
      "Konnte geode.cmake nicht finden unter:\n"
      "  ${_GEODE_ROOT}/cmake/geode.cmake\n"
      "  ${_GEODE_ROOT}/geode/cmake/geode.cmake\n"
      "GEODE_SDK=${_GEODE_ROOT}")
endif()
# ----------------------------------------------------------

# Nur die aktive Quelle
set(MOD_SOURCES
    src/main.cpp
)

# Geode-Mod-Target (Makro aus geode.cmake)
geode_add_mod(SunixWallpaper
  SOURCES ${MOD_SOURCES}
)
