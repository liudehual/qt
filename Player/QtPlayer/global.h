#ifndef GLOBAL_H
#define GLOBAL_H

#define QT_PLAYER_MAIN_VERSION "1"
#define QT_PLAYER_SUB_VERSION  "0"
#include <QDebug>
extern "C"
{
/// ffmpeg头文件
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavfilter/avfilter.h"
#include "libavformat/avio.h"
#include "libavcodec/opt.h"
#include "libavutil/time.h"
#include "libavutil/log.h"
#include "libavutil/avutil.h"
#include "libavutil/error.h"
#include "libavutil/fifo.h"
#include "libavutil/file.h"
#include "libavutil/frame.h"
#include "libswresample/swresample.h"
#include "libswscale/swscale.h"
#include "libavdevice/avdevice.h"
#include "libavutil/mem.h"
#include "libavutil/mathematics.h"
#include "libavcodec/avcodec.h"
#include "libavfilter/avfilter.h"
#include "libavutil/avassert.h"
#include "libavutil/avstring.h"
#include "libavutil/avutil.h"
#include "libavutil/channel_layout.h"
#include "libavutil/intreadwrite.h"
#include "libavutil/fifo.h"
#include "libavutil/mathematics.h"
#include "libavutil/opt.h"
#include "libavutil/parseutils.h"
#include "libavutil/pixdesc.h"
#include "libavutil/pixfmt.h"
#include <limits.h>

/// SDL头文件
#include "SDL/SDL.h"
#include "SDL/SDL_mutex.h"
#include "SDL/SDL_thread.h"
#include "SDL/SDL_types.h"
#include "SDL/SDL_timer.h"

}
#endif // GLOBAL_H
