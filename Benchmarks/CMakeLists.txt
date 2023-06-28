find_package(benchmark CONFIG REQUIRED)

add_executable(Benchmarks)

target_sources(Benchmarks PRIVATE
  benchmark.cpp
  maths.cpp
)
target_link_libraries(Benchmarks PRIVATE
  SE::SE
  benchmark::benchmark
)
add_custom_command(TARGET Benchmarks POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/sesimdsse2.dll"
  "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/sesimdsse42.dll"
  $<TARGET_FILE_DIR:Benchmarks>
)