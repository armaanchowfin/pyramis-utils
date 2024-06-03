cmake_minimum_required (VERSION 3.5)

include_directories(../)
include_directories(include/)

file(GLOB REGEX_SRC src/*.cpp )

add_definitions(-DBOOST_LOG_DYN_LINK)

add_library(libRegex STATIC ${REGEX_SRC})
set(CMAKE_CXX_FLAGS "-std=c++0x -Werror -g")
