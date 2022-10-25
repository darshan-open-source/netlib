cmake_minimum_required(VERSION 3.23.1)
project(test)
set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME}  web_test.cpp )
target_include_directories(${PROJECT_NAME} PUBLIC includes/)
include_directories(src/)

target_link_libraries(${PROJECT_NAME} pthread curl mysqlclient ssl crypto)
