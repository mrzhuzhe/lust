cmake_minimum_required (VERSION 3.0)
project(Cpp_Triangular_test)

include(ENV.cmake)

set( target_name main)
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) 
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )

link_directories(${opencv4_libary_path})
include_directories(${opencv4_include_path})

add_executable(${target_name} main.cpp)

target_link_libraries(${target_name} opencv_core opencv_imgproc opencv_highgui)