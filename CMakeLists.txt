cmake_minimum_required(VERSION 3.2)
project(CppPython CXX C)

if (NOT MSVC)
    message(FATAL_ERROR "Project only designed to work with MSVC on Windows - other platforms coming soon")
endif()

set(BUILD_OUTPUT_DIR ${BUILD_OUTPUT_PATH}/${CMAKE_CFG_INTDIR})
set(EXECUTABLE_OUTPUT_PATH ${BUILD_OUTPUT_PATH})
set(LIBRARY_OUTPUT_PATH ${EXECUTABLE_OUTPUT_PATH})

set(CMAKE_CONFIGURATION_TYPES "Debug;RelWithDebInfo" CACHE STRING "limited configs" FORCE)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_definitions(-DWIN32_LEAN_AND_MEAN)
add_definitions(-DNOMINMAX)

SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_EXTENSIONS TRUE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX")

add_subdirectory(src)
