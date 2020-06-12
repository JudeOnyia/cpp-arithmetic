# Specify Minimum Required Version
cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

# Specify Project and Language
project(cpp_arithmetic LANGUAGES CXX)

# Set CXX Flags with the -frounding-math option
set(CMAKE_CXX_FLAGS "-frounding-math")

# Find CGAL Library
find_package(CGAL REQUIRED)

# Set Include directories and libraries
include_directories(include ${CGAL_INCLUDE_DIRS})
link_libraries(${CGAL_LIBRARY} ${GMP_LIBRARIES})

# Add Executable Program
add_executable(test_interval app/test_interval.cpp)
add_executable(test_kernel app/test_kernel.cpp)
