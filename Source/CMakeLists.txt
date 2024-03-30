add_library(SE INTERFACE)
add_library(SE::SE ALIAS SE)

# Engine modules
add_subdirectory(Common)
add_subdirectory(System)
add_subdirectory(Utilities)

target_link_libraries(SE INTERFACE
  SE::Common
  SE::System
  SE::Utilities
)
