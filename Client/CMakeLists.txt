CMake_Minimum_Required(VERSION 2.6)
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb ")#
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")#
project(WClient)

set (LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/)
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/) 
#set (LIBRARY_BUILD_SHARED ON)  #是否生成共享链接库
option (LIBRARY_BUILD_STATIC "StaticLib" ON)  #是否生成静态链接库


set(VERSION_MAJOR 1)
set(VERSION_MINOR 2) 
set(SERVICE_ROOT "\"http://xiaoweime.s957.com/sql/\"") 

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(SERVICE_ROOT \"http://localhost/Service/sql/\")
endif()



configure_file(
    "${PROJECT_SOURCE_DIR}/WClientConfig.h.in"
    "${PROJECT_BINARY_DIR}/WClientConfig.h"
) 

include_directories(sql)
include_directories(crypto)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${PROJECT_BINARY_DIR})


add_subdirectory(sql)
add_subdirectory(curl)
add_subdirectory(frontends)
add_subdirectory(crypto)

