cmake_minimum_required(VERSION 3.25)
project(cpp_server)

set(CMAKE_CXX_STANDARD 20)

add_library(acceptor acceptor.h acceptor.cpp)
add_library(channel channel.h channel.cpp)
add_library(epoll epoll.h epoll.cpp)
add_library(event_loop event_loop.h event_loop.cpp)
add_library(inet_address inet_address.h inet_address.cpp)
add_library(server server.h server.cpp)
add_library(Socket Socket.h Socket.cpp)
add_library(util util.h util.cpp)

target_link_libraries(event_loop epoll)
target_link_libraries(channel event_loop)
target_link_libraries(acceptor channel event_loop Socket)

target_link_libraries(Socket inet_address util)
target_link_libraries(server acceptor channel inet_address Socket)

