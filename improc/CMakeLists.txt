cmake_minimum_required(VERSION 3.15)

project(improc)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 11)
include("../overlook.cmake")

#add_definitions(-ffast-math)

#add_definitions(-O3)
#set(CMAKE_BUILD_TYPE Debug)
message(STATUS "--- CMAKE_BUILD_TYPE is: ${CMAKE_BUILD_TYPE}")
message(STATUS "--- CMAKE_C_FLAGS are: ${CMAKE_C_FLAGS}")
message(STATUS "--- CMAKE_CXX_FLAGS are: ${CMAKE_CXX_FLAGS}")

add_executable(testbed
    # rgb2gray.h
    # rgb2gray.c
    # rgb2gray_test.cpp

    rgb2bgr.h
    rgb2bgr.c
    rgb2bgr_test.cpp

    # threshold.h
    # threshold.c
    # threshold_test.cpp

    #test.cpp

    #histogram.h
    #histogram.c
    #histogram_test.cpp

    # flip.h
    # flip.c
    # flip_test.cpp

    # boxfilter.h
    # boxfilter.c
    # boxfilter_test.cpp

    # ${CMAKE_SOURCE_DIR}/../matcalc/matrix_transpose.h
    # ${CMAKE_SOURCE_DIR}/../matcalc/matrix_transpose.c

    # copy_make_border.h
    # copy_make_border.c
    # copy_make_border_test.cpp

    # copy_cut_border.h
    # copy_cut_border.c
    # copy_cut_border_test.cpp

    # improc_zcx.h
    # improc_zcx.cpp
)
target_include_directories(testbed
    PUBLIC
        ${CMAKE_SOURCE_DIR}/..
)

if (CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
    if (CMAKE_SYSTEM_NAME MATCHES "Linux")
        #set(OpenCV_DIR "/home/zz/soft/opencv-4.5.1/lib/cmake/opencv4" CACHE PATH "") # shared
        set(OpenCV_DIR "/home/zz/lib/opencv/4.5.2/lib/cmake/opencv4" CACHE PATH "") # static
    elseif (ANDROID)
        set(OpenCV_DIR "/home/zz/soft/opencv-4.5.2-dev-android/sdk/native/jni" CACHE PATH "")
    endif()
elseif (CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
    if (CMAKE_SYSTEM_NAME MATCHES "Windows")
        set(OpenCV_DIR "E:/lib/opencv/4.5.2" CACHE PATH "")
    elseif (ANDROID)
        set(OpenCV_DIR "E:/soft/Android/opencv-4.5.2-android-sdk/sdk/native/jni" CACHE PATH "")
    endif()
elseif (CMAKE_HOST_SYSTEM_NAME MATCHES "Darwin")
    if (CMAKE_SYSTEM_NAME MATCHES "Darwin")
        set(OpenCV_DIR "/usr/local/opencv-4.2/lib/cmake/opencv4" CACHE PATH "")
    elseif (ANDROID)
        set(OpenCV_DIR "/Users/chris/soft/opencv-4.5.0-android-sdk/sdk/native/jni" CACHE PATH "")
    endif()
endif()
find_package(OpenCV REQUIRED)
target_link_libraries(testbed ${OpenCV_LIBS})


message(STATUS "--------------------------------------------")

include(CMakePrintHelpers)

# if (CMAKE_BUILD_TYPE MATCHES "Debug" OR CMAKE_BUILD_TYPE EQUAL "None" OR NOT CMAKE_BUILD_TYPE)
#     message(STATUS "----- CMAKE_BUILD_TYPE is Debug")
# elseif (CMAKE_BUILD_TYPE MATCHES "Release")
#     message(STATUS "----- CMAKE_BUILD_TYPE is Release")
# elseif (CMAKE_BUILD_TYPE MATCHES "RelWitchDebInfo")
#     message(STATUS "----- CMAKE_BUILD_TYPE is RelWitchDebInfo")
# elseif (CMAKE_BUILD_TYPE MATCHES "MinSizeRel")
#     message(STATUS "----- CMAKE_BUILD_TYPE is MinSizeRel")
# else ()
#     message(STATUS "----- unknown CMAKE_BUILD_TYPE = " ${CMAKE_BUILD_TYPE})
# endif ()

if (CMAKE_BUILD_TYPE MATCHES "Release" OR CMAKE_BUILD_TYPE MATCHES "RelWitchDebInfo" OR CMAKE_BUILD_TYPE MATCHES "MinSizeRel")
    message(STATUS "CMAKE_BUILD_TYPE is ${CMAKE_BUILD_TYPE}")
    cmake_print_variables(CMAKE_C_FLAGS_RELEASE)
    cmake_print_variables(CMAKE_CXX_FLAGS_RELEASE)
    cmake_print_variables(CMAKE_ASM_FLAGS_RELEASE)
elseif (CMAKE_BUILD_TYPE MATCHES "Debug")
    message(STATUS "CMAKE_BUILD_TYPE is ${CMAKE_BUILD_TYPE}")
    cmake_print_variables(CMAKE_C_FLAGS_DEBUG)
    cmake_print_variables(CMAKE_CXX_FLAGS_DEBUG)
    cmake_print_variables(CMAKE_ASM_FLAGS_DEBUG)
elseif (NOT CMAKE_BUILD_TYPE)
    message(STATUS "CMAKE_BUILD_TYPE is not defined")
    cmake_print_variables(CMAKE_C_FLAGS_DEBUG)
    cmake_print_variables(CMAKE_CXX_FLAGS_DEBUG)
    cmake_print_variables(CMAKE_ASM_FLAGS_DEBUG)
    cmake_print_variables(CMAKE_C_FLAGS_RELEASE)
    cmake_print_variables(CMAKE_CXX_FLAGS_RELEASE)
    cmake_print_variables(CMAKE_ASM_FLAGS_RELEASE)
    cmake_print_variables(CMAKE_C_FLAGS)
    cmake_print_variables(CMAKE_CXX_FLAGS)
endif()

# cmake_print_variables(CMAKE_C_FLAGS)
# cmake_print_variables(CMAKE_CXX_FLAGS)
# cmake_print_variables(CMAKE_ASM_FLAGS)
# cmake_print_variables(CMAKE_C_FLAGS_DEBUG)
# cmake_print_variables(CMAKE_CXX_FLAGS_DEBUG)
# cmake_print_variables(CMAKE_ASM_FLAGS_DEBUG)
# cmake_print_variables(CMAKE_C_FLAGS_RELEASE)
# cmake_print_variables(CMAKE_CXX_FLAGS_RELEASE)
# cmake_print_variables(CMAKE_ASM_FLAGS_RELEASE)
# cmake_print_variables(CMAKE_SHARED_LINKER_FLAGS)
# cmake_print_variables(CMAKE_MODULE_LINKER_FLAGS)
# cmake_print_variables(CMAKE_EXE_LINKER_FLAGS)