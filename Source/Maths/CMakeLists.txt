add_library(SE_Maths STATIC)
add_library(SE::Maths ALIAS SE_Maths)

target_sources(SE_Maths PRIVATE FILE_SET CXX_MODULES TYPE CXX_MODULES FILES
  maths.cppm
  vector.cppm
  misc.cppm
)
target_link_libraries(SE_Maths PRIVATE
  SE::Kernel
)
