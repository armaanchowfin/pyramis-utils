cmake_minimum_required (VERSION 3.5)

include_directories(include/)
include_directories(../)

file(GLOB COMM_SRC src/*.cpp)

add_library(libCOMMON STATIC ${COMM_SRC})
set(CMAKE_CXX_FLAGS "-std=c++0x -Werror -g")
