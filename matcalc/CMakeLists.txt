cmake_minimum_required(VERSION 3.15)

project(matcalc)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
option(PIXEL_ARMV8_2_FEATURE_FP16 "Enable armv8.2-a+fp16 support" ON)
option(PIXEL_ARMV8_2_FEATURE_DOTPROD "enable armv8.2-a+dotprod support" ON)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")

add_executable(testbed
    # matrix.h
    # matrix.c
    # matrix_test.cpp

    # matrix_column_max.h
    # matrix_column_max.c
    # matrix_column_max_test.cpp

    # mean_stddev.h
    # mean_stddev.c
    # mean_stddev_test.cpp

    # array_mean.h
    # array_mean.c
    # array_mean_test.cpp

    #eigen_test.cpp

    # matrix_add_subtract.h
    # matrix_add_subtract.c
    # matrix_add_subtract_test.cpp

    # array_fill.h
    # array_fill.c
    # array_fill_test.cpp

    #matrix_transpose.h
    #matrix_transpose.c
    #matrix_transpose_test.cpp

    #dotproduct.h
    #dotproduct.c
    #dotproduct_test.cpp

    #square_matrix_inverse.h
    #square_matrix_inverse.c
    square_matrix_inverse_test.cpp

    # matrix_gemm.h
    # matrix_gemm.c
    # matrix_gemm_test.cpp
)

target_include_directories(testbed PRIVATE
    ${CMAKE_SOURCE_DIR}/..
    ${CMAKE_SOURCE_DIR}/eigen
)

if (CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
    if (CMAKE_SYSTEM_NAME MATCHES "Linux")
        #set(OpenCV_DIR "/home/zz/soft/opencv-4.5.1/lib/cmake/opencv4" CACHE PATH "") # shared
        set(OpenCV_DIR "/home/zz/soft/opencv-4.5.1-static/lib/cmake/opencv4" CACHE PATH "") # static
    elseif (ANDROID)
        set(OpenCV_DIR "/home/zz/soft/opencv-4.5.1-android-sdk/sdk/native/jni" CACHE PATH "")
    endif()
elseif (CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
    if (CMAKE_SYSTEM_NAME MATCHES "Windows")
        #set(OpenCV_DIR "E:/lib/opencv/4.5.0" CACHE PATH "")
        set(OpenCV_DIR "E:/lib/opencv/4.5.2-pre" CACHE PATH "")
    elseif (ANDROID)
        set(OpenCV_DIR "E:/soft/Android/opencv-4.5.0-android-sdk/sdk/native/jni" CACHE PATH "")
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

# arm8.2, fp16 and dotprod(sdot)
if(((IOS AND CMAKE_OSX_ARCHITECTURES MATCHES "arm64") OR (APPLE AND CMAKE_OSX_ARCHITECTURES MATCHES "arm64") OR (CMAKE_SYSTEM_PROCESSOR           MATCHES "^(arm64|aarch64)")))
    if (PIXEL_ARMV8_2_FEATURE_FP16)                                                                                                  
        set(PIXEL_MARCH "-march=armv8.2-a+fp16")
    endif()
    if (PIXEL_ARMV8_2_FEATURE_DOTPROD)
        if(PIXEL_ARMV8_2_FEATURE_FP16)
            set(PIXEL_MARCH "-march=armv8.2-a+fp16+dotprod")
        else()
            set(PIXEL_MARCH "-march=armv8.2-a+dotprod")
        endif()
    endif()
    target_compile_options(testbed PRIVATE ${PIXEL_MARCH})
endif()


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