cmake_minimum_required(VERSION 3.29)

project(SingularityEngine
  VERSION 0.0.0
  LANGUAGES CXX
)

# CXX options
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

# Set linker to mold if it's available
find_program(MOLD_LINKER mold)
if (MOLD_LINKER)
  set(CMAKE_LINKER_TYPE MOLD)
  message(STATUS "Using mold linker!")
endif ()

# Include CPM
include(CMake/CPM.cmake)

# Subdirectories
add_subdirectory(Source)
add_subdirectory(Tests)