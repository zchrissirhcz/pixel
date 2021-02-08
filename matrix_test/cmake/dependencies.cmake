#--------------------------------------------------------------------------------
# OpenCV
#--------------------------------------------------------------------------------
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(OpenCV_DIR "E:/lib/opencv/4.5.0" CACHE PATH "")
elseif(ANDROID)
    set(OpenCV_DIR "E:/soft/Android/opencv-4.5.0-android-sdk/sdk/native/jni" CACHE PATH "")
endif()
message(STATUS "--------- OpenCV_DIR: ${OpenCV_DIR}")
find_package(OpenCV REQUIRED)
message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")
message(STATUS "    OpenCV_SHARED: ${OpenCV_SHARED}")
