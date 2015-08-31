include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_CXX_FLAGS -std=c++11) 

set(WClientLibs libcurl Crypto)
set (WClientSrc WClient.cpp)
if (LIBRARY_BUILD_STATIC)
    add_library(WClient STATIC ${WClientSrc})
    target_link_libraries(WClient ${WClientLibs})
else()
    add_library(WClient SHARED ${WClientSrc})
    target_link_libraries(WClient ${WClientLibs})
endif()
    
    
