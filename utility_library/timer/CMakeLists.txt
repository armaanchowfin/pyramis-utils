cmake_minimum_required (VERSION 3.5)

include_directories(include/)
include_directories(../)

file(GLOB TIM_SRC src/*.cpp)

add_library(libTIMER STATIC ${TIM_SRC})
set(CMAKE_CXX_FLAGS "-std=c++11 -lpthread")  
