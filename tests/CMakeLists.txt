cmake_minimum_required(VERSION 3.17)

project(neon_test)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#--- GoogleTest
if (CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
  if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(GTest_DIR "/home/zz/artifact/googletest/1.11.0/linux-x64/lib/cmake/GTest" CACHE PATH "包含 GTestConfig.cmake 的目录")
  elseif(ANDROID)
    set(GTest_DIR "/home/zz/artifact/googletest/1.11.0/android-arm64/lib/cmake/GTest" CACHE PATH "包含 GTestConfig.cmake 的目录")
  endif()
elseif (CMAKE_HOST_SYSTEM_NAME MATCHES "Darwin")
  if (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    set(GTest_DIR "/Users/chris/soft/googletest/osx/lib/cmake/GTest")
  elseif (ANDROID)
    set(GTest_DIR "/Users/chris/soft/googletest/android-armv8/lib/cmake/GTest")
  endif()
endif()

find_package(GTest REQUIRED)

enable_testing() # `ctest` or `make test` or `ctest -VV` or `ctest --output-on-failure`
macro(pixel_add_test name)
    add_executable(test_${name} test_${name}.cpp)
    target_link_libraries(test_${name} PRIVATE ${ARGN} GTest::gtest)
    gtest_add_tests(TARGET test_${name})
endmacro()

pixel_add_test(shift_right)
pixel_add_test(arithmetic)
pixel_add_test(load_store)
pixel_add_test(arrays)
pixel_add_test(max_and_min)
pixel_add_test(conditionals)
pixel_add_test(register_data_rearrange)