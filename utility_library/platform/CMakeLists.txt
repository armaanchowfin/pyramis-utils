cmake_minimum_required (VERSION 3.5)

include_directories(include/)
include_directories(../)

file(GLOB PLAT_SRC src/*.cpp)
set(SCTP_LIBRARY "-lsctp")

add_library(libPLATFORM STATIC ${PLAT_SRC})
target_link_libraries(libPLATFORM ${SCTP_LIBRARY})
set(CMAKE_CXX_FLAGS "-std=c++0x -Werror -g")
