系统平台:ubuntu 16.04
Qt版本:Qt5.2.0
ffmpeg版本:ffmpeg-2.2.16.tar.gz

各文件及目录介绍:
	H264DecodeModule: 解码库
	MultiVideo:多屏播放器
	ffmpeg-2.2.16.tar.gz:ffmpeg 解码库
	
编译及使用步骤:
	1.编译H264DecodeModule解码库，生成 libDecodeModule.a库文件
	2.将 libDecodeModule.a库文件 及 Module.h头文件分别拷贝到MultiVideo目录下的lib目录及include目录
	3.编译 MultiVideo 目录
	