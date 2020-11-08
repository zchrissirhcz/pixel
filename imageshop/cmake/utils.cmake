#################################################
#
# Useful funtions
#
#################################################
cmake_minimum_required(VERSION 3.1)

# --[ correctly show folder structure in Visual Studio
function(assign_source_group)
  foreach(_source IN ITEMS ${ARGN})
    if (IS_ABSOLUTE "${_source}")
      file(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
    else()
      set(_source_rel "${_source}")
    endif()
    get_filename_component(_source_path "${_source_rel}" PATH)
    string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
    source_group("${_source_path_msvc}" FILES "${_source}")
  endforeach()
endfunction(assign_source_group)

function(afq_add_executable)
  if (CMAKE_SYSTEM_NAME MATCHES "Windows" OR CMAKE_SYSTEM_NAME MATCHES "Darwin")
    foreach(_source IN ITEMS ${ARGN})
      assign_source_group(${_source})
    endforeach()
    #message("${ARGV}\n")
  endif ()
  add_executable(${ARGV})
endfunction(afq_add_executable)

function(afq_cuda_add_executable)
  if (CMAKE_SYSTEM_NAME MATCHES "Windows" OR CMAKE_SYSTEM_NAME MATCHES "Darwin")
    foreach(_source IN ITEMS ${ARGN})
      assign_source_group(${_source})
    endforeach()
    #message("${ARGV}\n")
  endif ()
  cuda_add_executable(${ARGV})
endfunction(afq_cuda_add_executable)

function(afq_add_library)
  if (CMAKE_SYSTEM_NAME MATCHES "Windows" OR CMAKE_SYSTEM_NAME MATCHES "Darwin")
    foreach(_source IN ITEMS ${ARGN})
      assign_source_group(${_source})
    endforeach()
    #message("${ARGV}\n")
  endif ()
  add_library(${ARGV})
endfunction(afq_add_library)

function(afq_cuda_add_library)
  if (CMAKE_SYSTEM_NAME MATCHES "Windows" OR CMAKE_SYSTEM_NAME MATCHES "Darwin")
    foreach(_source IN ITEMS ${ARGN})
      assign_source_group(${_source})
    endforeach()
    #message("${ARGV}\n")
  endif ()
  cuda_add_library(${ARGV})
endfunction(afq_cuda_add_library)

# append element to list with space as seperator 
function(afq_list_append __string __element)
  # set(__list ${${__string}})
  # set(__list "${__list} ${__element}")
  # set(${__string} ${__list} PARENT_SCOPE)
  #set(__list ${${__string}})
  set(${__string} "${${__string}} ${__element}" PARENT_SCOPE)
endfunction()



#################################################
#
# Important CFLAGS/CXXFLAGS
#
#################################################

set(AFQ_C_FLAGS "")
set(AFQ_CXX_FLAGS "")


# 0. 检查编译器版本。同一编译器的不同版本，编译选项的支持情况可能不同。
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  message(STATUS "--- CLANG_VERSION_MAJOR is: ${CLANG_VERSION_MAJOR}")
  message(STATUS "--- CLANG_VERSION_MINOR is: ${CLANG_VERSION_MINOR}")
  message(STATUS "--- CLANG_VERSION_PATCHLEVEL is: ${CLANG_VERSION_PATCHLEVEL}")
  message(STATUS "--- CLANG_VERSION_STRING is: ${CLANG_VERSION_STRING}")
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  message(STATUS "--- CMAKE_CXX_COMPILER_VERSION is: ${CMAKE_CXX_COMPILER_VERSION}")
  # if(CMAKE_CXX_COMPILER_VERSION GREATER 9.1) # when >= 9.2, not support this option
  #   message(STATUS "---- DEBUG INFO HERE !!!")
  # endif()
endif()


# 1. 函数没有声明就使用
if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
  afq_list_append(AFQ_C_FLAGS /we4013)
  afq_list_append(AFQ_CXX_FLAGS /we4013)
else()
  afq_list_append(AFQ_C_FLGAS -Werror=implicit-function-declaration)
  afq_list_append(AFQ_CXX_FLGAS -Werror=implicit-function-declaration)
endif()

# 2. 函数虽然有声明，但是声明不完整，没有写出返回值类型
if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
  afq_list_append(AFQ_C_FLAGS /we4431)
  afq_list_append(AFQ_CXX_FLAGS /we4431)
elseif(CMAKE_C_COMPILER_ID MATCHES "GNU")
  if(CMAKE_CXX_COMPILER_VERSION LESS 9.2) # gcc/g++ <= 9.1 required
    afq_list_append(AFQ_C_FLAGS -Werror=implicit-int)
    afq_list_append(AFQ_CXX_FLAGS -Werror=implicit-int)
  endif()
elseif(CMAKE_C_COMPILER_ID MATCHES "Clang")
  afq_list_append(AFQ_C_FLAGS -Werror=implicit-int)
  afq_list_append(AFQ_CXX_FLAGS -Werror=implicit-int)
endif()

# 3. 指针类型不兼容
if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
  afq_list_append(AFQ_C_FLAGS /we4133)
  afq_list_append(AFQ_CXX_FLAGS /we4133)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  if(CMAKE_CXX_COMPILER_VERSION LESS 9.2) # gcc/g++ <= 9.1 required
    afq_list_append(AFQ_C_FLAGS -Werror=incompatible-pointer-types)
    afq_list_append(AFQ_CXX_FLAGS -Werror=incompatible-pointer-types)
  endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  afq_list_append(AFQ_C_FLAGS -Werror=incompatible-pointer-types)
  afq_list_append(AFQ_CXX_FLAGS -Werror=incompatible-pointer-types)
endif()

# 4. 函数应该有返回值但没有return返回值
if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
  afq_list_append(AFQ_C_FLAGS /we4716)
  afq_list_append(AFQ_CXX_FLAGS /we4716)
else()
  afq_list_append(AFQ_C_FLAGS -Werror=return-type)
  afq_list_append(AFQ_CXX_FLAGS -Werror=return-type)
endif()

# 5. 避免使用影子(shadow)变量
if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
  afq_list_append(AFQ_C_FLAGS /we6244 /we6246 /we4457 /we4456)
  afq_list_append(AFQ_CXX_FLAGS /we6244 /we6246 /we4457 /we4456)
else()
  afq_list_append(AFQ_C_FLAGS -Werror=shadow)
  afq_list_append(AFQ_CXX_FLAGS -Werror=shadow)
endif()

# message(STATUS "=== CMAKE_C_COMPILER: ${CMAKE_C_COMPILER}")
# message(STATUS "=== CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
# message(FATAL_ERROR "=== CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")
# message(STATUS "=== CMAKE_C_COMPILER_ID: ${CMAKE_C_COMPILER_ID}")
# message(FATAL_ERROR "=== CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")

# 6. 函数不应该返回局部变量的地址
if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
  afq_list_append(AFQ_C_FLAGS /we4172)
  afq_list_append(AFQ_CXX_FLAGS /we4172)
else()
  if(CMAKE_C_COMPILER_ID MATCHES "Clang")
    afq_list_append(AFQ_C_FLAGS -Werror=return-stack-address)
    afq_list_append(AFQ_CXX_FLAGS -Werror=return-stack-address)
  else()#gcc/g++
    afq_list_append(AFQ_C_FLAGS -Werror=return-local-addr)
    afq_list_append(AFQ_CXX_FLAGS -Werror=return-local-addr)
  endif()
endif()

# 7. 变量没初始化就使用，要避免
if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
  afq_list_append(AFQ_C_FLAGS "/we4700 /we26495")
  afq_list_append(AFQ_CXX_FLAGS "/we4700 /we26495")
else()
  afq_list_append(AFQ_C_FLAGS -Werror=uninitialized)
  afq_list_append(AFQ_CXX_FLAGS -Werror=uninitialized)
endif()

# 8. printf等语句中的格式串和实参类型不匹配，要避免
if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
  afq_list_append(AFQ_C_FLAGS /we4477)
  afq_list_append(AFQ_CXX_FLAGS /we4477)
else()
  afq_list_append(AFQ_C_FLAGS -Werror=format)
  afq_list_append(AFQ_CXX_FLAGS -Werror=format)
endif()

# 9. 避免把unsigned int和int直接比较
if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
  afq_list_append(AFQ_C_FLAGS /we4018)
  afq_list_append(AFQ_CXX_FLAGS /we4018)
else()
  afq_list_append(AFQ_C_FLAGS -Werror=sign-compare)
  afq_list_append(AFQ_CXX_FLAGS -Werror=sign-compare)
endif()

# 10. 避免把int指针赋值给int类型变量
if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
  afq_list_append(AFQ_C_FLAGS /we4047)
  afq_list_append(AFQ_CXX_FLAGS /we4047)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  if(CMAKE_CXX_COMPILER_VERSION LESS 9.2) # gcc/g++ <= 9.1 required
    afq_list_append(AFQ_C_FLAGS -Werror=int-conversion)
    afq_list_append(AFQ_CXX_FLAGS -Werror=int-conversion)
  endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  afq_list_append(AFQ_C_FLAGS -Werror=int-conversion)
  afq_list_append(AFQ_CXX_FLAGS -Werror=int-conversion)
endif()

# 11. 检查数组下标越界访问
if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
  afq_list_append(AFQ_C_FLAGS "/we6201 /we6386 /we4789")
  afq_list_append(AFQ_CXX_FLAGS "/we6201 /we6386 /we4789")
else()
  afq_list_append(AFQ_C_FLAGS -Werror=array-bounds)
  afq_list_append(AFQ_CXX_FLAGS -Werror=array-bounds)
endif()

# 12. 函数声明中的参数列表和定义中不一样。只在纯C下出现
if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
  afq_list_append(AFQ_C_FLGAS /we4029)
endif()

# 13. 实参太多，比函数定义或声明中的要多。只在纯C下出现
if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
  afq_list_append(AFQ_C_FLAGS /we4020)
endif()

# 14. 避免void*类型的指针参参与算术运算
# MSVC C/C++默认会报错，Linux gcc不报warning和error，Linux g++只报warning
# Linux下 clang不报warning和error，clang++报error
if(CMAKE_C_COMPILER_ID MATCHES "GNU")
  afq_list_append(AFQ_C_FLAGS -Werror=pointer-arith)
  afq_list_append(AFQ_CXX_FLAGS -Werror=pointer-arith)
elseif(CMAKE_C_COMPILER_ID MATCHES "Clang")
  afq_list_append(AFQ_C_FLAGS -Werror=pointer-arith)
endif()

# 15. 避免符号重复定义（变量对应的强弱符号）。只在纯C下出现。
# 暂时没找到MSVC的对应编译选项
if(CMAKE_C_COMPILER_ID MATCHES "GNU" OR CMAKE_C_COMPILER_ID MATCHES "Clang")
  afq_list_append(AFQ_C_FLAGS -fno-common)
endif()

# 16. Windows下，源码已经是UTF-8编码，但输出中文到stdout时
# 要么编译报错，要么乱码。解决办法是编译输出为GBK格式
if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
  afq_list_append(AFQ_C_FLAGS "/source-charset:utf-8 /execution-charset:gbk ")
  afq_list_append(AFQ_CXX_FLAGS "/source-charset:utf-8 /execution-charset:gbk ")
endif()

# 17. 释放非堆内存
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  afq_list_append(AFQ_CXX_FLAGS -Werror=free-nonheap-object)
endif()


# 将上述定制的FLAGS追加到CMAKE默认的编译选项中
# 为什么是添加而不是直接设定呢？因为toolchain（比如android的）会加料
# afq_list_append(CMAKE_C_FLAGS "${AFQ_C_FLAGS}")
# afq_list_append(CMAKE_CXX_FLAGS "${AFQ_CXX_FLAGS}")
afq_list_append(CMAKE_C_FLAGS "${AFQ_C_FLAGS}")
afq_list_append(CMAKE_CXX_FLAGS "${AFQ_CXX_FLAGS}")

#################################################
#
# Add whole archive when build static library
#
#################################################
function (afq_add_whole_archive_flag lib output_var)
  #message(FATAL_ERROR "=== linker is: ${ANDROID_LD}")
  if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    message(STATUS "not supported yet")
  elseif(CMAKE_C_COMPILER_ID MATCHES "Clang" AND NOT ANDROID)
    set(${output_var} -Wl,-force_load ${lib} PARENT_SCOPE)
  else()
    #即使是NDK21并且手动传入ANDROID_LD=lld，依然要用ld的查重复符号的链接选项
    set(${output_var} -Wl,--whole-archive ${lib} -Wl,--no-whole-archive PARENT_SCOPE)
  endif()
endfunction()

#################################################
#
# cppcheck，开启静态代码检查，主要是检查编译器检测不到的UB
#   注：目前只有终端下能看到对应输出，其中NDK下仅第一次输出
#
#################################################
if(USE_CPPCHECK)
  find_program(CMAKE_CXX_CPPCHECK NAMES cppcheck)
  if (CMAKE_CXX_CPPCHECK)
    message(STATUS "cppcheck found")
    list(APPEND CMAKE_CXX_CPPCHECK
      "--enable=warning"
      "--inconclusive"
      "--force"
      "--inline-suppr"
    )
  else()
    message(STATUS "cppcheck not found. ignore it")
  endif()
endif()


#################################################
#
# platform architecture
#   TODO: more stuffs needed here
#
#################################################
# --[ platform
if((IOS AND CMAKE_OSX_ARCHITECTURES MATCHES "arm")
  OR (CMAKE_SYSTEM_PROCESSOR MATCHES "^(arm|aarch64)"))
  set(AFQ_TARGET_ARCH arm)
  option(NCNN_ARM82 "optimize aarch64 platform with armv8.2" ON)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^(mips)")
  set(AFQ_TARGET_ARCH mips)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^(riscv)")
  set(AFQ_TARGET_ARCH riscv)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^(powerpc)")
  set(AFQ_TARGET_ARCH powerpc)
elseif()
  set(AFQ_TARGET_ARCH x86)
endif()

message(STATUS "----- Target arch: ${AFQ_TARGET_ARCH}")

if(CMAKE_SYSTEM_NAME MATCHES "Windows" OR CMAKE_SYSTEM_NAME MATCHES "Linux" 
  OR CMAKE_SYSTEM_NAME MATCHES "Darwin")
  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(AFQ_TARGET_PLATFORM x64)
  else()#assume equal 4
    set(AFQ_TARGET_PLATFORM x86)
  endif()
  message("----- AFQ_TARGET_PLATFORM is: ${AFQ_TARGET_PLATFORM}")
endif()


#################################################
#
# Visual Studio stuffs: vs_version, vc_version
#
#################################################
if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
  if(MSVC_VERSION EQUAL 1600)
    set(vs_version vs2010)
    set(vc_version vc10)
  elseif(MSVC_VERSION EQUAL 1700)
    set(vs_version vs2012)
    set(vc_version vc11)
  elseif(MSVC_VERSION EQUAL 1800)
    set(vs_version vs2013)
    set(vc_version vc12)
  elseif(MSVC_VERSION EQUAL 1900)
    set(vs_version vs2015)
    set(vc_version vc14)
  elseif(MSVC_VERSION GREATER_EQUAL 1910 AND MSVC_VERSION LESS_EQUAL 1920)
    set(vs_version vs2017)
    set(vc_version vc15)
  elseif(MSVC_VERSION GREATER_EQUAL 1920)
    set(vs_version vs2019)
    set(vc_version vc16)
  endif()
endif()
