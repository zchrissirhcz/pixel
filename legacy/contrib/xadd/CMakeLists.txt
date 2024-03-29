cmake_minimum_required(VERSION 3.15)

project(xadd)

set(CMAKE_CXX_STANDARD 11)

#set(CMAKE_BUILD_TYPE Release)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


#tsan
option(USE_TSAN "Use Thread Sanitizer?" ON)
#--------------------------------------------------
# globally setting
#--------------------------------------------------
# https://stackoverflow.com/a/65019152/2999096
if(USE_TSAN)
  if (MSVC)
    message(WARNING "ThreadSanitizer for MSVC must be enabled manually in project's property page.")
  else()
    message(STATUS ">>> USE_TSAN: YES")
    message(STATUS ">>> CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
    if (CMAKE_BUILD_TYPE MATCHES "Debug")
      message(STATUS ">>>!!!!!!!!!!!Debug mode")
      # On debug mode
      set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=thread")
      set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=thread")
      set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=thread")
    elseif (CMAKE_BUILD_TYPE MATCHES "Release")
      # Note: `-g` is explicitly required here, even if NDK android.toolchain.cmake's `-g` exists.
      set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -g -fno-omit-frame-pointer -fsanitize=thread")
      set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g -fno-omit-frame-pointer -fsanitize=thread")
      set(CMAKE_LINKER_FLAGS_RELEASE "${CMAKE_LINKER_FLAGS_RELEASE} -g -fno-omit-frame-pointer -fsanitize=thread")
    elseif (CMAKE_BUILD_TYPE MATCHES "RelWithDebInfo")
      set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} -g -fno-omit-frame-pointer -fsanitize=thread")
      set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -g -fno-omit-frame-pointer -fsanitize=thread")
      set(CMAKE_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_LINKER_FLAGS_RELWITHDEBINFO} -g -fno-omit-frame-pointer -fsanitize=thread")
    elseif (CMAKE_BUILD_TYPE MATCHES "MinSizeRel")
      set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL} -g -fno-omit-frame-pointer -fsanitize=thread")
      set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} -g -fno-omit-frame-pointer -fsanitize=thread")
      set(CMAKE_LINKER_FLAGS_MINSIZEREL "${CMAKE_LINKER_FLAGS_MINSIZEREL} -g -fno-omit-frame-pointer -fsanitize=thread")
    elseif (CMAKE_BUILD_TYPE EQUAL "None" OR NOT CMAKE_BUILD_TYPE)
      set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fno-omit-frame-pointer -fsanitize=thread")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fno-omit-frame-pointer -fsanitize=thread")
      set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -g -fno-omit-frame-pointer -fsanitize=thread")
    endif()
  endif()
else()
  message(STATUS ">>> USE TSan: NO")
endif()



add_executable(testbed
    #testbed3.cpp
    #array_v9_std_atomic.cpp # requires pthread
    #array_v9_std_mutex.cpp
    array_v9_system_api.cpp
)

target_link_libraries(testbed pthread)