add_executable(flip_perf
  flip.h
  flip.c
  flip_perf.cpp
)

target_link_libraries(flip_perf PUBLIC
  pixel
  benchmark::benchmark
  ${OpenCV_LIBS}
)