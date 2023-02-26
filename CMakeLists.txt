cmake_minimum_required(VERSION 3.25.2)
project(netlib)
set(CMAKE_CXX_STANDARD 20)

include_directories(includes/)

add_library(netlib SHARED src/get.cpp src/config.cpp src/SocketServer.cpp src/helper.cpp src/standalone.cpp  src/stringutil.cpp src/public_directory.cpp src/server_time.cpp src/post.cpp src/cookies.cpp src/sessions.cpp src/response_header.cpp  src/httpResponse.cpp src/url.cpp src/dynamicurl.cpp src/Application.cpp  src/Router.cpp src/multipart.cpp src/header_processor2.cpp src/middleware.cpp src/webserver.cpp src/httpRequest.cpp)
add_executable(web_test web_test.cpp )
target_link_libraries(web_test pthread netlib curl mysqlclient ssl crypto gtest)
