add_library(pixel STATIC
  px_image.c
  px_compare.c
  
  px_rgb2bgr.c
  px_rgb2gray.c
  px_flip.c  

  px_cnn.c
  px_relu.c
  px_sigmoid.c
  px_max_pooling.c
  px_inner_product.c
  px_convolution.c

  px_array_mean.c

  # rgb2gray.h
  # rgb2gray.c

  # rgb2bgr.h
  # rgb2bgr.c

  # flip.h
  # flip.c
)
target_include_directories(pixel PUBLIC
  ${CMAKE_SOURCE_DIR}/include
)

if(ANDROID)
  target_link_libraries(pixel PUBLIC log)
endif()