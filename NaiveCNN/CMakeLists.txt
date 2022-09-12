add_library(cnn STATIC
  cnn/cnn.c
  cnn/cnn.h
  cnn/mat.c
  cnn/mat.h
)

add_library(naivecnn STATIC
  src/naive_cnn.h
  src/naive_cnn.c

  src/naive_train.c
  src/naive_infer.c

  src/naive_convolution.c
  src/naive_pooling.c
  src/naive_relu.c
)
target_link_libraries(cnn PUBLIC pixel m)
target_link_libraries(naivecnn PUBLIC pixel m)

add_executable(testbed_lenet
  examples/lenet.h
  examples/lenet.cpp
  examples/lenet_load.cpp
  examples/lenet_save.cpp
  examples/lenet_clear.cpp
  examples/lenet_forward.cpp
  examples/lenet_backward.cpp
  examples/lenet_apply_grads.cpp
)
target_include_directories(testbed_lenet PRIVATE "cnn")
target_link_libraries(testbed_lenet cnn ${OpenCV_LIBS})

add_executable(testbed_nc_lenet
  examples/nc_lenet.cpp
)
target_include_directories(testbed_nc_lenet PRIVATE "src")
target_link_libraries(testbed_nc_lenet naivecnn ${OpenCV_LIBS})