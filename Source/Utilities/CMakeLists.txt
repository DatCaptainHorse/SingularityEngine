add_library(SE_Utilities STATIC)
add_library(SE::Utilities ALIAS SE_Utilities)

target_sources(SE_Utilities PRIVATE FILE_SET CXX_MODULES TYPE CXX_MODULES FILES
  utilities.cppm
  logging.cppm
  fileutilities.cppm
  dynamicloader.cppm
)
target_link_libraries(SE_Utilities PRIVATE
  SE::Common
  fmt::fmt
)
