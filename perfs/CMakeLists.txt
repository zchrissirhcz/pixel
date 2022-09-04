macro(pixel_add_perf name)
  add_executable(perf_${name} ${name}_perf.cpp)
  target_link_libraries(perf_${name} pixel ${OpenCV_LIBS} benchmark::benchmark)
endmacro()


pixel_add_perf(rgb2gray)
pixel_add_perf(rgb2bgr)
pixel_add_perf(flip)