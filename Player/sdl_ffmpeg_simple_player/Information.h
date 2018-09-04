
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

extern "C"
{
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
#include "libavutil/avutil.h"
#include "libswresample/swresample.h"
#include "libswscale/swscale.h"
#include "libavutil/mathematics.h"
#include "inttypes.h"
#include "SDL/SDL.h"
#include "SDL/SDL_main.h"
#include "SDL/SDL_thread.h"
}

//这里不用这个东东 木有用
//#pragma comment(lib,"SDLmain.lib")


/*
h264有两种封装，
一种是annexb模式，传统模式，有startcode，SPS和PPS是在ES中
一种是mp4模式，一般mp4 mkv会有，没有startcode，SPS和PPS以及其它信息被封装在container中，每一个frame前面是这个frame的长度
很多解码器只支持annexb这种模式，因此需要将mp4做转换：
在ffmpeg中用h264_mp4toannexb_filter可以做转换
实现：
注册filter
avcbsfc = av_bitstream_filter_init("h264_mp4toannexb");
转换bitstream
av_bitstream_filter_filter(AVBitStreamFilterContext *bsfc,
AVCodecContext *avctx, const char *args,
uint8_t **poutbuf, int *poutbuf_size,
const uint8_t *buf, int buf_size, int keyframe)
*/

/*
MP4模式的文件中如果音频是AAC的，像TS中如果 有AAC格式不算，这时候的AAC不是 ADTS的，
需要；audio_dec_ctx->extradata[0],audio_dec_ctx->extradata[1]中存储，
audio_dec_ctx->extradata_size ==2 ，表示长度。
需要将这两个字节转换为AAC格式ADTS的头，7个字节，然后和packet中的数据组合存入文件，或者传输。
*/

/*
这里还有一点注意 avpacket读出来数据 先做解码，然后在存储，
如果想先存储，然后再转换需要改变一下解码方式，本程序暂时不写。
*/



