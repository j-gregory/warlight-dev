cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(warlight-dev)
set(CMAKE_BUILD_TYPE Release)

# Set this project to use C++11
set (CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

include_directories("./include/warlight-dev")

add_library(Bot src/Bot.cpp)
add_library(Parser src/Parser.cpp)
add_library(Region src/Region.cpp)
add_library(SuperRegion src/SuperRegion.cpp)
add_library(UCTManager src/UCTManager.cpp src/State.cpp)

add_executable(BotTest src/main.cpp)
target_link_libraries(BotTest Bot Parser Region SuperRegion UCTManager)