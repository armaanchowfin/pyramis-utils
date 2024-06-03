cmake_minimum_required (VERSION 3.5)

include_directories(../include/)
include_directories(../../)

set(CRYPTO_LIBRARIES  "-lssl -lcrypto")


#add_subdirectory(../../../Utility_library/Logging bin_Logging)
add_subdirectory(../../../Utility_library/platform bin_PLAT)
#add_subdirectory(../../../Utility_library/aka bin_AKA)
#add_subdirectory(../../../Utility_library/crypto5GT bin_crypto5GT)
add_subdirectory(../../../Utility_library/common bin_COMMON)


set(CMAKE_CXX_FLAGS "-std=c++0x -Werror -g")

add_executable(AKA_CRYPT_TEST ausfTest.cpp
               #../src/cryptUtil.cpp ../../common/src/utils.cpp
               )

target_link_libraries(AKA_CRYPT_TEST libPLATFORM libCOMMON
		/home/pascal/data/CewitClone3/three/Utility_library/aka/liblibAKA.a
		/home/pascal/data/CewitClone3/three/Utility_library/crypto5GT/liblibcrypto5GT.a 
	       	${CRYPTO_LIBRARIES})
