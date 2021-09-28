cmake_minimum_required(VERSION 3.17)

project(pixel_neon)

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

add_library(pixel_neon
  pixel_neon_base.hpp
  pixel_neon.hpp

  arithmetic/vadd.hpp
  arithmetic/vadd.cpp

  arithmetic/vaddl.hpp
  arithmetic/vaddl.cpp

  arithmetic/vaddw.hpp
  arithmetic/vaddw.cpp

  arithmetic/vaddhn.hpp
  arithmetic/vaddhn.cpp

  arithmetic/vqadd.hpp
  arithmetic/vqadd.cpp

  arithmetic/vhadd.hpp
  arithmetic/vhadd.cpp

  arithmetic/vrhadd.hpp
  arithmetic/vrhadd.cpp

  arithmetic/vsub.hpp
  arithmetic/vsub.cpp

  arithmetic/vsubl.hpp
  arithmetic/vsubl.cpp

  misc/vdup_n.hpp
  misc/vdup_n.cpp
)

target_include_directories(pixel_neon PUBLIC
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/arithmetic
  ${CMAKE_SOURCE_DIR}/data_processing
  ${CMAKE_SOURCE_DIR}/misc
)

find_package(GTest REQUIRED)

enable_testing() # `ctest` or `make test` or `ctest -VV` or `ctest --output-on-failure`
macro(pixel_add_test name)
    add_executable(test_${name} tests/test_${name}.cpp)
    target_link_libraries(test_${name} PRIVATE pixel_neon ${ARGN} GTest::gtest)
    gtest_add_tests(TARGET test_${name})
endmacro()


# pixel_add_test(arithmetic)
# pixel_add_test(load_store)
# pixel_add_test(arrays)
# pixel_add_test(max_and_min)
# pixel_add_test(conditionals)

# based on https://github.com/rogerou/Arm-neon-intrinsics
pixel_add_test(register_data_rearrange)
pixel_add_test(add)
pixel_add_test(sub)

pixel_add_test(shift_right)