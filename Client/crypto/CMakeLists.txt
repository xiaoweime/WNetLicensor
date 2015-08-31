include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set (CryptoSrc sha256.c)
if (LIBRARY_BUILD_STATIC)
    add_library(Crypto STATIC ${CryptoSrc})
else()
    add_library(Crypto SHARED ${CryptoSrc})
endif()
#add_executable(sha256_test sha256_demo.c)
#target_link_libraries(sha256_test Crypto)
    
    
