add_library(SE_System STATIC)
add_library(SE::System ALIAS SE_System)

target_sources(SE_System PRIVATE FILE_SET CXX_MODULES TYPE CXX_MODULES FILES
  system.cppm
  thread.cppm
  threadpool.cppm
)
