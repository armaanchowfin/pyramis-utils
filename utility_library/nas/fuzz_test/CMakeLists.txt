cmake_minimum_required (VERSION 3.5)
set(CMAKE_BUILD_TYPE Debug)

project(NAS_FUZZ)
message(STATUS "Compiling Network Function : ${PROJECT_NAME}")
set (CMAKE_CXX_STANDARD 11)

message(STATUS "Project Directory: ${PROJECT_SOURCE_DIR}")


# add all necessary libs
add_subdirectory(../../aka bin_AKA)
add_subdirectory(../../platform bin_PLAT)
add_subdirectory(../../common bin_COMMON)
add_subdirectory(../../nas bin_NAS)
add_subdirectory(../../crypto5GT bin_crypto5GT)


# add all include paths
include_directories(${PROJECT_SOURCE_DIR})
include_directories(../../)


add_executable(${PROJECT_NAME} fuzzer.cpp )

set(BASIC_LIBRARIES "-lcrypto -lssl -lpthread")
set(CMAKE_CXX_FLAGS "-std=c++0x -Werror -g -fsanitize=address")

target_link_libraries(${PROJECT_NAME}
                      libNAS libPLATFORM libCOMMON libAKA libcrypto5GT
                      ${BASIC_LIBRARIES})

