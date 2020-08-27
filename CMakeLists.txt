# CMakeList.txt : CMake project for sun, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("sun")

# The version number.
#set (sun_VERSION_MAJOR 1)
#et (sun_VERSION_MINOR 0)


# configure a header file to pass some of the CMake settings
# to the source code
#configure_file (
#  "${PROJECT_SOURCE_DIR}/sun_config.h.in"
#  "${PROJECT_BINARY_DIR}/sun_config.h"
#  )



# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
#include_directories("${PROJECT_BINARY_DIR}")


aux_source_directory(./src DIR_SRCS)

# Add source to this project's executable.
add_executable (sun ${DIR_SRCS}) # 生成可执行文件
#add_library(sun STATIC ${DIR_SRCS}) # 生成静态库
#add_library(sun SHARED ${DIR_SRCS}) # 生成动态库或共享库

# TODO: Add tests and install targets if needed.

add_compile_options("/std:c++latest")
#add_compile_options("/D \"_MBCS\"")
add_definitions ( -D_WINSOCK_DEPRECATED_NO_WARNINGS )

