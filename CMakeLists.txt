cmake_minimum_required(VERSION 3.26)

project(SingularityEngine
  VERSION 0.0.0
  LANGUAGES CXX
)

set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP 1)
set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "2182bf5c-ef0d-489a-91da-49dbc3090d2a")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# TODO: Add other compilers (GCC, Clang)
if(MSVC)
  add_compile_options(/experimental:module)
endif()

add_subdirectory(Source)
add_subdirectory(Tests)