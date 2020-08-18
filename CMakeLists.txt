# CMakeList.txt : CMake project for sun, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("sun")

aux_source_directory(./src DIR_SRCS)

# Add source to this project's executable.
add_executable (sun ${DIR_SRCS})

# TODO: Add tests and install targets if needed.
