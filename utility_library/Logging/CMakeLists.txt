cmake_minimum_required (VERSION 3.5)

file(GLOB LOG src/*.cpp)

add_definitions(-DBOOST_LOG_DYN_LINK)

add_library(liblog STATIC ${LOG})
