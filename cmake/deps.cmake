if(DEFINED ENV{ARTIFACTS_DIR})
  set(ARTIFACTS_DIR "$ENV{ARTIFACTS_DIR}")
else()
  message(WARNING "ARTIFACTS_DIR env var not defined, abort")
endif()

# googletest
if(ANDROID)
  set(GTest_DIR "${ARTIFACTS_DIR}/googletest/1.11.0/android-arm64/lib/cmake/GTest" CACHE PATH "Directory that contains GTestConfig.cmake")
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
  set(GTest_DIR "${ARTIFACTS_DIR}/googletest/1.11.0/linux-x64/lib/cmake/GTest" CACHE PATH "Directory that contains GTestConfig.cmake")
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
  set(GTest_DIR "${ARTIFACTS_DIR}/googletest/1.11.0/windows-x64/lib/cmake/GTest" CACHE PATH "Directory that contains GTestConfig.cmake")
else()
  message(WARNING "GTest_DIR not set yet")
endif()
message(STATUS ">>> GTest_DIR is: ${GTest_DIR}")
find_package(GTest REQUIRED)

# # google benchmark
# if(ANDROID)
#   set(benchmark_DIR "${ARTIFACTS_DIR}/benchmark/1.7.0/android-arm64/lib/cmake/benchmark" CACHE PATH "")
# elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
#   set(benchmark_DIR "${ARTIFACTS_DIR}/benchmark/1.7.0/linux-x64/lib/cmake/benchmark" CACHE PATH "")
# elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
#   set(benchmark_DIR "{ARTIFACTS_DIR}/benchmark/1.7.0/vs2019-x64/lib/cmake/benchmark" CACHE PATH "")
# elseif(CMAKE_SYSTEM_NAME MATCHES "Darwin")
#   set(benchmark_DIR "${ARTIFACTS_DIR}/benchmark/1.7.0/${CVBUILD_PLATFORM}-${CVBUILD_ARCH}/lib/cmake/benchmark" CACHE PATH "")
# else()
#   message(WARNING "GTest_DIR not set yet")
# endif()
# message(STATUS ">>> benchmark_DIR is: ${benchmark_DIR}")
# find_package(benchmark REQUIRED CONFIG)

# opencv
if(ANDROID)
  set(OpenCV_DIR "${ARTIFACTS_DIR}/opencv/4.6.0/android-arm64/sdk/native/jni" CACHE PATH "")
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
  set(OpenCV_DIR "${ARTIFACTS_DIR}/opencv/linux-x64/4.6.0/lib/cmake/opencv4" CACHE PATH "")
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
  set(OpenCV_DIR "${ARTIFACTS_DIR}/opencv/windows/4.5.5" CACHE PATH "")
else()
  message(WARNING "OpenCV_DIR not set yet")
endif()
message(STATUS ">>> OpenCV_DIR is: ${OpenCV_DIR}")
find_package(OpenCV REQUIRED)