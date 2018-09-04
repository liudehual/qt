project(SDL_FFMPEG_Simpler)

cmake_minimum_required(VERSION 2.8)

include_directories(.)
include_directories(/usr/local/include)
LINK_LIBRARIES(/usr/local/lib/)

set(SRC
    Main.cpp
    FileIo.cpp
    Player.cpp
)

add_executable(${PROJECT_NAME} ${SRC})

TARGET_LINK_LIBRARIES(${PROJECT_NAME} avformat avcodec avutil swresample swscale SDL)

