cmake_minimum_required(VERSION 3.21)
project(SunixWallpaper LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Quellen
file(GLOB_RECURSE MOD_SOURCES CONFIGURE_DEPENDS src/*.cpp)

# --- Geode SDK finden ---
if(NOT DEFINED GEODE_SDK AND DEFINED ENV{GEODE_SDK})
    set(GEODE_SDK "$ENV{GEODE_SDK}")
endif()

if(NOT GEODE_SDK OR NOT EXISTS "${GEODE_SDK}/cmake/GeodeFile.cmake")
    message(FATAL_ERROR "GEODE_SDK ist nicht gesetzt oder ungültig: '${GEODE_SDK}'")
endif()

include("${GEODE_SDK}/cmake/GeodeFile.cmake")

# Plattform für iOS
set(GEODE_TARGET_PLATFORM iOS CACHE STRING "Geode target platform")

# Mod definieren
setup_geode_mod(${PROJECT_NAME}
    SOURCES   ${MOD_SOURCES}
    RESOURCES res
)


