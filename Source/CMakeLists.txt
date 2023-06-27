add_subdirectory(System)
add_subdirectory(Utilities)

add_library(SE INTERFACE)
add_library(SE::SE ALIAS SE)

target_link_libraries(SE INTERFACE
  SE::System
  SE::Utilities
)
