add_library(cnn STATIC
  src/cnn.c
  src/cnn.h
  src/mat.c
  src/mat.h
  
  src/naive_cnn.h
  src/naive_cnn.c

  src/naive_train.c
  src/naive_infer.c

  src/naive_convolution.c
  src/naive_pooling.c
  src/naive_relu.c
)
target_link_libraries(cnn PUBLIC pixel m)

add_executable(testbed_lenet
  examples/lenet.h
  examples/lenet.cpp
  examples/lenet_load.cpp
  examples/lenet_save.cpp
  examples/lenet_clear.cpp
  examples/lenet_forward.cpp
)
target_include_directories(testbed_lenet PRIVATE "src")
target_link_libraries(testbed_lenet cnn ${OpenCV_LIBS})

add_executable(testbed_nc_lenet
  examples/nc_lenet.cpp
)
target_include_directories(testbed_nc_lenet PRIVATE "src")
target_link_libraries(testbed_nc_lenet cnn ${OpenCV_LIBS})