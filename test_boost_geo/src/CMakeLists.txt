cmake_minimum_required (VERSION 3.0)
project(Cpp_Boost_test)

include(ENV.cmake)

set( target_name main)
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) 
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )

include_directories(${BOOST_INC})
link_directories(${BOOST_LIBS})

link_libraries(boost_system  boost_filesystem)

add_executable(simple simple.cpp)
add_executable(spatial_index spatial_index.cpp)
add_executable(topology topology.cpp)