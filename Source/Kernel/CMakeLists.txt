add_subdirectory(SIMD)

add_library(SE_Kernel STATIC)
add_library(SE::Kernel ALIAS SE_Kernel)

target_sources(SE_Kernel PRIVATE FILE_SET CXX_MODULES TYPE CXX_MODULES FILES
  kernel.cppm
  cpucapabilities.cppm
  enginekernel.cppm
  SIMD/SIMD.cppm
  SIMD/SSE2proto.cppm
  SIMD/SSE42proto.cppm
)
target_link_libraries(SE_Kernel PRIVATE
  SE::Common
  SE::Utilities
)
add_dependencies(SE_Kernel
  SE::SIMD::SSE2
  SE::SIMD::SSE42
)
