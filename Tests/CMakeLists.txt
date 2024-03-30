add_executable(Tests)

target_sources(Tests PRIVATE
  test.cpp
  utilities.cpp
  system.cpp
)

set(SNITCH_DEFINE_MAIN OFF)
CPMAddPackage("gh:snitch-org/snitch@1.2.4")
target_link_libraries(Tests PRIVATE
  SE::SE
  snitch::snitch
)