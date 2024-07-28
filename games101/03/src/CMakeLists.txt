cmake_minimum_required(VERSION 3.10)
project(Rasterizer)

find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 17)

include_directories(/usr/local/include)

set( BOOST_PATH ${PROJECT_SOURCE_DIR}/../../boost/boost_1_85_0/build)
set(BOOST_INC ${BOOST_PATH}/include/)
set(BOOST_LIBS ${BOOST_PATH}/lib)


include_directories(${BOOST_INC})
link_directories(${BOOST_LIBS})

link_libraries(boost_system  boost_filesystem)

add_executable(Rasterizer main.cpp rasterizer.cpp Triangle.cpp)
target_link_libraries(Rasterizer ${OpenCV_LIBRARIES})

set(CMAKE_CXX_FLAGS "-g")