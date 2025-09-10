cmake_minimum_required(VERSION 3.21)
project(st2-wallpaper-geode-main LANGUAGES CXX)

# C++17 ist am stabilsten für iOS/Geode
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Geode SDK (kommt vom Workflow als env GEODE_SDK)
if (NOT DEFINED ENV{GEODE_SDK})
  message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
endif()
add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

# Nur die aktuelle Quelle bauen (ohne main_old.cpp)
set(MOD_SOURCES
  src/main.cpp
)

# Geode-Target anlegen (statt add_library/setup_geode_mod)
geode_add_mod(st2-wallpaper-geode-main
  SOURCES ${MOD_SOURCES}
)

# MSVC-Runtime dynamisch (falls du später Win baust)
if (MSVC)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
endif()
