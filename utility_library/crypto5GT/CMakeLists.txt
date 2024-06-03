cmake_minimum_required (VERSION 3.5)

include_directories(include/)
include_directories(../)

file(GLOB CRYPT_SRC src/*.cpp)

add_library(libcrypto5GT STATIC ${CRYPT_SRC})

set(CRYPTO_LIBRARIES  "-lssl -lcrypto")
set(CMAKE_CXX_FLAGS "-std=c++0x -Werror -g")
