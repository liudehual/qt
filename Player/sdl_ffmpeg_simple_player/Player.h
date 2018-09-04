#include "FileIo.h"

#define SDL_AUDIO_BUFFER_SIZE 1024
#define VIDEO_PICTURE_QUEUE_SIZE 1
#define MAX_QUEUE_SIZE (15 * 1024 * 1024)
#define MIN_FRAMES      5
#define FF_ALLOC_EVENT   (SDL_USEREVENT)
#define FF_REFRESH_EVENT (SDL_USEREVENT + 1)
#define FF_QUIT_EVENT (SDL_USEREVENT + 2)
#define AV_NOSYNC_THRESHOLD 10.0
#define VIDEO_ID 100
#define AUDIO_ID 101


typedef struct PacketQueue 
{
	AVPacketList *first_pkt, *last_pkt;
	int nb_packets;
	int size;
	SDL_mutex *mutex;
	SDL_cond *cond;
} PacketQueue;

typedef struct VideoPicture
{
	SDL_Overlay *bmp;
	int width, height; /* source height & width */
	int allocated;
	double pts;
} VideoPicture;

typedef struct VideoState 
{
	AVFormatContext *pFormatCtx;
	AVStream        *audio_st;
	AVStream        *video_st;
	PacketQueue     audioq;
	PacketQueue     videoq;
    uint8_t         audio_buf[(19200 * 3) / 2];
	unsigned int    audio_buf_size;
	unsigned int    audio_buf_index;
	AVPacket        audio_pkt;
	uint8_t         *audio_pkt_data;
	int             audio_pkt_size;
	VideoPicture    pictq[VIDEO_PICTURE_QUEUE_SIZE];
	int             pictq_size, pictq_rindex, pictq_windex;
	SDL_mutex       *pictq_mutex;
	SDL_cond        *pictq_cond;
	SDL_Thread      *read_tid;        //这是从文件读取数据放入队列的线程
	SDL_Thread      *video_tid;       //这是视频读取放入队列线程
	SDL_Thread      *refresh_tid;     //这是刷新线程
	double          audio_clock;
	double          video_clock;      ///<pts of last decoded frame / predicted pts of next decoded frame
	double          seek_time;        //要移动的时间（秒）
	int             seek_req;
	int             seek_flags;
	int64_t         seek_pos;
	int             width;            //当前sdl屏幕上的 宽和高
	int             height;           //当前sdl屏幕上的 宽和高 
	int             refresh;          //是否已经刷新
	int             force_refresh;    //强制刷新
} VideoState;


extern int fs_screen_width;                     //全屏的宽
extern int fs_screen_height;                    //全屏的高
extern AVPacket flush_pkt;                      //刷新packet
extern VideoState  * is;                        //音视频全局结构体
extern char filename[1024];                     //文件名称
extern AVCodecContext * video_dec_ctx ;         //视频解码context
extern AVCodecContext * audio_dec_ctx ;         //音频解码context
extern AVCodec * pvideo_codec;                  //视频解码器
extern AVCodec * paudio_codec;                  //音频解码器
extern char * window_title;                     //叠加的标题
extern SDL_Surface * screen;                    //sdl 屏幕显示结构体
extern int is_full_screen;                      //是否全屏播放
extern int screen_width ;                       //改变大小后的宽和高
extern int screen_height;                       //改变大小后的宽和高
extern int pause_play;                          //0表示暂停 1表示播放
extern int IsLoad;                              //是否已经加载文件 1 加载 0 未加载
extern int Isplay;                              //是否正在播放视频 1播放 0 未播放
extern int Isstop;                              //是否停止 1表示停止
extern int Isexit;                              //是否已经退出 1表示退出 0未退出    
//video pram
extern int m_video_stream_idx;                  //视频在文件中的流标号
extern double m_dbFrameRate;                    //视频帧率
extern double m_video_duration;                 //视频单帧持续时间 秒（s）
extern int m_dwWidth;                           //宽
extern int m_dwHeight;                          //高
extern AVCodecID m_video_codecID;               //视频编码类型
extern AVPixelFormat m_video_pixelfromat;       //视频yuv类型
extern char m_spspps[100];                      //spspps_buf
extern int m_spspps_size;                       //spspps_size
//audio pram
extern int m_audio_stream_idx;                  //音频在文件中的流标号
extern double m_audio_duration;                 //音频单帧持续时间 秒（s）
extern int m_dwChannelCount;                    //声道
extern int m_dwBitsPerSample;                   //样本
extern int m_dwFrequency;                       //采样率
extern AVCodecID m_audio_codecID;               //视频编码类型
extern int m_audio_frame_size;                  //一帧数据大小
extern int IsEnd_audio;                         //音频是否播放完毕 0 未播放完毕 1播放完毕
extern unsigned int m_mute_Volume;              //音量调节0-128 0为静音

int InitProgram();                                                      //程序启动做的初始化 打开编解码库 SDL初始化等,创建默认窗口
int UinitProgram();                                                     //程序退出做的释放工作,释放窗口
int Open_codec_context(int * stream_idx, AVFormatContext * fmt_ctx, enum AVMediaType type); //在文件中找到流标号
int stream_component_open(VideoState *is, int stream_index);            //创建 创建音频回调函数，视频线程
int stream_component_close(VideoState *is, int stream_index);           //关闭视频 关闭音频设备
void packet_queue_init(PacketQueue *q) ;                                //始化队列
int packet_queue_put(PacketQueue *q, AVPacket *pkt);                    //将packet填入队列（这里只是做的和刷新packet判断）
int packet_queue_get(PacketQueue *q, AVPacket *pkt, int queue_type);    //从队列里面取出数据
void packet_queue_abort(PacketQueue *q);                                //终止向队列输入输出
int packet_queue_put_private(PacketQueue *q, AVPacket *pkt);            //真正的填入队列函数
void packet_queue_start(PacketQueue *q);                                //刷新packet的初始化
void packet_queue_destroy(PacketQueue *q);                              //销毁队列
void packet_queue_flush(PacketQueue *q);                                //清空队列
void schedule_refresh(VideoState *is, int delay) ;                      //时间表刷新
Uint32 sdl_refresh_timer_cb(Uint32 interval, void *opaque) ;            //等待时间添加刷新事件
int read_thread(void *arg);                                             //从文件读取音视频 创建刷新线程
void audio_callback(void *userdata, unsigned char *stream, int len) ;   //音频回调函数
int audio_decode_frame(VideoState *is, uint8_t *audio_buf, int buf_size,double *pts_ptr);  //解码音频数据
int video_open(VideoState *is);                                         //创建打开窗口设置大小
int video_thread(void *arg);                                            //视频解码线程将解码后的数据传入VideoPicture结构体
int queue_picture(VideoState *is, AVFrame *pFrame, double pts);         //将解码后的数据添加到VideoPicture结构体中
void alloc_picture(void *userdata) ;                                    //创建输出SDL_Overlay yuv容器
void video_display(VideoState *is);                                     //显示视频
int refresh_thread(void *opaque);                                       //刷新线程
void video_refresh(void *opaque);                                       //刷新操作（视频）
int event_loop();                                                       //消息循环

void do_load(char * filename);                                          //打开文件获取文件信息
void do_play();                                                         //播放视频
void do_seek();                                                         //快进快退
void do_pause(VideoState *is);                                          //暂停
void do_stop(VideoState *is);                                           //停止 销毁窗口
void do_full_screen(VideoState *is);                                    //全屏
void do_exit(VideoState *is);                                           //退出程序
void do_resize_screen(VideoState *is,int width,int height);             //改变窗口大小
