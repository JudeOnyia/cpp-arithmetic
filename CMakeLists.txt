# Specify Minimum Required Version
cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

# Specify Project and Language
project(cpp_arithmetic LANGUAGES CXX)

# Set CXX Flags with the -frounding-math option
set(CMAKE_CXX_FLAGS "-frounding-math")

# Set Include Directory
include_directories(include)

# Add Executable Program
add_executable(test_interval app/test_interval.cpp)
