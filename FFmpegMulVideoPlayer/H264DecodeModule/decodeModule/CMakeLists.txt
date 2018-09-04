project(DecodeModule)
include_directories(.)
INCLUDE_DIRECTORIES(user/local/include)
LINK_LIBRARIES(/usr/local/lib)
set(SRC_LIST
	OSCond.cpp
	OSMutex.cpp
	OSThread.cpp
        VideoDecodeThread.cpp
        Module.cpp
)
add_library(DecodeModule STATIC ${SRC_LIST})
if(LINUX)
target_link_libraries(DecodeModule pthread avutil avformat avcodec  avfilter avdevice swscale swresample)
else()
target_link_libraries(DecodeModule avutil avformat avcodec  avfilter avdevice swscale swresample)
endif()
