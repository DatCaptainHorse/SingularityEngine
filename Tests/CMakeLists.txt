find_package(Catch2 CONFIG REQUIRED)

add_executable(Tests)

target_sources(Tests PRIVATE
  test.cpp
  maths.cpp
  system.cpp
  utilities.cpp
)

target_link_libraries(Tests PRIVATE
  SE::SE
  Catch2::Catch2WithMain
)