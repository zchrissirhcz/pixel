macro(pixel_add_test name)
  add_executable(${name} ${name}.cpp)
  target_link_libraries(${name} pixel ${OpenCV_LIBS} GTest::gtest GTest::gtest_main)
  if((NOT ANDROID) AND CMAKE_SYSTEM_NAME MATCHES "Linux" AND CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
    gtest_add_tests(TARGET ${name}
      COMMAND ${CMAKE_COMMAND} -DTEST_EXECUTABLE=$<TARGET_FILE:${name}> -P ${CMAKE_SOURCE_DIR}/cmake/qemu_run_test.cmake
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
  else()
    gtest_add_tests(TARGET test_${name}
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
pixel_add_test(px_bmp_test)
pixel_add_test(px_mnist_test)
pixel_add_test(px_image_io_test)

execute_process(
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/assets/mingren.bmp ${CMAKE_BINARY_DIR}/
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/assets/ring.bmp ${CMAKE_BINARY_DIR}/
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/assets/mingren_gray.bmp ${CMAKE_BINARY_DIR}/
)