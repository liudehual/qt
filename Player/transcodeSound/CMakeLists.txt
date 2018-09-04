project(transcodeSound)
cmake_minimum_required(VERSION 2.8)
include_directories(/usr/local/include)
link_directories(/usr/local/lib)
set(SRC_LIST
    main.cpp
)

add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(${PROJECT_NAME}
                        avformat
                        avcodec
                        avutil
                        avfilter
                        swresample
                        swscale
                        )
