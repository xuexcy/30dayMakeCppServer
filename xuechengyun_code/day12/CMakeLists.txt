cmake_minimum_required(VERSION 3.25)
project(client_and_server_main)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)

add_executable(server_main server_main.cpp)
add_executable(client_main client.cpp)

target_link_libraries(server_main event_loop server)
target_link_libraries(client_main buffer inet_address Socket util)


