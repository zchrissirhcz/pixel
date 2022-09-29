macro(pixel_add_test name)
  add_executable(${name} ${name}.cpp)
  target_link_libraries(${name} pixel ${OpenCV_LIBS} GTest::gtest GTest::gtest_main)
  if((NOT ANDROID) AND CMAKE_SYSTEM_NAME MATCHES "Linux" AND CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
    gtest_add_tests(TARGET ${name}
      COMMAND ${CMAKE_COMMAND} -DTEST_EXECUTABLE=$<TARGET_FILE:${name}> -P ${CMAKE_SOURCE_DIR}/cmake/qemu_run_test.cmake
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
  else()
    gtest_add_tests(TARGET ${name}
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
  endif()
endmacro()

pixel_add_test(px_rgb2bgr_test)
pixel_add_test(px_rgb2gray_test)
pixel_add_test(px_relu_test)
pixel_add_test(px_flip_test)
pixel_add_test(px_matrix_test)
pixel_add_test(px_sigmoid_test)
pixel_add_test(px_max_pooling_test)
pixel_add_test(px_inner_product_test)
pixel_add_test(px_convolution_test)
pixel_add_test(px_arithm_test)
pixel_add_test(px_compare_test)
pixel_add_test(px_mnist_test)
pixel_add_test(px_border_test)
pixel_add_test(px_copy_make_border_test)
pixel_add_test(px_copy_cut_border_test)

if(NOT CMAKE_CROSSCOMPILING)
  pixel_add_test(px_image_io_test)
  pixel_add_test(px_ppm_pgm_test)
  pixel_add_test(px_bmp_test)
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/assets/mingren.bmp ${CMAKE_BINARY_DIR}/
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/assets/ring.bmp ${CMAKE_BINARY_DIR}/
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/assets/mingren_gray.bmp ${CMAKE_BINARY_DIR}/
)

configure_file(px_directories.h.in
               ${CMAKE_CURRENT_LIST_DIR}/px_directories.h
               @ONLY)

set(mnist_file_lst t10k-images.idx3-ubyte t10k-labels.idx1-ubyte train-images.idx3-ubyte train-labels.idx1-ubyte)
foreach(mnist_file ${mnist_file_lst})
  if(NOT (EXISTS "${CMAKE_SOURCE_DIR}/legacy/NaiveCNN/mnist/${mnist_file}"))
    execute_process(COMMAND bash ${CMAKE_SOURCE_DIR}/legacy/NaiveCNN/mnist/get_data.sh)
    break()
  endif()
endforeach()