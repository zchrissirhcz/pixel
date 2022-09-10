add_library(pixel STATIC
  px_image.c
  px_compare.c
  px_arithm.c
  px_timer.c
  px_autotimer.cpp
  
  px_rgb2bgr.c
  px_rgb2gray.c
  px_flip.c
  px_histogram.c
  px_resize_linear.c
  px_resize_nearest.c
  px_rgb_to_nv21.cpp
  px_threshold.c

  px_cnn.c
  px_relu.c
  px_sigmoid.c
  px_max_pooling.c
  px_inner_product.c
  px_convolution.c
  px_eltwise.c

  px_array_mean.c
)
target_include_directories(pixel PUBLIC
  ${CMAKE_SOURCE_DIR}/include
)

if(ANDROID)
  target_link_libraries(pixel PUBLIC log)
endif()