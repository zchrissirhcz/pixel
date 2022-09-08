add_executable(histogram_perf
  histogram.h
  histogram.c
  histogram_test.cpp
)

target_link_libraries(histogram_perf PUBLIC
  pixel
  benchmark::benchmark
  ${OpenCV_LIBS}
)