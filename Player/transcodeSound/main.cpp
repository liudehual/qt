/*
*简单音频转码器（只处理音频）
*本程序实现从一个视频格式转码到另一个视频格式，只处理音频，视频忽略，若有多个音频流，只处理第一个,采用swr_convert重取样
*/

#ifdef __cplusplus
extern"C"
{
#endif
#include <libavformat/avformat.h>
#include "libavcodec/avcodec.h"
#include "libavfilter/avfiltergraph.h"
#include "libavfilter/buffersink.h"
#include "libavfilter/buffersrc.h"
#include "libavutil/avutil.h"
#include "libavutil/opt.h"
#include "libavutil/pixdesc.h"
#include "libswresample/swresample.h"
#include "libavutil/fifo.h"
#ifdef __cplusplus
}
#endif

#define BUF_SIZE_20K 2048000
#define BUF_SIZE_1K 1024000

SwrContext* pSwrCtx = NULL;
AVStream *out_stream = NULL;
AVFormatContext *in_fmt_ctx = NULL, *out_fmt_ctx = NULL;
int audio_index = -1;

void initSwr()
{
    if (out_fmt_ctx->streams[0]->codec->channels != in_fmt_ctx->streams[audio_index]->codec->channels
            || out_fmt_ctx->streams[0]->codec->sample_rate != in_fmt_ctx->streams[audio_index]->codec->sample_rate
            || out_fmt_ctx->streams[0]->codec->sample_fmt != in_fmt_ctx->streams[audio_index]->codec->sample_fmt){

        if ( NULL == pSwrCtx ){
            pSwrCtx = swr_alloc();
        }

        pSwrCtx = swr_alloc_set_opts(pSwrCtx,out_fmt_ctx->streams[0]->codec->channel_layout,
                out_fmt_ctx->streams[0]->codec->sample_fmt,
                out_fmt_ctx->streams[0]->codec->sample_rate,
                in_fmt_ctx->streams[audio_index]->codec->channel_layout,
                in_fmt_ctx->streams[audio_index]->codec->sample_fmt,
                in_fmt_ctx->streams[audio_index]->codec->sample_rate,
                0, NULL);

        swr_init(pSwrCtx);
    }
}

//setup_array函数摘自ffmpeg例程
static void setup_array(uint8_t* out[3], AVFrame* in_frame, int format, int samples)
{
    if (av_sample_fmt_is_planar((AVSampleFormat)format)){
        int i;int plane_size = av_get_bytes_per_sample((AVSampleFormat)(format & 0xFF)) * samples;format &= 0xFF;

        //从decoder出来的frame中的data数据不是连续分布的，所以不能这样写：
        in_frame->data[0]+i*plane_size;
        for (i = 0; i < in_frame->channels; i++){
            out[i] = in_frame->data[i];
        }
    }else{
        out[0] = in_frame->data[0];
    }
}

int TransSample(AVFrame *in_frame, AVFrame *out_frame)
{
    int ret;
    int max_dst_nb_samples = 4096;
    //int64_t dst_nb_samples;
    int64_t src_nb_samples = in_frame->nb_samples;
    out_frame->pts = in_frame->pts;
    int len;
    if (pSwrCtx != NULL){
        out_frame->nb_samples = av_rescale_rnd(swr_get_delay(pSwrCtx,
                                                             out_fmt_ctx->streams[0]->codec->sample_rate) + src_nb_samples,
                out_fmt_ctx->streams[0]->codec->sample_rate,
                in_fmt_ctx->streams[audio_index]->codec->sample_rate,
                AV_ROUND_UP);

        ret = av_samples_alloc(out_frame->data,
                               &out_frame->linesize[0],
                out_fmt_ctx->streams[0]->codec->channels,
                out_frame->nb_samples,
                out_fmt_ctx->streams[0]->codec->sample_fmt, 0);

        if (ret < 0){
            av_log(NULL, AV_LOG_WARNING, "[%s.%d %s() Could not allocate samples Buffer\n", __FILE__, __LINE__, __FUNCTION__);
            return -1;
        }

        max_dst_nb_samples = out_frame->nb_samples;

        //输入也可能是分平面的，所以要做如下处理
        uint8_t* m_ain[3];
        setup_array(m_ain, in_frame, in_fmt_ctx->streams[audio_index]->codec->sample_fmt, src_nb_samples);

        //注意这里，out_count和in_count是samples单位，不是byte
        //所以这样av_get_bytes_per_sample(in_fmt_ctx->streams[audio_index]->codec->sample_fmt) * src_nb_samples是错的
        len = swr_convert(pSwrCtx, out_frame->data, out_frame->nb_samples, (const uint8_t**)m_ain, src_nb_samples);
        if (len < 0){
            char errmsg[BUF_SIZE_1K];
            av_strerror(len, errmsg, sizeof(errmsg));
            av_log(NULL, AV_LOG_WARNING, "[%s:%d] swr_convert!(%d)(%s)", __FILE__, __LINE__, len, errmsg);
            return -1;
        }
    }else{
        printf("pSwrCtx with out init!\n");
        return -1;
    }
    return 0;
}

int flush_encoder(AVFormatContext *fmt_ctx, unsigned int stream_index)
{
    int ret;
    int got_frame;
    AVPacket enc_pkt;
    if (!(fmt_ctx->streams[stream_index]->codec->codec->capabilities & CODEC_CAP_DELAY)){
        return 0;
    }

    int i = 0;
    while(1)
    {
        enc_pkt.data = NULL;
        enc_pkt.size = 0;
        av_init_packet(&enc_pkt);
        ret = avcodec_encode_audio2(out_fmt_ctx->streams[stream_index]->codec, &enc_pkt,NULL, &got_frame);
        if (ret < 0){
            break;
        }

        if (!got_frame){
            break;
        }

        /* prepare packet for muxing */
        enc_pkt.stream_index = stream_index;

        enc_pkt.dts = av_rescale_q_rnd(enc_pkt.dts,out_fmt_ctx->streams[stream_index]->codec->time_base,
                                       out_fmt_ctx->streams[stream_index]->time_base,
                                       (AVRounding)(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));

        enc_pkt.pts = av_rescale_q_rnd(enc_pkt.pts,out_fmt_ctx->streams[stream_index]->codec->time_base,
                                       out_fmt_ctx->streams[stream_index]->time_base,
                                       (AVRounding)(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));

        enc_pkt.duration = av_rescale_q(enc_pkt.duration,out_fmt_ctx->streams[stream_index]->codec->time_base,
                                        out_fmt_ctx->streams[stream_index]->time_base);

        /* mux encoded frame */
        ret = av_interleaved_write_frame(out_fmt_ctx, &enc_pkt);

        av_free_packet(&enc_pkt);

        if (ret < 0){
            break;
        }
        i++;
    }

    printf("flusher write %d frame", i);

    return ret;
}

void usage()
{
    av_log(NULL,AV_LOG_INFO,"Usge: \n"
           "./transcodeSound inputFile outputFile \n");
}

int main(int argc, char* argv[])
{
    AVFrame *frame, *frame_out;
    AVPacket pkt_in, pkt_out;
    int ret;

    if (argc < 3){
        usage();
        return -1;
    }

    av_register_all();
    avfilter_register_all();

    do{
        //input
        if (avformat_open_input(&in_fmt_ctx, argv[1], NULL, NULL) < 0){
            printf("can not open input file context");
            break;
        }

        if (avformat_find_stream_info(in_fmt_ctx, NULL) < 0){
            printf("can not find input stream info!\n");
            break;
        }

        //output
        avformat_alloc_output_context2(&out_fmt_ctx, NULL, NULL, argv[2]);
        if(!out_fmt_ctx){
            printf("can not alloc output context!\n");
            break;
        }

        //open decoder & new out stream & open encoder
        for (int i = 0; i < in_fmt_ctx->nb_streams; i++){
            if (in_fmt_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO){
                //open decoder
                if(0 > avcodec_open2(in_fmt_ctx->streams[i]->codec,
                                     avcodec_find_decoder(in_fmt_ctx->streams[i]->codec->codec_id),
                                     NULL)){
                    printf("can not find or open decoder!\n");
                    break;
                }
                audio_index = i;

                //new stream
                out_stream = avformat_new_stream(out_fmt_ctx, NULL);
                if (!out_stream){
                    printf("can not new stream for output!\n");
                    break;
                }
                //set codec context param
                //use default audio encoder
                out_stream->codec->codec = avcodec_find_encoder(out_fmt_ctx->oformat->audio_codec);
                //use the input audio encoder
                //
                out_stream->codec->codec = avcodec_find_encoder(in_fmt_ctx->streams[i]->codec->codec_id);
                out_stream->codec->sample_rate = in_fmt_ctx->streams[i]->codec->sample_rate;
                out_stream->codec->channel_layout = in_fmt_ctx->streams[i]->codec->channel_layout;
                out_stream->codec->channels = av_get_channel_layout_nb_channels(out_stream->codec->channel_layout);

                // take first format from list of supported formats
                out_stream->codec->sample_fmt = out_stream->codec->codec->sample_fmts[0];
                AVRational time_base={1, out_stream->codec->sample_rate};
                out_stream->codec->time_base = time_base;

                //open encoder
                if (!out_stream->codec->codec){
                    printf("can not find the encoder!\n");
                    break;
                }
                if ((avcodec_open2(out_stream->codec, out_stream->codec->codec, NULL)) < 0){
                    printf("can not open the encoder\n");
                    break;
                }
                if (out_fmt_ctx->oformat->flags & AVFMT_GLOBALHEADER)
                    out_stream->codec->flags |= CODEC_FLAG_GLOBAL_HEADER;
                break;
            }
        }
        //dump input info
        av_dump_format(in_fmt_ctx, 0, argv[1], 0);
        //dump output info
        av_dump_format(out_fmt_ctx, 0, argv[2], 1);
        if (-1 == audio_index){
            printf("found no audio stream in input file!\n");
            break;
        }
        if (!(out_fmt_ctx->oformat->flags & AVFMT_NOFILE)){
            if(avio_open(&out_fmt_ctx->pb, argv[2], AVIO_FLAG_WRITE) < 0){
                printf("can not open output file handle!\n");
                break;
            }
        }

        if(avformat_write_header(out_fmt_ctx, NULL) < 0){
            printf("can not write the header of the output file!\n");
            break;
        }
        //
        initSwr();
        int got_frame, got_picture;
        int frame_index = 0;
        int i = 0;

        for(;;){
            pkt_in.data = NULL;
            pkt_in.size = 0;
            got_frame = -1;
            got_picture = -1;
            if (av_read_frame(in_fmt_ctx, &pkt_in) < 0){
                break;
            }

            if (pkt_in.stream_index != audio_index){
                continue;
            }

            frame = av_frame_alloc();

            if ((ret = avcodec_decode_audio4(in_fmt_ctx->streams[audio_index]->codec,
                                             frame,
                                             &got_frame,
                                             &pkt_in)) < 0){
                av_frame_free(&frame);
                printf("can not decoder a frame");
                break;
            }

            av_free_packet(&pkt_in);

            if (got_frame){
                frame->pts = av_frame_get_best_effort_timestamp(frame);
                frame_out = av_frame_alloc();
                if (0 != TransSample(frame, frame_out)){
                    printf("can not swr the audio data!\n");
                    break;
                }

                av_init_packet(&pkt_out);
                int ret = avcodec_encode_audio2(out_fmt_ctx->streams[0]->codec, &pkt_out, frame_out, &got_picture);

                av_free(frame_out->data[0]);
                av_frame_free(&frame_out);

                if (got_picture ){
                    pkt_out.stream_index = out_stream->index;

                    pkt_out.dts = av_rescale_q_rnd(pkt_out.dts,
                                                   out_fmt_ctx->streams[out_stream->index]->codec->time_base,
                            out_fmt_ctx->streams[out_stream->index]->time_base,
                            (AVRounding)(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));

                    pkt_out.pts = av_rescale_q_rnd(pkt_out.pts,
                                                   out_fmt_ctx->streams[out_stream->index]->codec->time_base,
                            out_fmt_ctx->streams[out_stream->index]->time_base,
                            (AVRounding)(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));

                    pkt_out.duration = av_rescale_q(pkt_out.duration,
                                                    out_fmt_ctx->streams[out_stream->index]->codec->time_base,
                            out_fmt_ctx->streams[out_stream->index]->time_base);

                    av_log(NULL, AV_LOG_DEBUG, "Muxing frame\n");

                    /* mux encoded frame */
                    ret = av_interleaved_write_frame(out_fmt_ctx, &pkt_out);
                    av_free_packet(&pkt_out);

                    if (ret < 0){
                        printf("write a frame failed!\n");
                        break;
                    }

                    printf("success write a frame:index %d\n", frame_index++);
                }
            }
        }

        ret = flush_encoder(out_fmt_ctx, out_stream->index);
        if (ret < 0){
            printf("Flushing encoder failed");
            return -1;
        }

        //write file trailer
        av_write_trailer(out_fmt_ctx);

        //clean
        avcodec_close(out_stream->codec);
        avcodec_close(in_fmt_ctx->streams[audio_index]->codec);
    }while(0);

    avformat_close_input(&in_fmt_ctx);
    if (out_fmt_ctx && !(out_fmt_ctx->oformat->flags & AVFMT_NOFILE)){
        avio_close(out_fmt_ctx->pb);
    }
    avformat_free_context(out_fmt_ctx);
    getchar();
    return 0;
}

