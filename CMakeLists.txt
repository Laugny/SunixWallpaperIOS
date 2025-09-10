cmake_minimum_required(VERSION 3.21)
project(SunixWallpaper LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS" OR IOS)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
endif()

# Geode SDK aus vendor/ laden
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/vendor/geode/CMakeLists.txt")
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vendor/geode" "${CMAKE_CURRENT_BINARY_DIR}/geode")
else()
    message(FATAL_ERROR "vendor/geode not found. Run: git submodule update --init --recursive")
endif()

set(MOD_SOURCES
    src/main.cpp
)

geode_add_mod(SunixWallpaper
    SOURCES ${MOD_SOURCES}
)
