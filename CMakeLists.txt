cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(SunixWallpaper VERSION 1.0.0)

# Alle Source-Dateien einsammeln
file(GLOB_RECURSE MOD_SOURCES CONFIGURE_DEPENDS src/*.cpp)

# Geode SDK einbinden
if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

# Hier NICHT add_library(...)!
# Stattdessen direkt Geode-Macro:
setup_geode_mod(${PROJECT_NAME}
    SOURCES ${MOD_SOURCES}
    RESOURCES res
)
