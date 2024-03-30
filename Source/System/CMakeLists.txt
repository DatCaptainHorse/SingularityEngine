add_library(SE_System STATIC)
add_library(SE::System ALIAS SE_System)

target_sources(SE_System PUBLIC FILE_SET CXX_MODULES FILES
  system.cppm
  thread.cppm
)
