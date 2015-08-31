set(Libs libcurl WClient)

set(CMAKE_CXX_FLAGS -std=c++11)
set(frontend_terminal_src FrontendTerminal.cpp)

add_executable(terminal_demo ${frontend_terminal_src})
target_link_libraries(terminal_demo ${Libs})
