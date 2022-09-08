add_executable(array_mean_perf
  array_mean.h
  array_mean.c
  array_mean_test.cpp
)

target_link_libraries(array_mean_perf PUBLIC
  pixel
  benchmark::benchmark
  ${OpenCV_LIBS}
)