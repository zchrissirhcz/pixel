add_library(pixel STATIC
  px_image.c
  px_compare.c
  px_arithm.c
  px_timer.c
  px_autotimer.cpp
  px_typebase.c
  px_bmp.c
  px_mnist.c
  px_filesystem.c
  px_image_io.c
  px_ppm_pgm.c
  px_resize.c
  px_image_drawing.cpp
  px_image_rotate.c
  
  px_rgb_to_bgr.c
  px_rgb_to_gray.c
  px_flip.c
  px_histogram.c
  px_resize_linear.c
  px_resize_nearest.c
  px_rgb_to_nv21.cpp
  px_nv21_to_rgb.cpp
  px_threshold.c
  px_border.c
  px_copy_make_border.c
  px_copy_cut_border.c
  improc_zcx.cpp

  px_cnn.c
  px_relu.c
  px_sigmoid.c
  px_tanh.c
  px_max_pooling.c
  px_inner_product.c
  px_convolution.c
  px_eltwise.c

  px_array_mean.c
  px_digits.c
)
target_include_directories(pixel PUBLIC
  ${CMAKE_SOURCE_DIR}/include
)

if(ANDROID)
  target_link_libraries(pixel PUBLIC log)
endif()