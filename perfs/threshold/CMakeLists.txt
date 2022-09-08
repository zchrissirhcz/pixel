add_executable(threshold_perf
  threshold.h
  threshold.c
  threshold_test.cpp
)

target_link_libraries(threshold_perf PUBLIC
  pixel
  benchmark::benchmark
  ${OpenCV_LIBS}
  GTest::gtest
  GTest::gtest_main
)