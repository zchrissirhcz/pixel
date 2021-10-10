cmake_minimum_required(VERSION 3.17)

project(pixel_neon)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include("../overlook.cmake")

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

  arithmetic/vsubw.hpp
  arithmetic/vsubw.cpp

  arithmetic/vsubhn.hpp
  arithmetic/vsubhn.cpp

  arithmetic/vqsub.hpp
  arithmetic/vqsub.cpp

  arithmetic/vhsub.hpp
  arithmetic/vhsub.cpp

  arithmetic/vrsubhn.hpp
  arithmetic/vrsubhn.cpp

  multiply/vmul.hpp
  multiply/vmul.cpp
  multiply/vmul_n.hpp
  multiply/vmul_n.cpp
  multiply/vmul_lane.hpp
  multiply/vmul_lane.cpp

  multiply/vmull.hpp
  multiply/vmull.cpp
  multiply/vmull_n.hpp
  multiply/vmull_n.cpp
  multiply/vmull_lane.hpp
  multiply/vmull_lane.cpp

  multiply/vqdmull.hpp
  multiply/vqdmull.cpp
  multiply/vqdmull_n.hpp
  multiply/vqdmull_n.cpp
  multiply/vqdmull_lane.hpp
  multiply/vqdmull_lane.cpp

  multiply/vqdmulh.hpp
  multiply/vqdmulh.cpp
  multiply/vqdmulh_n.hpp
  multiply/vqdmulh_n.cpp
  multiply/vqdmulh_lane.hpp
  multiply/vqdmulh_lane.cpp

  multiply/vqrdmulh.hpp
  multiply/vqrdmulh.cpp
  multiply/vqrdmulh_n.hpp
  multiply/vqrdmulh_n.cpp
  multiply/vqrdmulh_lane.hpp
  multiply/vqrdmulh_lane.cpp

  multiply/vmla.hpp
  multiply/vmla.cpp
  multiply/vmla_n.hpp
  multiply/vmla_n.cpp
  multiply/vmla_lane.hpp
  multiply/vmla_lane.cpp

  multiply/vmlal.hpp
  multiply/vmlal.cpp
  multiply/vmlal_n.hpp
  multiply/vmlal_n.cpp
  multiply/vmlal_lane.hpp
  multiply/vmlal_lane.cpp

  multiply/vfma.hpp
  multiply/vfma.cpp

  multiply/vqdmlal.hpp
  multiply/vqdmlal.cpp
  multiply/vqdmlal_n.hpp
  multiply/vqdmlal_n.cpp
  multiply/vqdmlal_lane.hpp
  multiply/vqdmlal_lane.cpp

  multiply/vmls.hpp
  multiply/vmls.cpp
  multiply/vmls_n.hpp
  multiply/vmls_n.cpp
  multiply/vmls_lane.hpp
  multiply/vmls_lane.cpp

  multiply/vmlsl.hpp
  multiply/vmlsl.cpp
  multiply/vmlsl_n.hpp
  multiply/vmlsl_n.cpp
  multiply/vmlsl_lane.hpp
  multiply/vmlsl_lane.cpp

  multiply/vfms.hpp
  multiply/vfms.cpp

  multiply/vqdmlsl.hpp
  multiply/vqdmlsl.cpp
  multiply/vqdmlsl_n.hpp
  multiply/vqdmlsl_n.cpp
  multiply/vqdmlsl_lane.hpp
  multiply/vqdmlsl_lane.cpp

  round/vrnd.hpp
  round/vrnd.cpp
  
  round/vrndn.hpp
  round/vrndn.cpp

  round/vrnda.hpp
  round/vrnda.cpp

  round/vrndp.hpp
  round/vrndp.cpp

  round/vrndm.hpp
  round/vrndm.cpp

  logical_and_compare/vceq.hpp
  logical_and_compare/vceq.cpp

  logical_and_compare/vcge.hpp
  logical_and_compare/vcge.cpp

  logical_and_compare/vcle.hpp
  logical_and_compare/vcle.cpp

  logical_and_compare/vcgt.hpp
  logical_and_compare/vcgt.cpp

  logical_and_compare/vclt.hpp
  logical_and_compare/vclt.cpp

  logical_and_compare/vcage.hpp
  logical_and_compare/vcage.cpp

  logical_and_compare/vcale.hpp
  logical_and_compare/vcale.cpp

  logical_and_compare/vcagt.hpp
  logical_and_compare/vcagt.cpp

  logical_and_compare/vcalt.hpp
  logical_and_compare/vcalt.cpp

  logical_and_compare/vtst.hpp
  logical_and_compare/vtst.cpp

  data_processing/vabs.hpp
  data_processing/vabs.cpp

  data_processing/vqabs.hpp
  data_processing/vqabs.cpp

  data_processing/vabd.hpp
  data_processing/vabd.cpp

  data_processing/vabdl.hpp
  data_processing/vabdl.cpp

  data_processing/vaba.hpp
  data_processing/vaba.cpp

  data_processing/vabal.hpp
  data_processing/vabal.cpp

  data_processing/vmax.hpp
  data_processing/vmax.cpp

  data_processing/vpmax.hpp
  data_processing/vpmax.cpp

  data_processing/vmin.hpp
  data_processing/vmin.cpp

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
pixel_add_test(mul)
pixel_add_test(round)
pixel_add_test(logical_and_compare)
pixel_add_test(data_processing)

pixel_add_test(shift_right)