cmake_minimum_required (VERSION 2.8.9)
project (homeAutomation)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

include_directories (include)
include_directories ( ${Boost_INCLUDE_DIR} )

find_package(Boost REQUIRED COMPONENTS system)

file (GLOB SOURCES "src/*.cpp")

add_executable (homeAutomation ${SOURCES})

target_link_libraries (homeAutomation ${Boost_LIBRARIES} -lwiringPi -lwiringPiDev -lmosquittopp -lrf24 -lpthread)
