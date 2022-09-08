# macro(pixel_add_perf name)
#   add_executable(perf_${name} ${name}_perf.cpp)
#   target_link_libraries(perf_${name} pixel ${OpenCV_LIBS} benchmark::benchmark)
# endmacro()


# pixel_add_perf(rgb2gray)
# pixel_add_perf(rgb2bgr)
# pixel_add_perf(flip)


execute_process(
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/assets/sky.jpg ${CMAKE_BINARY_DIR}/
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/assets/river_bank2.jpg ${CMAKE_BINARY_DIR}/
)

add_subdirectory(flip)
add_subdirectory(array_mean)


