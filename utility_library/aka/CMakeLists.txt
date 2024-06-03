cmake_minimum_required (VERSION 3.5)

include_directories(include/)
include_directories(../)

file(GLOB AKA_SRC src/*.cpp)

add_library(libAKA STATIC ${AKA_SRC})

set(CMAKE_CXX_FLAGS "-std=c++0x -Werror -g")
set(CRYPTO_LIBRARIES  "-lssl -lcrypto")
