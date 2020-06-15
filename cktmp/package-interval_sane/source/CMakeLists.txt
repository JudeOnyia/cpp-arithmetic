cmake_minimum_required(VERSION 3.10)
project(arithmetic LANGUAGES CXX)

option(ENABLE_EXTRAS "Enable extras" false)

#include(Sanitizers.cmake)

set(CMAKE_VERBOSE_MAKEFILE true)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

if (CMAKE_CXX_COMPILER_ID MATCHES GNU OR
  CMAKE_CXX_COMPILER_ID MATCHES Clang)
	add_compile_options("-pedantic-errors")
endif()

find_package(Catch2 REQUIRED)

find_package(CGAL REQUIRED)
#set(Boost_USE_MULTITHREADED ON)
#find_package(Boost 1.54.0 REQUIRED COMPONENTS numeric)

# GCC may need to -frounding-math option.
if (CMAKE_CXX_COMPILER_ID MATCHES GNU)
	add_compile_options("-frounding-math")
endif()

if(EXISTS ${CMAKE_SOURCE_DIR}/include/ra/kernel.hpp)
	set(KERNEL_FOUND 1)
else()
	set(KERNEL_FOUND 0)
endif()
if(EXISTS ${CMAKE_SOURCE_DIR}/app/delaunay_triangulation.cpp)
	set(DELAUNAY_FOUND 1)
else()
	set(DELAUNAY_FOUND 0)
endif()

add_library(ra INTERFACE)
target_include_directories(ra INTERFACE include)
target_link_libraries(ra INTERFACE ${CGAL_LIBRARY} ${GMP_LIBRARIES})
add_executable(test_interval app/test_interval.cpp)
target_link_libraries(test_interval ra Catch2::Catch2)
if (KERNEL_FOUND)
	add_executable(test_kernel app/test_kernel.cpp)
	target_link_libraries(test_kernel ra Catch2::Catch2)
endif()
if (DELAUNAY_FOUND)
	add_executable(delaunay_triangulation app/delaunay_triangulation.cpp)
	target_link_libraries(delaunay_triangulation ra)
endif()

if (ENABLE_EXTRAS)
	add_executable(trivial_interval app/trivial_interval.cpp)
	target_link_libraries(trivial_interval ra)
	add_executable(trivial_kernel app/trivial_kernel.cpp)
	target_link_libraries(trivial_kernel ra)
	add_executable(demo_kernel app/demo_kernel.cpp)
	target_link_libraries(demo_kernel ra)
	add_executable(triangulation_2_demo app/triangulation_2_demo.cpp)
	target_link_libraries(triangulation_2_demo ra)
endif()

#if (ENABLE_EXTRAS)
#	add_test(test_interval test_interval)
#	add_test(test_kernel test_kernel)
#	enable_testing()
#endif()

