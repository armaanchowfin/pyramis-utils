cmake_minimum_required (VERSION 3.5)

include_directories(include/)
include_directories(codec/include/)
include_directories(../)

file(GLOB NGAP_ASN src/*.c)
file(GLOB NGAP_CODEC codec/src/*.cpp)

# uncomment to enable additional debug logs
#add_definitions(-DASN_EMIT_DEBUG=1)
add_definitions(-DBOOST_LOG_DYN_LINK)
add_library(libNGAP_ASN STATIC ${NGAP_ASN} ${NGAP_CODEC})
set(CMAKE_CXX_FLAGS "-std=c++0x -Werror -g")
