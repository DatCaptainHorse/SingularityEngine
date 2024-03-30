add_library(SE_Utilities STATIC)
add_library(SE::Utilities ALIAS SE_Utilities)

target_sources(SE_Utilities PUBLIC FILE_SET CXX_MODULES FILES
  filesystem.cppm
  utilities.cppm
  logging.cppm
)
