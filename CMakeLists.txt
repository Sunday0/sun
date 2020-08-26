# CMakeList.txt : CMake project for sun, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("sun")

aux_source_directory(./src DIR_SRCS)

# Add source to this project's executable.
add_executable (sun ${DIR_SRCS}) # 生成可执行文件
#add_library(sun STATIC ${DIR_SRCS}) # 生成静态库
#add_library(sun SHARED ${DIR_SRCS}) # 生成动态库或共享库

# TODO: Add tests and install targets if needed.

add_compile_options("/std:c++latest")
