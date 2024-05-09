cmake_minimum_required (VERSION 3.0)
project(Cpp_CGAL_test)

include(ENV.cmake)

set( target_name main)
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) 
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )

include_directories(${BOOST_INC})
link_directories(${BOOST_LIBS})
include_directories(${Eigen3_include_path})

find_package(CGAL REQUIRED COMPONENTS Qt6)
link_libraries(boost_system boost_filesystem CGAL::CGAL)

add_executable(simple simple.cpp)
add_executable(surface_mesh surface_mesh.cpp)
add_executable(triangular triangular.cpp)
set(CMAKE_BUILD_TYPE Release) 

if(CGAL_Qt6_FOUND)
  target_link_libraries(surface_mesh PUBLIC CGAL::CGAL_Basic_viewer)
  target_link_libraries(triangular PUBLIC CGAL::CGAL_Basic_viewer)
  message(STATUS QT founded)
endif()