cmake_minimum_required (VERSION 3.5)

include_directories(../)
include_directories(pfcpIEs/)

file(GLOB PFCP_SRC *.cpp )
file(GLOB PFCP_DATATYPES  pfcpIEs/*.cpp)

add_definitions(-DBOOST_LOG_DYN_LINK)
# uncomment to enable additional NAS debug logs
add_definitions(-DPFCPLOG)

add_library(libPFCP STATIC ${PFCP_DATATYPES} ${PFCP_SRC})
