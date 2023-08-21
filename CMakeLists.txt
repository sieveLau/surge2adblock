cmake_minimum_required(VERSION 3.23)
project(surge2adblock)

set(CMAKE_CXX_STANDARD 17)

ADD_SUBDIRECTORY(ext/curlcpp)
INCLUDE_DIRECTORIES(ext/curlcpp)

add_executable(surge2adblock main.cpp)
target_link_libraries(surge2adblock curlcpp::curlcpp)
