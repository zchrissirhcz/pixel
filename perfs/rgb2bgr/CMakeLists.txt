add_executable(rgb2bgr_perf
  rgb2bgr.h
  rgb2bgr.c
  rgb2bgr_test.cpp
)

target_link_libraries(rgb2bgr_perf PUBLIC
  pixel
  benchmark::benchmark
  ${OpenCV_LIBS}
  GTest::gtest
  GTest::gtest_main
)