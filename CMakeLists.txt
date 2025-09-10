cmake_minimum_required(VERSION 3.21)
<<<<<<< HEAD
project(st2-wallpaper-geode-main LANGUAGES CXX)

# C++17 ist am problemlosesten über alle Targets
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Geode-SDK laden (per ENV wie bisher)
=======
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(st2-wallpaper-geode-main VERSION 1.0.0)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_library(${PROJECT_NAME} SHARED ${SOURCES})

>>>>>>> e41d371 (initial)
if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()
<<<<<<< HEAD
add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

# Quellen explizit angeben (verhindert versehentliches Mitbauen von main_old.cpp)
set(MOD_SOURCES
  src/main.cpp
)

# Geode-Ziel erzeugen (statt add_library + setup_geode_mod)
geode_add_mod(st2-wallpaper-geode-main
  SOURCES ${MOD_SOURCES}
)

# MSVC-CRT dynamisch (/MD), vermeidet /MT-Mismatch
if(MSVC)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
endif()
=======

if ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS" OR IOS)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
else()
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

setup_geode_mod(${PROJECT_NAME})
>>>>>>> e41d371 (initial)
