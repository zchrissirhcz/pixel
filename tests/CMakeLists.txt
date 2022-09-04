macro(pixel_add_test name)
  add_executable(${name} ${name}.cpp)
  target_link_libraries(${name} pixel ${OpenCV_LIBS} GTest::gtest GTest::gtest_main)
  gtest_add_tests(TARGET ${name} WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
endmacro()

pixel_add_test(px_rgb2bgr_test)
pixel_add_test(px_rgb2gray_test)
pixel_add_test(px_relu_test)

pixel_add_test(px_flip_test)
