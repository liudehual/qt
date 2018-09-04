cmake_minimum_required(VERSION 2.8)

project(H264DecodeModule)
include_directories(../decodeModule)
INCLUDE_DIRECTORIES(../decodeModule/include)
add_executable(${PROJECT_NAME} "main.cpp")
target_link_libraries(${PROJECT_NAME} DecodeModule)

