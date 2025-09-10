cmake_minimum_required(VERSION 3.21)
project(SunixWallpaper LANGUAGES CXX)

# C++17 ist der stabile Standard für Geode
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# iOS: nur arm64 (aus den Geode iOS-Docs)
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS" OR IOS)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
else()
    # für Desktop-Builds (optional)
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
endif()

# Geode-CMake laden (stellt geode_add_mod bereit)
if (DEFINED ENV{GEODE_SDK})
    include("$ENV{GEODE_SDK}/cmake/geode.cmake")
else()
    message(FATAL_ERROR "GEODE_SDK ist nicht gesetzt – Geode SDK fehlt im Build-Environment")
endif()

# Nur deine aktive Quelle einbinden
set(MOD_SOURCES
    src/main.cpp
)

# Mod-Target anlegen (Makro kommt aus geode.cmake)
geode_add_mod(SunixWallpaper
    SOURCES ${MOD_SOURCES}
)
