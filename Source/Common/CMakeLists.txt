add_library(SE_Common STATIC)
add_library(SE::Common ALIAS SE_Common)

target_sources(SE_Common PUBLIC FILE_SET CXX_MODULES FILES
  common.cppm
)
