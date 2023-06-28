cmake_minimum_required(VERSION 3.26)

# Specify project
project(SingularityEngine
  VERSION 0.0.0
  LANGUAGES CXX
)

# Options
option(SE_BUILD_TESTS "Build Singularity Engine tests" ON)
option(SE_BUILD_BENCHMARKS "Build Singularity Engine benchmarks" ON)

# C++20 modules support
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP 1)
set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "2182bf5c-ef0d-489a-91da-49dbc3090d2a")

# Set C++20 as standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# TODO: Add other compilers (GCC, Clang)
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  add_compile_options(/experimental:module)
endif()

# Include support scripts
include(CMake/CompilerChecks.cmake)

# External dependencies
add_subdirectory(External/fmt)

# Engine sources
add_subdirectory(Source)

# Tests
if(SE_BUILD_TESTS)
  add_subdirectory(Tests)
endif()

# Benchmarks
if(SE_BUILD_BENCHMARKS)
  add_subdirectory(Benchmarks)
endif()