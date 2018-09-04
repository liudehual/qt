#include "Player.h"

int fs_screen_width = 0; 
int fs_screen_height = 0;
AVPacket flush_pkt;
VideoState  * is = NULL;
char  filename[1024] = INPUTFLVFILENAME;
AVCodecContext * video_dec_ctx  = NULL;
AVCodecContext * audio_dec_ctx  = NULL;
AVCodec * pvideo_codec = NULL;
AVCodec * paudio_codec = NULL;
char * window_title = NULL;
SDL_Surface * screen = NULL;
int is_full_screen = 0;
int screen_width  = 0;
int screen_height = 0;
int pause_play = 1;  //这里设置文件刚打开时候的状态0 暂停 1播放
int Isstop = 0;
int Isexit = 0;
int IsLoad = 0;
int Isplay = 0;
//video pram
int m_video_stream_idx = -1;
double m_video_duration = 0.040; 
double m_dbFrameRate = 25.0;
int m_dwWidth = 640;
int m_dwHeight = 480; 
AVCodecID m_video_codecID = AV_CODEC_ID_H264;
AVPixelFormat m_video_pixelfromat = AV_PIX_FMT_YUV420P;     
char m_spspps[100];                    
int m_spspps_size = 0;

//audio pram
int m_audio_stream_idx = -1;
double m_audio_duration = 0.023; 
int m_dwChannelCount = 2;                  
int m_dwBitsPerSample = 16;                 
int m_dwFrequency = 44100;                       
AVCodecID m_audio_codecID = AV_CODEC_ID_AAC;              
int m_audio_frame_size = 1024;   
int IsEnd_audio = 0;
unsigned int m_mute_Volume = 64;

int Open_codec_context(int * stream_idx, AVFormatContext * fmt_ctx, enum AVMediaType type)
{
    int ret;
    ret = av_find_best_stream(fmt_ctx, type, -1, -1, NULL, 0);
    if (ret < 0)
    {
        printf("Could not find %s stream in input file '%s'\n",av_get_media_type_string(type), INPUTFLVFILENAME);
        return ret;
    }
    else
    {
        *stream_idx = ret;
    }
    return 0;
}

//先创建一个函数来初始化队列
void packet_queue_init(PacketQueue *q) 
{
    memset(q, 0, sizeof(PacketQueue));
    q->mutex = SDL_CreateMutex();
    q->cond = SDL_CreateCond();
}

//来给队列中填入
int packet_queue_put(PacketQueue *q, AVPacket *pkt) 
{
    int ret;

    /* duplicate the packet */
    if (pkt != &flush_pkt && av_dup_packet(pkt) < 0)
    {
        return -1;
    }

    SDL_LockMutex(q->mutex);
    ret = packet_queue_put_private(q, pkt);
    SDL_UnlockMutex(q->mutex);

    if (pkt != &flush_pkt && ret < 0)
    {
        av_free_packet(pkt);
    }

    return ret;
}

void packet_queue_start(PacketQueue *q)
{
    SDL_LockMutex(q->mutex);
    packet_queue_put_private(q, &flush_pkt);
    SDL_UnlockMutex(q->mutex);
}


//从队列里面取出数据
int packet_queue_get(PacketQueue *q, AVPacket *pkt, int queue_type)
{
    AVPacketList *pkt1;
    int ret;

    SDL_LockMutex(q->mutex);

    for(;;)
    {
        pkt1 = q->first_pkt;
        if (pkt1)
        {
            q->first_pkt = pkt1->next;
            if (!q->first_pkt)
            {
                q->last_pkt = NULL;
            }
            q->nb_packets--;
            //q->size -= pkt1->pkt.size;
            q->size -= pkt1->pkt.size;
            *pkt = pkt1->pkt;
            av_free(pkt1);
            ret = 1;
            break;
        }
        else
        {
            int sdlcondwait = SDL_CondWait(q->cond, q->mutex);
        }
    }
    SDL_UnlockMutex(q->mutex);
    return ret;
}

void packet_queue_abort(PacketQueue *q)
{
    SDL_LockMutex(q->mutex);
    SDL_CondSignal(q->cond);
    SDL_UnlockMutex(q->mutex);
}

int packet_queue_put_private(PacketQueue *q, AVPacket *pkt)
{
    AVPacketList *pkt1;

    pkt1 = (AVPacketList * )av_malloc(sizeof(AVPacketList));
    if (!pkt1)
    {
        return -1;
    }
    pkt1->pkt = *pkt;
    pkt1->next = NULL;

    if (!q->last_pkt)
    {
        q->first_pkt = pkt1;
    }
    else
    {
        q->last_pkt->next = pkt1;
    }
    q->last_pkt = pkt1;
    q->nb_packets++;
    //q->size += pkt1->pkt.size + sizeof(*pkt1);
    q->size += pkt1->pkt.size;
    /* XXX: should duplicate packet data in DV case */
    SDL_CondSignal(q->cond);
    return 0;
}

void audio_callback(void * userdata, unsigned char *stream, int len) 
{
    VideoState *is = (VideoState *)userdata;
    int len1, audio_size;
    double pts;

    while(len > 0)
    {
        if(is->audio_buf_index >= is->audio_buf_size)
        {
            /* We have already sent all our data; get more */
            int size = sizeof(is->audio_buf);
            audio_size = audio_decode_frame(is, is->audio_buf, sizeof(is->audio_buf),&pts);
            if(audio_size < 0)
            {
                /* If error, output silence */
                is->audio_buf_size = 1024;
                memset(is->audio_buf, 0, is->audio_buf_size);
            }
            else
            {
                is->audio_buf_size = audio_size;
            }
            is->audio_buf_index = 0;
        }
        len1 = is->audio_buf_size - is->audio_buf_index;
        if(len1 > len)
        {
            len1 = len;
        }
        //memcpy(stream, (unsigned char  *)is->audio_buf + is->audio_buf_index, len1);
        //这里是控制音量大小的
        SDL_MixAudio(stream, (unsigned char  *)is->audio_buf + is->audio_buf_index, len1, m_mute_Volume);
        len -= len1;
        stream += len1;
        is->audio_buf_index += len1;
    }
}

Uint32 sdl_refresh_timer_cb(Uint32 interval, void *opaque) 
{
    SDL_Event event;
    event.type = FF_REFRESH_EVENT;
    event.user.data1 = opaque;
    SDL_PushEvent(&event);
    return 0; /* 0 means stop timer */
}

/* schedule a video refresh in 'delay' ms */
void schedule_refresh(VideoState *is, int delay) 
{
    SDL_AddTimer(delay, sdl_refresh_timer_cb, is);
}

int queue_picture(VideoState *is, AVFrame *pFrame,double pts) 
{
    VideoPicture * vp = NULL;
    AVFrame * pOutputFrame = NULL;
    uint8_t * pOutput_buf = NULL;
    int Out_size = 0;
    struct SwsContext * img_convert_ctx;

    pOutputFrame = av_frame_alloc();

    /* wait until we have space for a new pic */
    SDL_LockMutex(is->pictq_mutex);
    while (is->pictq_size >= VIDEO_PICTURE_QUEUE_SIZE )
    {
        SDL_CondWait(is->pictq_cond, is->pictq_mutex);
    }
    SDL_UnlockMutex(is->pictq_mutex);

    // windex is set to 0 initially
    vp = &is->pictq[is->pictq_windex];

    /* allocate or resize the buffer! */
    if(!vp->bmp ||
            vp->width != is->width ||
            vp->height != is->height)
    {
        SDL_Event event;

        vp->allocated = 0;
        /* we have to do it in the main thread */
        event.type = FF_ALLOC_EVENT;
        event.user.data1 = is;
        SDL_PushEvent(&event);

        /* wait until we have a picture allocated */
        SDL_LockMutex(is->pictq_mutex);
        while (!vp->allocated)
        {
            SDL_CondWait(is->pictq_cond, is->pictq_mutex);
        }
        /* if the queue is aborted, we have to pop the pending ALLOC event or wait for the allocation to complete */
        if (SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_EVENTMASK(FF_ALLOC_EVENT)) != 1)
        {
            while (!vp->allocated)
            {
                SDL_CondWait(is->pictq_cond, is->pictq_mutex);
            }
        }
        SDL_UnlockMutex(is->pictq_mutex);
    }
    /* We have a place to put our picture on the queue */

    if(vp->bmp)
    {

        SDL_LockYUVOverlay(vp->bmp);
        /* point pict at the queue */

        pOutputFrame->data[0] = vp->bmp->pixels[0];
        pOutputFrame->data[1] = vp->bmp->pixels[2];
        pOutputFrame->data[2] = vp->bmp->pixels[1];

        pOutputFrame->linesize[0] = vp->bmp->pitches[0];
        pOutputFrame->linesize[1] = vp->bmp->pitches[2];
        pOutputFrame->linesize[2] = vp->bmp->pitches[1];

        img_convert_ctx = sws_getContext(pFrame->width,pFrame->height, is->video_st->codec->pix_fmt,
                                         is->width,is->height,AV_PIX_FMT_YUV420P,SWS_BICUBIC, NULL,NULL,NULL);
        if(img_convert_ctx == NULL)
        {
            fprintf(stderr, "Cannot initialize the conversion context!\n");
            getchar();
        }

        // 将图片转换为RGB格式
        sws_scale(img_convert_ctx,pFrame->data,pFrame->linesize,0,
                  pFrame->height,pOutputFrame->data,pOutputFrame->linesize);

        SDL_UnlockYUVOverlay(vp->bmp);
        vp->pts = pts;
        /* now we inform our display thread that we have a pic ready */
        if(++is->pictq_windex == VIDEO_PICTURE_QUEUE_SIZE)
        {
            is->pictq_windex = 0;
        }
        SDL_LockMutex(is->pictq_mutex);
        is->pictq_size++;
        SDL_UnlockMutex(is->pictq_mutex);
    }
    return 0;
}

int audio_decode_frame(VideoState *is, uint8_t *audio_buf, int buf_size,double *pts_ptr) 
{
    int len1, data_size, n;
    AVPacket *pkt = &is->audio_pkt;
    is->audio_pkt_size  = 0;
    double pts;

    for(;;)
    {
        if (Isstop || IsEnd_audio )
        {
            return -1;
        }
        while(is->audio_pkt_size > 0)
        {
            data_size = buf_size;
            len1 = avcodec_decode_audio3(is->audio_st->codec, (int16_t *)audio_buf, &data_size,pkt);
            if(len1 < 0)
            {
                /* if error, skip frame */
                is->audio_pkt_size = 0;
                break;
            }
            is->audio_pkt_data += len1;
            is->audio_pkt_size -= len1;
            if(data_size <= 0)
            {
                /* No data yet, get more frames */
                continue;
            }
            pts = is->audio_clock;
            *pts_ptr = pts;

            //这里加一个判断是用于对不同的格式进行pts单位的统一化：统一到 “秒”，可扩展其它格式
            if (strstr(is->pFormatCtx->iformat->name,"mpegts")!= NULL)
            {
                double time_base = 90 * 1000;
                n = 2 * is->audio_st->codec->channels;
                double rate_size = data_size / n;
                is->audio_clock +=  rate_size * time_base/ is->audio_st->codec->sample_rate /100000;
            }
            else
            {
                n = 2 * is->audio_st->codec->channels;
                double rate_size = data_size / n;
                is->audio_clock +=  rate_size / is->audio_st->codec->sample_rate;
            }
            /* We have data, return it and come back for more later */
            return data_size;
        }
        if(pkt->data)
        {
            av_free_packet(pkt);
        }

        if (pause_play)
        {
            /* next packet */
            if(packet_queue_get(&is->audioq, pkt, AUDIO_ID) < 0)
            {
                return -1;
            }
            if(pkt->data == flush_pkt.data)
            {
                avcodec_flush_buffers(is->audio_st->codec);
                continue;
            }
            is->audio_pkt_data = pkt->data;
            is->audio_pkt_size = pkt->size;
        }
    }
}

/* called to display each frame */
void video_refresh(void * opaque)
{
    VideoState *is = (VideoState *)opaque;
    VideoPicture *vp;
    double delay, diff;

    if(is->video_st)
    {
        if(is->pictq_size == 0)
        {
            schedule_refresh(is, 1);
        }
        else
        {
            //取出要显示视频的图像数据
            vp = &is->pictq[is->pictq_rindex];
            //算出一帧视频本应该显示的时间
            delay = m_video_duration;
            /* update delay to sync to audio */
            diff = vp->pts - is->audio_clock;

            if(fabs(diff) < AV_NOSYNC_THRESHOLD) //求浮点数x的绝对值
            {
                if(diff <= -delay)
                {
                    delay = 0.01;       //如果 视频显示过慢，离音频 过于远 则 显示时间为10ms
                }
                else if(diff >= delay)
                {
                    delay = 2 * delay;  //如果 视频显示过快 则停留 两帧的时间
                }
            }
            //这里是刷新时间间隔
            schedule_refresh(is, delay * 1000);

            /* update queue for next picture! */
            if(++is->pictq_rindex == VIDEO_PICTURE_QUEUE_SIZE)
            {
                is->pictq_rindex = 0;
            }
            SDL_LockMutex(is->pictq_mutex);
            is->pictq_size--;
            SDL_CondSignal(is->pictq_cond);
            SDL_UnlockMutex(is->pictq_mutex);

            /* show the picture! */
            video_display(is);
        }
    }

    else if (is->audio_st)
    {
        /* draw the next audio frame */

        /* if only audio stream, then display the audio bars (better
           than nothing, just to test the implementation */
        video_display(is);
    }
    is->force_refresh = 0;
}

void alloc_picture(void *userdata) 
{
    VideoState *is = (VideoState *)userdata;
    VideoPicture *vp;

    vp = &is->pictq[is->pictq_windex];
    if(vp->bmp)
    {
        // we already have one make another, bigger/smaller
        SDL_FreeYUVOverlay(vp->bmp);
    }

    video_open(is);
    // Allocate a place to put our YUV image on that screen
    vp->bmp = SDL_CreateYUVOverlay(is->width,
                                   is->height,
                                   SDL_YV12_OVERLAY,
                                   screen);
    vp->width = is->width;
    vp->height = is->height;

    SDL_LockMutex(is->pictq_mutex);
    vp->allocated = 1;
    SDL_CondSignal(is->pictq_cond);
    SDL_UnlockMutex(is->pictq_mutex);
}


void video_display(VideoState *is) 
{
    SDL_Rect rect;
    VideoPicture *vp;

    if (!screen)
    {
        video_open(is);
    }
    vp = &is->pictq[is->pictq_rindex];
    if(vp->bmp)
    {
        if (vp->bmp->w != is->width)
        {
            //这里一定要注意 当sdl的窗口大小改变的时候 切换那一瞬间
            //is->pictq[is->pictq_rindex] 这里的数据有可能还是原来窗口大小的数据
            //这时将这一真不花到sdl窗口上防止窗口死掉。也可以用get_next_frame控制
            return ;
        }
        rect.x = 0;
        rect.y = 0;
        rect.w = is->width;
        rect.h = is->height;
        SDL_DisplayYUVOverlay(vp->bmp, &rect);
    }
}

int video_open(VideoState *is)
{
    int flags = SDL_HWSURFACE | SDL_ASYNCBLIT | SDL_HWACCEL;
    int w,h;
    VideoPicture *vp = &is->pictq[is->pictq_rindex];

    if (is_full_screen)
    {
        flags |= SDL_FULLSCREEN;
    }
    else
    {
        flags |= SDL_RESIZABLE;
    }

    if (is_full_screen && fs_screen_width)
    {
        w = fs_screen_width;
        h = fs_screen_height;
    }
    else if (!is_full_screen && screen_width)
    {
        w = screen_width;
        h = screen_height;
    }
    else
    {
        //视频本身的大小
        w = m_dwWidth;
        h = m_dwHeight;
    }
    if (screen && is->width == screen->w && screen->w == w
            && is->height== screen->h && screen->h == h)
    {
        return 0;
    }

    is->width  = w;
    is->height = h;


    screen = SDL_SetVideoMode(w, h, 0, flags);
    if (!screen)
    {
        fprintf(stderr, "SDL: could not set video mode - exiting\n");
        do_exit(is);
    }
    //添加标题
    if (!window_title)
    {
        window_title = filename;
    }
    SDL_WM_SetCaption(window_title, window_title);
    return 0;
}

int refresh_thread(void *opaque)
{
    VideoState * is = (VideoState *)opaque;
    while (!Isstop)
    {
        SDL_Event event;
        event.type = FF_REFRESH_EVENT;
        event.user.data1 = opaque;
        if (!is->refresh && (is->force_refresh))
        {
            is->refresh = 1;
            SDL_PushEvent(&event);
        }
        //FIXME ideally we should wait the correct time but SDLs event passing is so slow it would be silly
        //av_sleep(1000);
    }
    return 0;
}

int video_thread(void *arg)
{
    VideoState *is = (VideoState *)arg;
    //AVPacket pkt1, *packet = &pkt1;
    int frameFinished;
    AVFrame * pFrame = NULL;

    pFrame = av_frame_alloc();

    for(;;)
    {
        AVPacket pkt;
        if (Isstop)
        {
            break;
        }
        if (pause_play)
        {
            if(packet_queue_get(&is->videoq, &pkt, VIDEO_ID) < 0)
            {
                // means we quit getting packets
                break;
            }
            if(pkt.data == flush_pkt.data)
            {
                avcodec_flush_buffers(is->video_st->codec);
                continue;
            }
            // Decode video frame
            avcodec_decode_video2(is->video_st->codec, pFrame, &frameFinished,&pkt);
            av_free_packet(&pkt);

            // Did we get a video frame?
            if(frameFinished)
            {
                is->video_clock +=  m_video_duration;
                if(queue_picture(is, pFrame,is->video_clock) < 0)
                {
                    break;
                }
            }
        }
        else
        {
            continue;
        }
    }
    av_frame_free(&pFrame);
    return 0;
}

void packet_queue_flush(PacketQueue *q) 
{
    AVPacketList *pkt, *pkt1;

    SDL_LockMutex(q->mutex);
    for(pkt = q->first_pkt; pkt != NULL; pkt = pkt1)
    {
        pkt1 = pkt->next;
        av_free_packet(&pkt->pkt);
        av_freep(&pkt);
    }
    q->last_pkt = NULL;
    q->first_pkt = NULL;
    q->nb_packets = 0;
    q->size = 0;
    SDL_UnlockMutex(q->mutex);
}

void packet_queue_destroy(PacketQueue *q)
{
    packet_queue_flush(q);
    SDL_DestroyMutex(q->mutex);
    SDL_DestroyCond(q->cond);
}

void do_seek()
{
    is->seek_req = 1;
    is->seek_pos = is->pictq[is->pictq_rindex].pts + is->seek_time;
    is->seek_flags = is->seek_time < 0 ? AVSEEK_FLAG_BACKWARD : 0;
}

void do_exit(VideoState *is)
{
    VideoPicture *vp;
    int i;

    Isstop = 1;

    //释放packet_queue
    SDL_WaitThread(is->read_tid,NULL);
    SDL_WaitThread(is->refresh_tid,NULL);
    packet_queue_destroy(&is->videoq);
    packet_queue_destroy(&is->audioq);
    /* free all pictures */
    for (i = 0; i < VIDEO_PICTURE_QUEUE_SIZE; i++)
    {
        vp = &is->pictq[i];
        if (vp->bmp)
        {
            SDL_FreeYUVOverlay(vp->bmp);
            vp->bmp = NULL;
        }
    }

    SDL_Quit();
}

void do_full_screen(VideoState *is)
{
    is_full_screen = !is_full_screen;
    video_open(is);
    is->force_refresh = 1;
}

void do_resize_screen(VideoState *is,int width,int height)
{
    //这里做判断是因为 如果改变大小时 的 宽 和高 出现 奇数则图像会不正确 或 中断 要偶数
    if (width  % 2 != 0 )
    {
        width = width - 1;
    }
    if (height  % 2 != 0 )
    {
        height = height - 1;
    }
    screen = SDL_SetVideoMode(width, height, 0,
                              SDL_HWSURFACE|SDL_RESIZABLE|SDL_ASYNCBLIT|SDL_HWACCEL);
    screen_width  = is->width  = width;
    screen_height = is->height = height;
    is->force_refresh = 1;
}

void do_pause(VideoState *is)
{
    //这里做判断是因为当暂停 时 声音会一直播放暂停前的一帧音频数据
    if (pause_play)
    {
        //暂停
        SDL_PauseAudio(1);
    }
    else
    {
        //播放
        SDL_PauseAudio(0);
    }
    pause_play = !pause_play;
}

void do_stop(VideoState *is)
{
    VideoPicture *vp;
    int i;

    Isstop = 1;

    //释放packet_queue
    SDL_WaitThread(is->read_tid,NULL);
    SDL_WaitThread(is->refresh_tid,NULL);
    packet_queue_destroy(&is->videoq);
    packet_queue_destroy(&is->audioq);
    /* free all pictures */
    for (i = 0; i < VIDEO_PICTURE_QUEUE_SIZE; i++)
    {
        vp = &is->pictq[i];
        if (vp->bmp)
        {
            SDL_FreeYUVOverlay(vp->bmp);
            vp->bmp = NULL;
        }
    }
    SDL_Quit();
}


int InitProgram()
{
    int flags;
    /* register all codecs, demux and protocols */
    av_register_all();
    avcodec_register_all();
    avformat_network_init();

    //设置SDL窗口居中
    putenv ("SDL_VIDEO_WINDOW_POS");
    putenv ("SDL_VIDEO_CENTERED=1");

    //SDL初始化
    flags = SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER;
    if (SDL_Init (flags))
    {
        fprintf(stderr, "Could not initialize SDL - %s\n", SDL_GetError());
        fprintf(stderr, "(Did you set the DISPLAY variable?)\n");
        return -1;
    }

    //获取全屏的屏幕宽高
    const SDL_VideoInfo *vi = SDL_GetVideoInfo();
    fs_screen_width = vi->current_w;
    fs_screen_height = vi->current_h;

    //设置SDL事件状态
    SDL_EventState(SDL_ACTIVEEVENT, SDL_IGNORE);
    SDL_EventState(SDL_SYSWMEVENT, SDL_IGNORE);
    SDL_EventState(SDL_USEREVENT, SDL_IGNORE);
    
    //刷新packet初始化
    av_init_packet(&flush_pkt);
    flush_pkt.data = (uint8_t *)(intptr_t)"FLUSH";

    //音视频全局结构体初始化
    is = (VideoState *)calloc(1,sizeof(VideoState));
    if (!is)
    {
        return -1;
    }

    //设置SDL 互斥变量 条件变量
    is->pictq_mutex = SDL_CreateMutex();
    is->pictq_cond  = SDL_CreateCond();

    //创建接收显示视频和 消息循环的窗口
    //MFC中不必创建 这里只是为了能接收到消息循环
    video_open(is);
    //输出命令字符：MFC则不用，用按钮替代
    printf("按键  'L' --------加载要播放的文件\n");
    printf("按键  'D' --------开始播放文件\n");
    printf("按键  'P' --------暂停播放文件\n");
    printf("按键  'S' --------停止播放文件销毁窗口\n");
    printf("按键  'F' --------全屏/退出全屏播放\n");
    printf("按键  'Q' --------退出程序\n");
    printf("按键  'M' --------增加音量\n");
    printf("按键  'N' --------减小音量\n");
    printf("拖动'边框'--------改变窗口大小\n");
    return 1;
}

int UinitProgram()
{
    //释放互斥变量 条件变量
    SDL_DestroyMutex(is->pictq_mutex);
    SDL_DestroyCond(is->pictq_cond);
    //音视频全局结构体析构
    av_free(is);
    avformat_network_deinit();
    return 1;
}
int stream_component_open(VideoState *is, int stream_index)
{
    AVFormatContext *ic = is->pFormatCtx;
    SDL_AudioSpec wanted_spec, spec;

    if (stream_index < 0 || stream_index >= ic->nb_streams)
    {
        return -1;
    }

    ic->streams[stream_index]->discard = AVDISCARD_DEFAULT;

    /* prepare audio output */
    if (stream_index == m_audio_stream_idx)
    {
        if (!paudio_codec || avcodec_open2(audio_dec_ctx, paudio_codec,NULL) < 0)
        {
            return -1;
        }

        // Set audio settings from codec info
        wanted_spec.freq = m_dwFrequency;

        switch (m_dwBitsPerSample)
        {
        case 8:
            wanted_spec.format = AUDIO_S8;
            break;
        case 16:
            wanted_spec.format = AUDIO_S16SYS;
            break;
        default:
            wanted_spec.format = AUDIO_S16SYS;
            break;
        }
        wanted_spec.channels = m_dwChannelCount;
        wanted_spec.silence = 0;//silence值，由于为signed，故为0 信号
        wanted_spec.samples = SDL_AUDIO_BUFFER_SIZE;
        wanted_spec.callback = audio_callback;
        wanted_spec.userdata = is;
        wanted_spec.size = 0;  //SDL_OpenAudio().调用时计算得到

        //打开音频设备
        if(SDL_OpenAudio(&wanted_spec, &spec) < 0)
        {
            fprintf(stderr, "SDL_OpenAudio: %s\n", SDL_GetError());
            return -1;
        }

        if (spec.format != AUDIO_S16SYS)
        {
            fprintf(stderr, "SDL advised audio format %d is not supported!\n", spec.format);
            return -1;
        }
        is->audio_st = ic->streams[stream_index];
        is->audio_buf_size  = 0;
        is->audio_buf_index = 0;
        memset(&is->audio_pkt, 0, sizeof(is->audio_pkt));
        packet_queue_init(&is->audioq);
        packet_queue_start(&is->audioq);
        SDL_PauseAudio(0);
    }
    else if (stream_index == m_video_stream_idx)
    {
        if (!pvideo_codec || avcodec_open2(video_dec_ctx, pvideo_codec,NULL) < 0)
        {
            return -1;
        }
        is->video_st = ic->streams[stream_index];
        packet_queue_init(&is->videoq);
        packet_queue_start(&is->videoq);
        is->video_tid = SDL_CreateThread(video_thread, is);
    }
    return 1;
}

int stream_component_close(VideoState *is, int stream_index)
{
    AVFormatContext *ic = is->pFormatCtx;

    if (stream_index < 0 || stream_index >= ic->nb_streams)
    {
        return - 1;
    }

    ic->streams[stream_index]->discard = AVDISCARD_ALL;

    if (stream_index == m_audio_stream_idx)
    {
        packet_queue_abort(&is->audioq);
        SDL_CloseAudio();
        packet_queue_flush(&is->audioq);
        av_free_packet(&is->audio_pkt);
        is->audio_st = NULL;
        avcodec_close(audio_dec_ctx);
    }
    else if (stream_index == m_video_stream_idx)
    {
        packet_queue_abort(&is->videoq);
        /* note: we also signal this mutex to make sure we deblock the
           video thread in all cases */
        SDL_LockMutex(is->pictq_mutex);
        SDL_CondSignal(is->pictq_cond);
        SDL_UnlockMutex(is->pictq_mutex);
        SDL_KillThread(is->video_tid);
        packet_queue_flush(&is->videoq);
        is->video_st = NULL;
        avcodec_close(video_dec_ctx);
    }
    return 1;
}

int read_thread(void *arg)
{
    int ret = -1;
    int error = -1;
    int eof = 0;
    AVPacket pkt1, *pkt = &pkt1;

    //创建刷新线程
    is->refresh_tid = SDL_CreateThread(refresh_thread, is);

    /* open the streams */
    if (AVMEDIA_TYPE_AUDIO >= 0)
    {
        ret = stream_component_open(is, AVMEDIA_TYPE_AUDIO);
    }
    if (AVMEDIA_TYPE_VIDEO >= 0)
    {
        ret = stream_component_open(is, AVMEDIA_TYPE_VIDEO);
    }

    if (m_video_stream_idx < 0 && m_audio_stream_idx < 0)
    {
        fprintf(stderr, "%s: could not open codecs\n", filename);
        ret = -1;
        goto fail;
    }

    for(;;)
    {
        if (Isstop)
        {
            break;
        }
        // seek stuff goes here
        if(is->seek_req)
        {
            int stream_index = -1;
            //转化成纳秒
            int64_t seek_target = is->seek_pos * 1000000;

            if(m_video_stream_idx >= 0)
            {
                stream_index = m_video_stream_idx;
            }
            else if(m_audio_stream_idx >= 0)
            {
                stream_index = m_audio_stream_idx;
            }
            if(stream_index >= 0)
            {
                AVRational time_base_q ;
                time_base_q.num = 1;
                time_base_q.den = AV_TIME_BASE;
                //这里一定要注意：不单纯的是从秒转成毫秒，//seek_target = seek_target / 1000; 这样做是不对的
                seek_target = av_rescale_q(seek_target, time_base_q, is->pFormatCtx->streams[stream_index]->time_base);
            }
            error = av_seek_frame(is->pFormatCtx, stream_index, seek_target, is->seek_flags);
            if( error < 0)
            {
                fprintf(stderr, "%s: error while seeking\n", is->pFormatCtx->filename);
            }
            else
            {
                if(m_audio_stream_idx >= 0)
                {
                    packet_queue_flush(&is->audioq);
                    packet_queue_put(&is->audioq, &flush_pkt);
                }
                if(m_video_stream_idx >= 0)
                {
                    packet_queue_flush(&is->videoq);
                    packet_queue_put(&is->videoq, &flush_pkt);
                }
                //这里要重新这是video_clock 和 audio_clock
                is->video_clock += ((int)(is->seek_time /m_video_duration) + 1)  * m_video_duration ;
                if (strstr(is->pFormatCtx->iformat->name,"mpegts")!= NULL)
                {
                    double time_base = 90 * 1000;
                    int count = (int)(is->seek_time /(1024.0 * time_base/ is->audio_st->codec->sample_rate /100000)) + 1;
                    is->audio_clock +=  count * (1024.0 * time_base/ is->audio_st->codec->sample_rate /100000);
                }
                else
                {
                    double time_base = 90 * 1000;
                    //is->audio_clock +=  ((int)(is->seek_time/(1024 / is->audio_st->codec->sample_rate)) + 1) * (1024 / is->audio_st->codec->sample_rate);
                    int count = (int)(is->seek_time /(1024.0 / is->audio_st->codec->sample_rate)) + 1;

                    is->audio_clock +=  count * (1024.0 / is->audio_st->codec->sample_rate);
                }
            }
            is->seek_req = 0;
            is->seek_time = 0;
            eof = 0;
        }

        /* if the queue are full, no need to read more */
        if (   is->audioq.size + is->videoq.size > MAX_QUEUE_SIZE
               || (   (is->audioq.nb_packets > MIN_FRAMES )
                      && (is->videoq.nb_packets > MIN_FRAMES)))
        {
            /* wait 10 ms */
            SDL_Delay(10);
            continue;
        }

        if (eof)
        {
            //到文件末尾 放入队列一个空包
            if (m_video_stream_idx >= 0)
            {
                av_init_packet(pkt);
                pkt->data = NULL;
                pkt->size = 0;
                pkt->stream_index = m_video_stream_idx;
                packet_queue_put(&is->videoq, pkt);
            }
            if (m_audio_stream_idx >= 0 )
            {
                if (is->audioq.nb_packets == 0)
                {
                    IsEnd_audio = 1;
                }
                av_init_packet(pkt);
                pkt->data = NULL;
                pkt->size = 0;
                pkt->stream_index = m_audio_stream_idx;
                packet_queue_put(&is->audioq, pkt);
            }
            SDL_Delay(10);
            if (is->audioq.size + is->videoq.size == 0)
            {
                ret = AVERROR_EOF;
                goto fail;
            }
            eof = 0;
            continue;
        }
        //这里确定文件到了末尾
        ret = av_read_frame(is->pFormatCtx, pkt);
        if (ret < 0)
        {
            if (ret == AVERROR_EOF || url_feof(is->pFormatCtx->pb))
            {
                eof = 1;
            }
            if (is->pFormatCtx->pb && is->pFormatCtx->pb->error)
            {
                break;
            }
            SDL_Delay(100); /* wait for user event */
            continue;
        }
        // Is this a packet from the video stream?
        if(pkt->stream_index == m_video_stream_idx)
        {
            packet_queue_put(&is->videoq, pkt);
        }
        else if(pkt->stream_index == m_audio_stream_idx)
        {
            packet_queue_put(&is->audioq, pkt);
        }
        else
        {
            av_free_packet(pkt);
        }
    }
    ret = 0;

fail:
    /* close each stream */
    if (m_audio_stream_idx >= 0)
    {
        stream_component_close(is, m_audio_stream_idx);
    }
    if (m_video_stream_idx >= 0)
    {
        stream_component_close(is,m_video_stream_idx);
    }
    if (is->pFormatCtx)
    {
        avformat_close_input(&is->pFormatCtx);
    }
    if (ret == AVERROR_EOF)
    {
        SDL_Event event;
        event.type = FF_QUIT_EVENT;
        event.user.data1 = is;
        SDL_PushEvent(&event);
    }
    return 1;
}

void do_load(char * pathname)
{
    //打开要别应用的输入文件
    if (avformat_open_input(&is->pFormatCtx, pathname, NULL, NULL) < 0)
    {
        printf("Could not open source file %s\n", pathname);
        getchar();
    }

    //查看流信息
    if (avformat_find_stream_info(is->pFormatCtx, NULL) < 0)
    {
        printf("Could not find stream information\n");
        getchar();
    }
    //输入信息错误写入
    av_dump_format(is->pFormatCtx, 0, (const char *)pathname, 0);

    //得到视频信息的CONTEXT
    if (Open_codec_context(&m_video_stream_idx, is->pFormatCtx, AVMEDIA_TYPE_VIDEO) >= 0)
    {
        is->video_st = is->pFormatCtx->streams[m_video_stream_idx];
        video_dec_ctx = is->video_st->codec;
        // Find the decoder for the video stream
        pvideo_codec = avcodec_find_decoder(video_dec_ctx->codec_id);
        if(pvideo_codec == NULL)
        {
            fprintf(stderr, "Unsupported codec!\n");
            getchar(); // Codec not found
        }
        // Open codec
        if(avcodec_open2(video_dec_ctx, pvideo_codec,NULL) < 0)
        {
            getchar(); // Could not open codec
        }

        //视频的帧率
        m_dbFrameRate = av_q2d(is->video_st->r_frame_rate);
        //宽高,视频编码类型,视频yuv类型,spspps_buf,spspps_size
        m_dwWidth = is->pFormatCtx->streams[m_video_stream_idx]->codec->width;
        m_dwHeight = is->pFormatCtx->streams[m_video_stream_idx]->codec->height;
        m_video_codecID = is->pFormatCtx->streams[m_video_stream_idx]->codec->codec_id;
        m_video_pixelfromat = is->pFormatCtx->streams[m_video_stream_idx]->codec->pix_fmt;
        m_spspps_size = is->pFormatCtx->streams[m_video_stream_idx]->codec->extradata_size;
        memcpy(m_spspps,is->pFormatCtx->streams[m_video_stream_idx]->codec->extradata,m_spspps_size);

        //这里加一个判断是用于对不同的格式进行pts单位的统一化：统一到 “秒”,可扩展其它格式
        if (strstr(is->pFormatCtx->iformat->name,"mpegts")!= NULL)
        {
            double time_base = 90 * 1000;
            m_video_duration = time_base / m_dbFrameRate /100000;
        }
        else
        {
            m_video_duration = 1 / m_dbFrameRate;
        }
    }

    //得到音频信息的CONTEXT
    if (Open_codec_context(&m_audio_stream_idx, is->pFormatCtx, AVMEDIA_TYPE_AUDIO) >= 0)
    {
        is->audio_st = is->pFormatCtx->streams[m_audio_stream_idx];
        audio_dec_ctx = is->audio_st->codec;
        // Find the decoder for the video stream
        paudio_codec = avcodec_find_decoder(audio_dec_ctx->codec_id);
        if(paudio_codec == NULL)
        {
            fprintf(stderr, "Unsupported codec!\n");
            getchar(); // Codec not found
        }

        // Open codec
        if(avcodec_open2(audio_dec_ctx, paudio_codec,NULL) < 0)
        {
            getchar(); // Could not open codec
        }

        //声道，样本，采样率，视频编码类型，一帧数据大小
        m_dwChannelCount = is->pFormatCtx->streams[m_audio_stream_idx]->codec->channels;
        switch (is->pFormatCtx->streams[m_audio_stream_idx]->codec->sample_fmt)
        {
        case AV_SAMPLE_FMT_U8:
            m_dwBitsPerSample  = 8;
            break;
        case AV_SAMPLE_FMT_S16:
            m_dwBitsPerSample  = 16;
            break;
        case AV_SAMPLE_FMT_S32:
            m_dwBitsPerSample  = 32;
            break;
        default:
            break;
        }
        m_dwFrequency = is->pFormatCtx->streams[m_audio_stream_idx]->codec->sample_rate;
        m_audio_codecID = is->pFormatCtx->streams[m_audio_stream_idx]->codec->codec_id;
        m_audio_frame_size = is->pFormatCtx->streams[m_audio_stream_idx]->codec->frame_size;

        //这里加一个判断是用于对不同的格式进行pts单位的统一化：统一到 “秒”,可扩展其它格式
        if (strstr(is->pFormatCtx->iformat->name,"mpegts")!= NULL)
        {
            double time_base = 90 * 1000;
            m_audio_duration = time_base * m_audio_frame_size / m_dwFrequency /100000;
        }
        else
        {
            double time_base = 1.0;
            m_audio_duration = time_base * m_audio_frame_size / m_dwFrequency;
        }
    }
}


void do_play()
{
    is->read_tid  = SDL_CreateThread(read_thread, is);
    if (!is->read_tid)
    {
        printf("SDL_CreateThread :read_thread error\n");
        av_free(is);
        getchar();
    }
}

int event_loop()
{
    SDL_Event  event;
    for(;;)
    {
        double incr, pos;
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
            case SDLK_l:     //加载文件
                if (!IsLoad) //如果已经加载了文件
                {
                    do_load(filename);
                    IsLoad = 1;
                }
                else
                {
                    printf("文件已经加载.........\n");
                }
                break;
            case SDLK_d:   //播放
                if (IsLoad) //如果加载的文件才能播放
                {
                    if (!Isplay) //如果已经再播放
                    {
                        do_play();
                        Isplay = 1;
                    }
                    else
                    {
                        printf("文件正在播放.........\n");
                    }
                }
                else
                {
                    printf("请先加载文件.........\n");
                }
                break;
            case SDLK_p:   //播放暂停
            case SDLK_SPACE:
                do_pause(is);//这里无论文件播放或未加载 或途中都可以暂停
                break;
            case SDLK_s:   //停止
                if (!Isstop)
                {
                    do_stop(is);
                    break;
                }
                else
                {
                    goto end;
                }
            case SDLK_q:   //退出
                if (!Isexit)
                {
                    do_exit(is);
                    Isexit = 1;
                    break;
                }
                else
                {
                    goto end;
                }
            case SDLK_f:   //全屏
                do_full_screen(is);
                break;
            case SDLK_m:
                m_mute_Volume  += 10;
                if (m_mute_Volume > 128)
                {
                    m_mute_Volume = 128;
                }
                break;
            case SDLK_n:
                m_mute_Volume -= 10;
                if (m_mute_Volume < 0)
                {
                    m_mute_Volume = 0;
                }
                break;
            case SDLK_LEFT:
                is->seek_time = -10.0;
                goto do_seek;
            case SDLK_RIGHT:
                is->seek_time = 10.0;
                goto do_seek;
            case SDLK_UP:
                is->seek_time = 40.0;
                goto do_seek;
            case SDLK_DOWN:
                is->seek_time = - 40.0;
                goto do_seek;
do_seek:
                do_seek();
                break;
            default:
                break;
            }
            break;
        case SDL_VIDEORESIZE:    //改变窗口大小
            if (!is_full_screen) //全屏的时候会跳到这里 所以限制一下
            {
                do_resize_screen(is,event.resize.w,event.resize.h);
            }
            break;
        case FF_QUIT_EVENT:
        case SDL_QUIT:
            if (!Isexit)
            {
                do_exit(is);
                Isexit = 1;
                break;
            }
            else
            {
                goto end;
            }
        case FF_ALLOC_EVENT:
            alloc_picture(event.user.data1);
            break;
        case FF_REFRESH_EVENT:
            video_refresh(event.user.data1);
            is->refresh = 0;
            break;
        default:
            break;
        }
    }
end :
    return 1;
}
