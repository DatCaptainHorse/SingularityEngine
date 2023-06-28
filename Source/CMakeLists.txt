add_subdirectory(Common)
add_subdirectory(Kernel)
add_subdirectory(Maths)
add_subdirectory(System)
add_subdirectory(Utilities)

add_library(SE INTERFACE)
add_library(SE::SE ALIAS SE)

target_link_libraries(SE INTERFACE
  SE::Common
  SE::Kernel
  SE::Maths
  SE::System
  SE::Utilities
)
