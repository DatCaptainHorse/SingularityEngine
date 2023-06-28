# Compiler checks and defines

# Check if GCC
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  add_compile_definitions(SE_COMPILER_GCC)
endif()

# Check if Clang
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_definitions(SE_COMPILER_CLANG)
endif()

# Check if MSVC
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  add_compile_definitions(SE_COMPILER_MSVC)
endif()
