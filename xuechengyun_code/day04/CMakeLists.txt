cmake_minimum_required(VERSION 3.25)
project(server_client)

set(CMAKE_CXX_STANDARD 20)

add_library(inet_address inet_address.h inet_address.cpp)
add_library(Socket Socket.h Socket.cpp)
add_library(epoll epoll.h epoll.cpp)
add_library(util util.h util.cpp)
add_executable(server server.cpp)
add_executable(client client.cpp)

target_link_libraries(server inet_address Socket epoll util)
target_link_libraries(client inet_address Socket util)
