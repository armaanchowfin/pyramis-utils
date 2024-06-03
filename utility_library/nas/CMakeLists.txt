cmake_minimum_required (VERSION 3.5)

include_directories(../)
include_directories(NasDatatypes/)
include_directories(mobility/)
include_directories(sessionMgmt/)

include_directories(../ngap/include/)
file(GLOB NAS_SRC NasMessage.cpp mobility/*.cpp)
file(GLOB NAS_DATATYPES  NasDatatypes/*.cpp)
file(GLOB NAS_SESSION_MM_SRC sessionMgmt/*.cpp)
file(GLOB NAS_UTIL nasUtil/*.cpp)

add_definitions(-DBOOST_LOG_DYN_LINK)
# uncomment to enable additional NAS debug logs
add_definitions(-DNASLOG)

add_library(libNAS STATIC ${NAS_DATATYPES} ${NAS_SRC} ${NAS_SESSION_MM_SRC} ${NAS_UTIL})
#set(CMAKE_CXX_FLAGS "-std=c++0x -Werror -g -fsanitize=address")
