// ConsoleApplication2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_thread.h>

#include <stdio.h>
#include <assert.h>

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}

#ifdef _WIN32
#undef main
#endif

#define SDL_AUDIO_BUFFER_SIZE 1024
#define MAX_AUDIO_FRAME_SIZE 192000

static AVPacket flush_pkt;

typedef struct PacketQueue {
    AVPacketList* first_pkt, * last_pkt;
    int nb_packets;
    int size;
    SDL_mutex* mutex;
    SDL_cond* cond;
} PacketQueue;

PacketQueue audioq;

int quit = 0;

void packet_queue_init(PacketQueue* q) {
    memset(q, 0, sizeof(PacketQueue));
    q->mutex = SDL_CreateMutex();
    q->cond = SDL_CreateCond();
}

int packet_queue_put(PacketQueue* q, AVPacket* pkt) {
    AVPacketList* pkt1;

    pkt1 = (AVPacketList*)av_malloc(sizeof(AVPacketList));
    if (!pkt1)
    {
        return -1;
    }

    pkt1->pkt = *pkt;
    pkt1->next = NULL;

    SDL_LockMutex(q->mutex);

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
    q->size += pkt1->pkt.size;
    SDL_CondSignal(q->cond);

    SDL_UnlockMutex(q->mutex);
    return 0;
    //if (pkt == &flush_pkt)
    //{
    //    q->s

    //}
    //if (av_dup_packet(pkt) < 0)
    //{

    //}
}

static int packet_queue_get(PacketQueue* q, AVPacket* pkt, int block)
{
    AVPacketList* pkt1;
    int ret;

    SDL_LockMutex(q->mutex);

    for (;;)
    {
        if (quit)
        {
            ret = -1;
            break;
        }

        pkt1 = q->first_pkt;

        if (pkt1)
        {
            q->first_pkt = pkt1->next;
            if (!q->first_pkt)
            {
                q->last_pkt = NULL;
            }

            q->nb_packets--;
            q->size -= pkt1->pkt.size;
            *pkt = pkt1->pkt;
            av_free(pkt1);
            ret = 1;
            break;
        }
        else if (!block)
        {
            ret = 0;
            break;
        }
        else
        {
            SDL_CondWait(q->cond, q->mutex);
        }
    }
    SDL_UnlockMutex(q->mutex);
    return ret;
}

int new_decode(AVCodecContext* avctx, AVFrame* frame,
    int* got_frame, AVPacket* pkt) {
    int ret;

    *got_frame = 0;

    if (pkt)
    {
        ret = avcodec_send_packet(avctx, pkt);

        // decoded frames with avcodec_send_packet
        if (ret < 0)
        {
            return ret == AVERROR_EOF ? 0 : ret;
        }
    }

    ret = avcodec_receive_frame(avctx, frame);
    if (ret < 0 && ret != AVERROR(EAGAIN) && ret != AVERROR_EOF)
    {
        return ret;
    }

    if (ret >= 0)
        *got_frame = 1;
    return 0;
}


int audio_decode_frame(AVCodecContext* aCodecCtx, uint8_t* audio_buf, int buf_size) {
    static AVPacket pkt;
    static uint8_t* audio_pkt_data = NULL;
    static int audio_pkt_size = 0;
    static AVFrame frame;

    int len1, data_size = 0;

    for (;;)
    {
        while (audio_pkt_size > 0) {
            int got_frame = 0;
            len1 = new_decode(aCodecCtx, &frame, &got_frame, &pkt);
            if (len1 < 0)
            {
                audio_pkt_size = 0;
                break;
            }

            audio_pkt_data += len1;
            audio_pkt_size -= len1;
            data_size = 0;

            if (got_frame)
            {
                data_size = av_samples_get_buffer_size(NULL,
                                                    aCodecCtx->channels,
                                                    frame.nb_samples,
                                                    aCodecCtx->sample_fmt,
                                                    1);

                assert(data_size <= buf_size);
                memcpy(audio_buf, frame.data[0], data_size);
            }
            if (data_size <= 0)
            {
                continue;
            }
            return data_size;
        }
        if (pkt.data)
        {
            /*av_free_packet(&pkt);*/
            av_packet_unref(&pkt);
        }
        if (quit)
        {
            return -1;
        }

        if (packet_queue_get(&audioq, &pkt, 1) < 0)
        {
            return -1;
        }

        audio_pkt_data = pkt.data;
        audio_pkt_size = pkt.size;
    }
        
}

void audio_callback(void* userdata, Uint8* stream, int len) {
    AVCodecContext* avCodecCtx = (AVCodecContext*)userdata;
    int len1, audio_size;

    static uint8_t audio_buf[(MAX_AUDIO_FRAME_SIZE * 3) / 2];
    static unsigned int audio_buf_size = 0;
    static unsigned int audio_buf_index = 0;

    while (len > 0)
    {
        if (audio_buf_index >= audio_buf_size)
        {
            audio_size = audio_decode_frame(avCodecCtx, audio_buf, sizeof(audio_buf));
            if (audio_size < 0)
            {
                audio_buf_size = 1024;
                memset(audio_buf, 0, audio_buf_size);
            }
            else
            {
                audio_buf_size = audio_size;
            }
            audio_buf_index = 0;
        }
        len1 = audio_buf_size - audio_buf_index;
        if (len1 > len)
        {
            len1 = len;
        }
        memcpy(stream, (uint8_t*)audio_buf + audio_buf_index, len1);
        len -= len1;
        stream += len1;
        audio_buf_index += len1;
    }
}

int main(int argc, char *argv[])
{
    av_init_packet(&flush_pkt);
    flush_pkt.data = (uint8_t*)&flush_pkt;

    AVFormatContext*    pFormatCtx = NULL;

    int                 i, videoStream, audioStream;
    AVCodecContext*     pCodecCtxOrig = NULL;
    AVCodecContext*     pCodecCtx = NULL;
    AVCodecContext*     pCodecCtxVideo = NULL;
    AVCodec*            pCodec = NULL;
    AVFrame*            pFrame = NULL;
    AVFrame*            pFrameVideo = NULL;
    /*AVPacket            packet;*/
    AVPacket*           packetVideo;

    int					frameNotFinished;
    /*int                 frameFinished;*/
    struct              SwsContext* sws_ctx = NULL;

    uint8_t*            buffer = NULL;

    AVCodecContext*     aCodecCtxOrig = NULL;
    AVCodecContext*     aCodecCtx = NULL;
    AVCodecParameters*  aCodecParOrig = NULL;
    AVCodecParameters*  aCodecParOrigVideo = NULL;
    AVCodec*            aCodec = NULL;
    AVCodec*            aCodecVideo = NULL;

    AVDictionary*       optionsDict = NULL;

    //SDL_Overlay				*bmp = NULL;
    SDL_Window*         pWindow;
    SDL_Texture*        sdlTexture;
    SDL_Renderer*       sdlRenderer;
    SDL_Surface*        screen = NULL;
    SDL_Rect			rect;
    SDL_Event			event;

    // SDL Audio
    SDL_AudioSpec       wanted_spec, spec;

    if (argc < 2) {
        fprintf(stderr, "Usage: test <file>\n");
        exit(1);
    }

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
        fprintf(stderr, "Could not initialize SDL - %s\n", SDL_GetError());
        exit(1);
    }

    // Open video file
    if (avformat_open_input(&pFormatCtx, argv[1], NULL, NULL) != 0)
        return -1; // Couldn't open file

      // Retrieve stream information
    if (avformat_find_stream_info(pFormatCtx, NULL) < 0)
        return -1; // Couldn't find stream information

      // Dump information about file onto standard error
    //av_dump_format(pFormatCtx, 0, argv[1], 0);

    // Find the first video stream
    videoStream = -1;
    audioStream = -1;
    for (unsigned int i = 0; i < pFormatCtx->nb_streams; i++) {
        if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO &&
            videoStream < 0) {
            videoStream = i;
        }
        if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO &&
            audioStream < 0) {
            audioStream = i;
        }
    }
    if (videoStream == -1)
        return -1; // Didn't find a video stream
    if (audioStream == -1)
        return -1;

    aCodecParOrig = pFormatCtx->streams[audioStream]->codecpar;
    aCodec = avcodec_find_decoder(aCodecParOrig->codec_id);

    if (!aCodec) {
        fprintf(stderr, "Unsupported codec!\n");
        return -1;
    }

    // Copy context
    aCodecCtx = avcodec_alloc_context3(aCodec);
    if (avcodec_parameters_to_context(aCodecCtx, aCodecParOrig) != 0) {
        fprintf(stderr, "Couldn't copy codec context");
        return -1; // Error copying codec context
    }

    // Set audio settings from codec info
    wanted_spec.freq = aCodecCtx->sample_rate;
    wanted_spec.format = AUDIO_S16SYS;
    wanted_spec.channels = aCodecCtx->channels;
    wanted_spec.silence = 0;
    wanted_spec.samples = SDL_AUDIO_BUFFER_SIZE;
    wanted_spec.callback = audio_callback;
    wanted_spec.userdata = aCodecCtx;

    if (SDL_OpenAudio(&wanted_spec, &spec) < 0)
    {
        fprintf(stderr, "SDL_OpenAudio: %s\n", SDL_GetError());
        return -1;
    }

    avcodec_open2(aCodecCtx, aCodec, NULL);

    packet_queue_init(&audioq);
    SDL_PauseAudio(0);

    // Get a pointer to the codec context for the video stream
    aCodecParOrigVideo = pFormatCtx->streams[videoStream]->codecpar;

    // Find the decoder for the video stream
    aCodecVideo = avcodec_find_decoder(aCodecParOrigVideo->codec_id);

    // Allocate an AVCodecContext and set its fields to default values
    pCodecCtxVideo = avcodec_alloc_context3(aCodecVideo);

    if (avcodec_parameters_to_context(pCodecCtxVideo, aCodecParOrigVideo) != 0) {
        fprintf(stderr, "Couldn't Fill the codec context based on the values from the supplied codec parameters.");
        return -1; // Error copying codec context
    }

    // Open codec
    if (avcodec_open2(pCodecCtxVideo, aCodecVideo, &optionsDict) < 0)
        return -1; // Could not open codec

    pFrame = av_frame_alloc();
    pFrameVideo = av_frame_alloc();

    buffer = (uint8_t*)av_malloc(av_image_get_buffer_size(
        AV_PIX_FMT_YUV420P,
        pCodecCtxVideo->width, pCodecCtxVideo->height, 1));

    av_image_fill_arrays(pFrameVideo->data, pFrameVideo->linesize,
        buffer,
        AV_PIX_FMT_YUV420P, pCodecCtxVideo->width, pCodecCtxVideo->height, 1);

    // Init packet
    packetVideo = (AVPacket*)av_malloc(sizeof(AVPacket));

    //Output Info-----------------------------
    printf("--------------- File Information ----------------\n");
    av_dump_format(pFormatCtx, 0, argv[1], 0);
    printf("-------------------------------------------------\n");

    sws_ctx = sws_getContext
    (
        pCodecCtxVideo->width,
        pCodecCtxVideo->height,
        pCodecCtxVideo->pix_fmt,
        pCodecCtxVideo->width,
        pCodecCtxVideo->height,
        AV_PIX_FMT_YUV420P,
        SWS_BILINEAR,
        NULL,
        NULL,
        NULL
    );

    pWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        pCodecCtxVideo->width,
        pCodecCtxVideo->height, SDL_WINDOW_SHOWN);

    if (pWindow == NULL) {
        fprintf(stderr, "SDL: could not set video mode - exiting\n");
        exit(1);
    }

    sdlRenderer = SDL_CreateRenderer(pWindow, -1, 0);

    sdlTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_IYUV,
        SDL_TEXTUREACCESS_STREAMING, aCodecParOrigVideo->width,
        aCodecParOrigVideo->height);

    // pCodecCtxVideo->width,
    rect.x = 0;
    rect.y = 0;
    rect.w = pCodecCtxVideo->width;
    rect.h = pCodecCtxVideo->height;

    // Read frames and save first five frames to disk
    i = 0;
    while (av_read_frame(pFormatCtx, packetVideo) >= 0) {
        // Is this a packet from the video stream?
        if (packetVideo->stream_index == videoStream) {
            // Decode video frame

            new_decode(pCodecCtxVideo, pFrameVideo, &frameNotFinished,
                packetVideo);

            // Did we get a video frame?
            if (frameNotFinished) {
                // Convert the image into YUV format that SDL uses
                sws_scale(sws_ctx,
                    (const unsigned char* const*)pFrame->data,
                    pFrame->linesize, 0, pCodecCtxVideo->height,
                    pFrameVideo->data, pFrameVideo->linesize);

                SDL_UpdateYUVTexture(sdlTexture, &rect,
                    pFrameVideo->data[0], pFrameVideo->linesize[0],
                    pFrameVideo->data[1], pFrameVideo->linesize[1],
                    pFrameVideo->data[2], pFrameVideo->linesize[2]);

                //SDL_UnlockYUVOverlay(bmp);

                SDL_RenderClear(sdlRenderer);
                SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, &rect);
                SDL_RenderPresent(sdlRenderer);

                //Delay 40ms
                SDL_Delay(40);
                av_packet_unref(packetVideo);
            }
        }
        else if (packetVideo->stream_index == audioStream)
        {
            packet_queue_put(&audioq, packetVideo);
        }
        else
        {
            av_packet_unref(packetVideo);
        }
        //{

        //}

        // Free the packet that was allocated by av_read_frame
        //av_packet_unref(packetVideo);
        SDL_PollEvent(&event);
        switch (event.type) {
        case SDL_QUIT:
            SDL_Quit();
            exit(0);
            break;
        default:
            break;
        }
    }

    // Free the YUV frame
    av_free(pFrame);
    av_free(pFrameVideo);
    //av_free(pFrame);
    //av_free(pFrame);

    // Close the codec
    avcodec_close(pCodecCtx);
    avcodec_close(pCodecCtxVideo);
    avcodec_close(pCodecCtxOrig);
    avcodec_close(aCodecCtxOrig);
    avcodec_close(aCodecCtx);


    // Close the video file
    avformat_close_input(&pFormatCtx);

	return 0;
}
//
//const char program_name[] = "ffplay";
//const int program_birth_year = 2003;
//
//#define MAX_QUEUE_SIZE (15 * 1024 * 1024)
//#define MIN_FRAMES 25
//#define EXTERNAL_CLOCK_MIN_FRAMES 2
//#define EXTERNAL_CLOCK_MAX_FRAMES 10
//
///* Minimum SDL audio buffer size, in samples. */
//#define SDL_AUDIO_MIN_BUFFER_SIZE 512
///* Calculate actual buffer size keeping in mind not cause too frequent audio callbacks */
//#define SDL_AUDIO_MAX_CALLBACKS_PER_SEC 30
//
///* Step size for volume control in dB */
//#define SDL_VOLUME_STEP (0.75)
//
///* no AV sync correction is done if below the minimum AV sync threshold */
//#define AV_SYNC_THRESHOLD_MIN 0.04
///* AV sync correction is done if above the maximum AV sync threshold */
//#define AV_SYNC_THRESHOLD_MAX 0.1
///* If a frame duration is longer than this, it will not be duplicated to compensate AV sync */
//#define AV_SYNC_FRAMEDUP_THRESHOLD 0.1
///* no AV correction is done if too big error */
//#define AV_NOSYNC_THRESHOLD 10.0
//
///* maximum audio speed change to get correct sync */
//#define SAMPLE_CORRECTION_PERCENT_MAX 10
//
///* external clock speed adjustment constants for realtime sources based on buffer fullness */
//#define EXTERNAL_CLOCK_SPEED_MIN  0.900
//#define EXTERNAL_CLOCK_SPEED_MAX  1.010
//#define EXTERNAL_CLOCK_SPEED_STEP 0.001
//
///* we use about AUDIO_DIFF_AVG_NB A-V differences to make the average */
//#define AUDIO_DIFF_AVG_NB   20
//
///* polls for possible required screen refresh at least this often, should be less than 1/fps */
//#define REFRESH_RATE 0.01
//
///* NOTE: the size must be big enough to compensate the hardware audio buffersize size */
///* TODO: We assume that a decoded and resampled frame fits into this buffer */
//#define SAMPLE_ARRAY_SIZE (8 * 65536)
//
//#define CURSOR_HIDE_DELAY 1000000
//
//#define USE_ONEPASS_SUBTITLE_RENDER 1
//
//static unsigned sws_flags = SWS_BICUBIC;
//
////enum ShowMode {
////    SHOW_MODE_NONE = -1, SHOW_MODE_VIDEO = 0, SHOW_MODE_WAVES, SHOW_MODE_RDFT, SHOW_MODE_NB
////} show_mode;
//
//typedef struct MyAVPacketList {
//    AVPacket pkt;
//    struct MyAVPacketList* next;
//    int serial;
//} MyAVPacketList;
//
//typedef struct PacketQueue {
//    MyAVPacketList* first_pkt, * last_pkt;
//    int nb_packets;
//    int size;
//    int64_t duration;
//    int abort_request;
//    int serial;
//    SDL_mutex* mutex;
//    /*std::unique_ptr<SDL_mutex> mutex{ new SDL_mutex };*/
//
//    SDL_cond* cond;
//} PacketQueue;
//
//#define VIDEO_PICTURE_QUEUE_SIZE 3
//#define SUBPICTURE_QUEUE_SIZE 16
//#define SAMPLE_QUEUE_SIZE 9
//#define FRAME_QUEUE_SIZE FFMAX(SAMPLE_QUEUE_SIZE, FFMAX(VIDEO_PICTURE_QUEUE_SIZE, SUBPICTURE_QUEUE_SIZE))
//
//typedef struct AudioParams {
//    int freq;
//    int channels;
//    int64_t channel_layout;
//    enum AVSampleFormat fmt;
//    int frame_size;
//    int bytes_per_sec;
//} AudioParams;
//
//typedef struct Clock {
//    double pts;           /* clock base */
//    double pts_drift;     /* clock base minus time at which we updated the clock */
//    double last_updated;
//    double speed;
//    int serial;           /* clock is based on a packet with this serial */
//    int paused;
//    int* queue_serial;    /* pointer to the current packet queue serial, used for obsolete clock detection */
//} Clock;
//
///* Common struct for handling all types of decoded data and allocated render buffers. */
//typedef struct Frame {
//    AVFrame* frame;
//    AVSubtitle sub;
//    int serial;
//    double pts;           /* presentation timestamp for the frame */
//    double duration;      /* estimated duration of the frame */
//    int64_t pos;          /* byte position of the frame in the input file */
//    int width;
//    int height;
//    int format;
//    AVRational sar;
//    int uploaded;
//    int flip_v;
//} Frame;
//
//typedef struct FrameQueue {
//    Frame queue[FRAME_QUEUE_SIZE];
//    int rindex;
//    int windex;
//    int size;
//    int max_size;
//    int keep_last;
//    int rindex_shown;
//    SDL_mutex* mutex;
//    SDL_cond* cond;
//    PacketQueue* pktq;
//} FrameQueue;
//
//enum {
//    AV_SYNC_AUDIO_MASTER, /* default choice */
//    AV_SYNC_VIDEO_MASTER,
//    AV_SYNC_EXTERNAL_CLOCK, /* synchronize to an external clock */
//};
//
//typedef struct Decoder {
//    AVPacket pkt;
//    PacketQueue* queue;
//    AVCodecContext* avctx;
//    int pkt_serial;
//    int finished;
//    int packet_pending;
//    SDL_cond* empty_queue_cond;
//    int64_t start_pts;
//    AVRational start_pts_tb;
//    int64_t next_pts;
//    AVRational next_pts_tb;
//    SDL_Thread* decoder_tid;
//} Decoder;
//
//typedef struct VideoState {
//    SDL_Thread* read_tid;
//    AVInputFormat* iformat;
//    int abort_request;
//    int force_refresh;
//    int paused;
//    int last_paused;
//    int queue_attachments_req;
//    int seek_req;
//    int seek_flags;
//    int64_t seek_pos;
//    int64_t seek_rel;
//    int read_pause_return;
//    AVFormatContext* ic;
//    int realtime;
//
//    Clock audclk;
//    Clock vidclk;
//    Clock extclk;
//
//    FrameQueue pictq;
//    FrameQueue subpq;
//    FrameQueue sampq;
//
//    Decoder auddec;
//    Decoder viddec;
//    Decoder subdec;
//
//    int audio_stream;
//
//    int av_sync_type;
//
//    double audio_clock;
//    int audio_clock_serial;
//    double audio_diff_cum; /* used for AV difference average computation */
//    double audio_diff_avg_coef;
//    double audio_diff_threshold;
//    int audio_diff_avg_count;
//    AVStream* audio_st;
//    PacketQueue audioq;
//    int audio_hw_buf_size;
//    uint8_t* audio_buf;
//    uint8_t* audio_buf1;
//    unsigned int audio_buf_size; /* in bytes */
//    unsigned int audio_buf1_size;
//    int audio_buf_index; /* in bytes */
//    int audio_write_buf_size;
//    int audio_volume;
//    int muted;
//    struct AudioParams audio_src;
//#if CONFIG_AVFILTER
//    struct AudioParams audio_filter_src;
//#endif
//    struct AudioParams audio_tgt;
//    struct SwrContext* swr_ctx;
//    int frame_drops_early;
//    int frame_drops_late;
//
//    enum ShowMode {
//        SHOW_MODE_NONE = -1, SHOW_MODE_VIDEO = 0, SHOW_MODE_WAVES, SHOW_MODE_RDFT, SHOW_MODE_NB
//    } show_mode;
//    int16_t sample_array[SAMPLE_ARRAY_SIZE];
//    int sample_array_index;
//    int last_i_start;
//    RDFTContext* rdft;
//    int rdft_bits;
//    FFTSample* rdft_data;
//    int xpos;
//    double last_vis_time;
//    SDL_Texture* vis_texture;
//    SDL_Texture* sub_texture;
//    SDL_Texture* vid_texture;
//
//    int subtitle_stream;
//    AVStream* subtitle_st;
//    PacketQueue subtitleq;
//
//    double frame_timer;
//    double frame_last_returned_time;
//    double frame_last_filter_delay;
//    int video_stream;
//    AVStream* video_st;
//    PacketQueue videoq;
//    double max_frame_duration;      // maximum duration of a frame - above this, we consider the jump a timestamp discontinuity
//    struct SwsContext* img_convert_ctx;
//    struct SwsContext* sub_convert_ctx;
//    int eof;
//
//    char* filename;
//    int width, height, xleft, ytop;
//    int step;
//
//#if CONFIG_AVFILTER
//    int vfilter_idx;
//    AVFilterContext* in_video_filter;   // the first filter in the video chain
//    AVFilterContext* out_video_filter;  // the last filter in the video chain
//    AVFilterContext* in_audio_filter;   // the first filter in the audio chain
//    AVFilterContext* out_audio_filter;  // the last filter in the audio chain
//    AVFilterGraph* agraph;              // audio filter graph
//#endif
//
//    int last_video_stream, last_audio_stream, last_subtitle_stream;
//
//    SDL_cond* continue_read_thread;
//} VideoState;
//
///* options specified by the user */
//static AVInputFormat* file_iformat;
//static const char* input_filename;
//static const char* window_title;
//static int default_width = 640;
//static int default_height = 480;
//static int screen_width = 0;
//static int screen_height = 0;
//static int screen_left = SDL_WINDOWPOS_CENTERED;
//static int screen_top = SDL_WINDOWPOS_CENTERED;
//static int audio_disable;
//static int video_disable;
//static int subtitle_disable;
//static const char* wanted_stream_spec[AVMEDIA_TYPE_NB] = { 0 };
//static int seek_by_bytes = -1;
//static float seek_interval = 10;
//static int display_disable;
//static int borderless;
//static int alwaysontop;
//static int startup_volume = 100;
//static int show_status = 1;
//static int av_sync_type = AV_SYNC_AUDIO_MASTER;
//static int64_t start_time = AV_NOPTS_VALUE;
//static int64_t duration = AV_NOPTS_VALUE;
//static int fast = 0;
//static int genpts = 0;
//static int lowres = 0;
//static int decoder_reorder_pts = -1;
//static int autoexit;
//static int exit_on_keydown;
//static int exit_on_mousedown;
//static int loop = 1;
//static int framedrop = -1;
//static int infinite_buffer = -1;
////static enum ShowMode show_mode = SHOW_MODE_NONE;
//static const char* audio_codec_name;
//static const char* subtitle_codec_name;
//static const char* video_codec_name;
//double rdftspeed = 0.02;
//static int64_t cursor_last_shown;
//static int cursor_hidden = 0;
//#if CONFIG_AVFILTER
//static const char** vfilters_list = NULL;
//static int nb_vfilters = 0;
//static char* afilters = NULL;
//#endif
//static int autorotate = 1;
//static int find_stream_info = 1;
//static int filter_nbthreads = 0;
//
///* current context */
//static int is_full_screen;
//static int64_t audio_callback_time;
//
//static AVPacket flush_pkt;
//
//#define FF_QUIT_EVENT    (SDL_USEREVENT + 2)
//
//static SDL_Window* window;
//static SDL_Renderer* renderer;
//static SDL_RendererInfo renderer_info = { 0 };
//static SDL_AudioDeviceID audio_dev;
//
//static const struct TextureFormatEntry {
//    enum AVPixelFormat format;
//    int texture_fmt;
//} sdl_texture_format_map[] = {
//    { AV_PIX_FMT_RGB8,           SDL_PIXELFORMAT_RGB332 },
//    { AV_PIX_FMT_RGB444,         SDL_PIXELFORMAT_RGB444 },
//    { AV_PIX_FMT_RGB555,         SDL_PIXELFORMAT_RGB555 },
//    { AV_PIX_FMT_BGR555,         SDL_PIXELFORMAT_BGR555 },
//    { AV_PIX_FMT_RGB565,         SDL_PIXELFORMAT_RGB565 },
//    { AV_PIX_FMT_BGR565,         SDL_PIXELFORMAT_BGR565 },
//    { AV_PIX_FMT_RGB24,          SDL_PIXELFORMAT_RGB24 },
//    { AV_PIX_FMT_BGR24,          SDL_PIXELFORMAT_BGR24 },
//    { AV_PIX_FMT_0RGB32,         SDL_PIXELFORMAT_RGB888 },
//    { AV_PIX_FMT_0BGR32,         SDL_PIXELFORMAT_BGR888 },
//    { AV_PIX_FMT_NE(RGB0, 0BGR), SDL_PIXELFORMAT_RGBX8888 },
//    { AV_PIX_FMT_NE(BGR0, 0RGB), SDL_PIXELFORMAT_BGRX8888 },
//    { AV_PIX_FMT_RGB32,          SDL_PIXELFORMAT_ARGB8888 },
//    { AV_PIX_FMT_RGB32_1,        SDL_PIXELFORMAT_RGBA8888 },
//    { AV_PIX_FMT_BGR32,          SDL_PIXELFORMAT_ABGR8888 },
//    { AV_PIX_FMT_BGR32_1,        SDL_PIXELFORMAT_BGRA8888 },
//    { AV_PIX_FMT_YUV420P,        SDL_PIXELFORMAT_IYUV },
//    { AV_PIX_FMT_YUYV422,        SDL_PIXELFORMAT_YUY2 },
//    { AV_PIX_FMT_UYVY422,        SDL_PIXELFORMAT_UYVY },
//    { AV_PIX_FMT_NONE,           SDL_PIXELFORMAT_UNKNOWN },
//};
//
//static void calculate_display_rect(SDL_Rect* rect,
//    int scr_xleft, int scr_ytop, int scr_width, int scr_height,
//    int pic_width, int pic_height, AVRational pic_sar)
//{
//    AVRational aspect_ratio = pic_sar;
//    int64_t width, height, x, y;
//
//    if (av_cmp_q(aspect_ratio, av_make_q(0, 1)) <= 0)
//        aspect_ratio = av_make_q(1, 1);
//
//    aspect_ratio = av_mul_q(aspect_ratio, av_make_q(pic_width, pic_height));
//
//    /* XXX: we suppose the screen has a 1.0 pixel ratio */
//    height = scr_height;
//    width = av_rescale(height, aspect_ratio.num, aspect_ratio.den) & ~1;
//    if (width > scr_width) {
//        width = scr_width;
//        height = av_rescale(width, aspect_ratio.den, aspect_ratio.num) & ~1;
//    }
//    x = (scr_width - width) / 2;
//    y = (scr_height - height) / 2;
//    rect->x = scr_xleft + x;
//    rect->y = scr_ytop + y;
//    rect->w = FFMAX((int)width, 1);
//    rect->h = FFMAX((int)height, 1);
//}
//
//
//static void set_default_window_size(int width, int height, AVRational sar)
//{
//    SDL_Rect rect;
//    int max_width = screen_width ? screen_width : INT_MAX;
//    int max_height = screen_height ? screen_height : INT_MAX;
//    if (max_width == INT_MAX && max_height == INT_MAX)
//        max_height = height;
//    calculate_display_rect(&rect, 0, 0, max_width, max_height, width, height, sar);
//    default_width = rect.w;
//    default_height = rect.h;
//}
//
///**
// * Decode one audio frame and return its uncompressed size.
// *
// * The processed audio frame is decoded, converted if required, and
// * stored in is->audio_buf, with size in bytes given by the return
// * value.
// */
//static int audio_decode_frame(VideoState* is)
//{
//    int data_size, resampled_data_size;
//    int64_t dec_channel_layout;
//    av_unused double audio_clock0;
//    int wanted_nb_samples;
//    Frame* af;
//
//    if (is->paused)
//        return -1;
//
//    do {
//#if defined(_WIN32)
//        while (frame_queue_nb_remaining(&is->sampq) == 0) {
//            if ((av_gettime_relative() - audio_callback_time) > 1000000LL * is->audio_hw_buf_size / is->audio_tgt.bytes_per_sec / 2)
//                return -1;
//            av_usleep(1000);
//        }
//#endif
//        if (!(af = frame_queue_peek_readable(&is->sampq)))
//            return -1;
//        frame_queue_next(&is->sampq);
//    } while (af->serial != is->audioq.serial);
//
//    data_size = av_samples_get_buffer_size(NULL, af->frame->channels,
//        af->frame->nb_samples,
//        af->frame->format, 1);
//
//    dec_channel_layout =
//        (af->frame->channel_layout && af->frame->channels == av_get_channel_layout_nb_channels(af->frame->channel_layout)) ?
//        af->frame->channel_layout : av_get_default_channel_layout(af->frame->channels);
//    wanted_nb_samples = synchronize_audio(is, af->frame->nb_samples);
//
//    if (af->frame->format != is->audio_src.fmt ||
//        dec_channel_layout != is->audio_src.channel_layout ||
//        af->frame->sample_rate != is->audio_src.freq ||
//        (wanted_nb_samples != af->frame->nb_samples && !is->swr_ctx)) {
//        swr_free(&is->swr_ctx);
//        is->swr_ctx = swr_alloc_set_opts(NULL,
//            is->audio_tgt.channel_layout, is->audio_tgt.fmt, is->audio_tgt.freq,
//            dec_channel_layout, af->frame->format, af->frame->sample_rate,
//            0, NULL);
//        if (!is->swr_ctx || swr_init(is->swr_ctx) < 0) {
//            av_log(NULL, AV_LOG_ERROR,
//                "Cannot create sample rate converter for conversion of %d Hz %s %d channels to %d Hz %s %d channels!\n",
//                af->frame->sample_rate, av_get_sample_fmt_name(af->frame->format), af->frame->channels,
//                is->audio_tgt.freq, av_get_sample_fmt_name(is->audio_tgt.fmt), is->audio_tgt.channels);
//            swr_free(&is->swr_ctx);
//            return -1;
//        }
//        is->audio_src.channel_layout = dec_channel_layout;
//        is->audio_src.channels = af->frame->channels;
//        is->audio_src.freq = af->frame->sample_rate;
//        is->audio_src.fmt = af->frame->format;
//    }
//
//    if (is->swr_ctx) {
//        const uint8_t** in = (const uint8_t**)af->frame->extended_data;
//        uint8_t** out = &is->audio_buf1;
//        int out_count = (int64_t)wanted_nb_samples * is->audio_tgt.freq / af->frame->sample_rate + 256;
//        int out_size = av_samples_get_buffer_size(NULL, is->audio_tgt.channels, out_count, is->audio_tgt.fmt, 0);
//        int len2;
//        if (out_size < 0) {
//            av_log(NULL, AV_LOG_ERROR, "av_samples_get_buffer_size() failed\n");
//            return -1;
//        }
//        if (wanted_nb_samples != af->frame->nb_samples) {
//            if (swr_set_compensation(is->swr_ctx, (wanted_nb_samples - af->frame->nb_samples) * is->audio_tgt.freq / af->frame->sample_rate,
//                wanted_nb_samples * is->audio_tgt.freq / af->frame->sample_rate) < 0) {
//                av_log(NULL, AV_LOG_ERROR, "swr_set_compensation() failed\n");
//                return -1;
//            }
//        }
//        av_fast_malloc(&is->audio_buf1, &is->audio_buf1_size, out_size);
//        if (!is->audio_buf1)
//            return AVERROR(ENOMEM);
//        len2 = swr_convert(is->swr_ctx, out, out_count, in, af->frame->nb_samples);
//        if (len2 < 0) {
//            av_log(NULL, AV_LOG_ERROR, "swr_convert() failed\n");
//            return -1;
//        }
//        if (len2 == out_count) {
//            av_log(NULL, AV_LOG_WARNING, "audio buffer is probably too small\n");
//            if (swr_init(is->swr_ctx) < 0)
//                swr_free(&is->swr_ctx);
//        }
//        is->audio_buf = is->audio_buf1;
//        resampled_data_size = len2 * is->audio_tgt.channels * av_get_bytes_per_sample(is->audio_tgt.fmt);
//    }
//    else {
//        is->audio_buf = af->frame->data[0];
//        resampled_data_size = data_size;
//    }
//
//    audio_clock0 = is->audio_clock;
//    /* update the audio clock with the pts */
//    if (!isnan(af->pts))
//        is->audio_clock = af->pts + (double)af->frame->nb_samples / af->frame->sample_rate;
//    else
//        is->audio_clock = NAN;
//    is->audio_clock_serial = af->serial;
//#ifdef DEBUG
//    {
//        static double last_clock;
//        printf("audio: delay=%0.3f clock=%0.3f clock0=%0.3f\n",
//            is->audio_clock - last_clock,
//            is->audio_clock, audio_clock0);
//        last_clock = is->audio_clock;
//    }
//#endif
//    return resampled_data_size;
//}
//
///* prepare a new audio buffer */
//static void sdl_audio_callback(void* opaque, Uint8* stream, int len)
//{
//    VideoState* is = opaque;
//    int audio_size, len1;
//
//    audio_callback_time = av_gettime_relative();
//
//    while (len > 0) {
//        if (is->audio_buf_index >= is->audio_buf_size) {
//            audio_size = audio_decode_frame(is);
//            if (audio_size < 0) {
//                /* if error, just output silence */
//                is->audio_buf = NULL;
//                is->audio_buf_size = SDL_AUDIO_MIN_BUFFER_SIZE / is->audio_tgt.frame_size * is->audio_tgt.frame_size;
//            }
//            else {
//                if (is->show_mode != SHOW_MODE_VIDEO)
//                    update_sample_display(is, (int16_t*)is->audio_buf, audio_size);
//                is->audio_buf_size = audio_size;
//            }
//            is->audio_buf_index = 0;
//        }
//        len1 = is->audio_buf_size - is->audio_buf_index;
//        if (len1 > len)
//            len1 = len;
//        if (!is->muted && is->audio_buf && is->audio_volume == SDL_MIX_MAXVOLUME)
//            memcpy(stream, (uint8_t*)is->audio_buf + is->audio_buf_index, len1);
//        else {
//            memset(stream, 0, len1);
//            if (!is->muted && is->audio_buf)
//                SDL_MixAudioFormat(stream, (uint8_t*)is->audio_buf + is->audio_buf_index, AUDIO_S16SYS, len1, is->audio_volume);
//        }
//        len -= len1;
//        stream += len1;
//        is->audio_buf_index += len1;
//    }
//    is->audio_write_buf_size = is->audio_buf_size - is->audio_buf_index;
//    /* Let's assume the audio driver that is used by SDL has two periods. */
//    if (!isnan(is->audio_clock)) {
//        set_clock_at(&is->audclk, is->audio_clock - (double)(2 * is->audio_hw_buf_size + is->audio_write_buf_size) / is->audio_tgt.bytes_per_sec, is->audio_clock_serial, audio_callback_time / 1000000.0);
//        sync_clock_to_slave(&is->extclk, &is->audclk);
//    }
//}
//
//
//static int audio_open(void* opaque, int64_t wanted_channel_layout, int wanted_nb_channels, int wanted_sample_rate, struct AudioParams* audio_hw_params)
//{
//    SDL_AudioSpec wanted_spec, spec;
//    const char* env;
//    static const int next_nb_channels[] = { 0, 0, 1, 6, 2, 6, 4, 6 };
//    static const int next_sample_rates[] = { 0, 44100, 48000, 96000, 192000 };
//    int next_sample_rate_idx = FF_ARRAY_ELEMS(next_sample_rates) - 1;
//
//    env = SDL_getenv("SDL_AUDIO_CHANNELS");
//    if (env) {
//        wanted_nb_channels = atoi(env);
//        wanted_channel_layout = av_get_default_channel_layout(wanted_nb_channels);
//    }
//    if (!wanted_channel_layout || wanted_nb_channels != av_get_channel_layout_nb_channels(wanted_channel_layout)) {
//        wanted_channel_layout = av_get_default_channel_layout(wanted_nb_channels);
//        wanted_channel_layout &= ~AV_CH_LAYOUT_STEREO_DOWNMIX;
//    }
//    wanted_nb_channels = av_get_channel_layout_nb_channels(wanted_channel_layout);
//    wanted_spec.channels = wanted_nb_channels;
//    wanted_spec.freq = wanted_sample_rate;
//    if (wanted_spec.freq <= 0 || wanted_spec.channels <= 0) {
//        av_log(NULL, AV_LOG_ERROR, "Invalid sample rate or channel count!\n");
//        return -1;
//    }
//    while (next_sample_rate_idx && next_sample_rates[next_sample_rate_idx] >= wanted_spec.freq)
//        next_sample_rate_idx--;
//    wanted_spec.format = AUDIO_S16SYS;
//    wanted_spec.silence = 0;
//    wanted_spec.samples = FFMAX(SDL_AUDIO_MIN_BUFFER_SIZE, 2 << av_log2(wanted_spec.freq / SDL_AUDIO_MAX_CALLBACKS_PER_SEC));
//    wanted_spec.callback = sdl_audio_callback;
//    wanted_spec.userdata = opaque;
//    while (!(audio_dev = SDL_OpenAudioDevice(NULL, 0, &wanted_spec, &spec, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE | SDL_AUDIO_ALLOW_CHANNELS_CHANGE))) {
//        av_log(NULL, AV_LOG_WARNING, "SDL_OpenAudio (%d channels, %d Hz): %s\n",
//            wanted_spec.channels, wanted_spec.freq, SDL_GetError());
//        wanted_spec.channels = next_nb_channels[FFMIN(7, wanted_spec.channels)];
//        if (!wanted_spec.channels) {
//            wanted_spec.freq = next_sample_rates[next_sample_rate_idx--];
//            wanted_spec.channels = wanted_nb_channels;
//            if (!wanted_spec.freq) {
//                av_log(NULL, AV_LOG_ERROR,
//                    "No more combinations to try, audio open failed\n");
//                return -1;
//            }
//        }
//        wanted_channel_layout = av_get_default_channel_layout(wanted_spec.channels);
//    }
//    if (spec.format != AUDIO_S16SYS) {
//        av_log(NULL, AV_LOG_ERROR,
//            "SDL advised audio format %d is not supported!\n", spec.format);
//        return -1;
//    }
//    if (spec.channels != wanted_spec.channels) {
//        wanted_channel_layout = av_get_default_channel_layout(spec.channels);
//        if (!wanted_channel_layout) {
//            av_log(NULL, AV_LOG_ERROR,
//                "SDL advised channel count %d is not supported!\n", spec.channels);
//            return -1;
//        }
//    }
//
//    audio_hw_params->fmt = AV_SAMPLE_FMT_S16;
//    audio_hw_params->freq = spec.freq;
//    audio_hw_params->channel_layout = wanted_channel_layout;
//    audio_hw_params->channels = spec.channels;
//    audio_hw_params->frame_size = av_samples_get_buffer_size(NULL, audio_hw_params->channels, 1, audio_hw_params->fmt, 1);
//    audio_hw_params->bytes_per_sec = av_samples_get_buffer_size(NULL, audio_hw_params->channels, audio_hw_params->freq, audio_hw_params->fmt, 1);
//    if (audio_hw_params->bytes_per_sec <= 0 || audio_hw_params->frame_size <= 0) {
//        av_log(NULL, AV_LOG_ERROR, "av_samples_get_buffer_size failed\n");
//        return -1;
//    }
//    return spec.size;
//}
//
///* open a given stream. Return 0 if OK */
//static int stream_component_open(VideoState* is, int stream_index)
//{
//    AVFormatContext* ic = is->ic;
//    AVCodecContext* avctx;
//    AVCodec* codec;
//    const char* forced_codec_name = NULL;
//    AVDictionary* opts = NULL;
//    AVDictionaryEntry* t = NULL;
//    int sample_rate, nb_channels;
//    int64_t channel_layout;
//    int ret = 0;
//    int stream_lowres = lowres;
//
//    if (stream_index < 0 || stream_index >= ic->nb_streams)
//        return -1;
//
//    avctx = avcodec_alloc_context3(NULL);
//    if (!avctx)
//        return AVERROR(ENOMEM);
//
//    ret = avcodec_parameters_to_context(avctx, ic->streams[stream_index]->codecpar);
//    if (ret < 0)
//        goto fail;
//    avctx->pkt_timebase = ic->streams[stream_index]->time_base;
//
//    codec = avcodec_find_decoder(avctx->codec_id);
//
//    switch (avctx->codec_type) {
//    case AVMEDIA_TYPE_AUDIO: is->last_audio_stream = stream_index; forced_codec_name = audio_codec_name; break;
//    case AVMEDIA_TYPE_SUBTITLE: is->last_subtitle_stream = stream_index; forced_codec_name = subtitle_codec_name; break;
//    case AVMEDIA_TYPE_VIDEO: is->last_video_stream = stream_index; forced_codec_name = video_codec_name; break;
//    }
//    if (forced_codec_name)
//        codec = avcodec_find_decoder_by_name(forced_codec_name);
//    if (!codec) {
//        if (forced_codec_name) av_log(NULL, AV_LOG_WARNING,
//            "No codec could be found with name '%s'\n", forced_codec_name);
//        else                   av_log(NULL, AV_LOG_WARNING,
//            "No decoder could be found for codec %s\n", avcodec_get_name(avctx->codec_id));
//        ret = AVERROR(EINVAL);
//        goto fail;
//    }
//
//    avctx->codec_id = codec->id;
//    if (stream_lowres > codec->max_lowres) {
//        av_log(avctx, AV_LOG_WARNING, "The maximum value for lowres supported by the decoder is %d\n",
//            codec->max_lowres);
//        stream_lowres = codec->max_lowres;
//    }
//    avctx->lowres = stream_lowres;
//
//    if (fast)
//        avctx->flags2 |= AV_CODEC_FLAG2_FAST;
//
//    opts = filter_codec_opts(codec_opts, avctx->codec_id, ic, ic->streams[stream_index], codec);
//    if (!av_dict_get(opts, "threads", NULL, 0))
//        av_dict_set(&opts, "threads", "auto", 0);
//    if (stream_lowres)
//        av_dict_set_int(&opts, "lowres", stream_lowres, 0);
//    if (avctx->codec_type == AVMEDIA_TYPE_VIDEO || avctx->codec_type == AVMEDIA_TYPE_AUDIO)
//        av_dict_set(&opts, "refcounted_frames", "1", 0);
//    if ((ret = avcodec_open2(avctx, codec, &opts)) < 0) {
//        goto fail;
//    }
//    if ((t = av_dict_get(opts, "", NULL, AV_DICT_IGNORE_SUFFIX))) {
//        av_log(NULL, AV_LOG_ERROR, "Option %s not found.\n", t->key);
//        ret = AVERROR_OPTION_NOT_FOUND;
//        goto fail;
//    }
//
//    is->eof = 0;
//    ic->streams[stream_index]->discard = AVDISCARD_DEFAULT;
//    switch (avctx->codec_type) {
//    case AVMEDIA_TYPE_AUDIO:
//#if CONFIG_AVFILTER
//    {
//        AVFilterContext* sink;
//
//        is->audio_filter_src.freq = avctx->sample_rate;
//        is->audio_filter_src.channels = avctx->channels;
//        is->audio_filter_src.channel_layout = get_valid_channel_layout(avctx->channel_layout, avctx->channels);
//        is->audio_filter_src.fmt = avctx->sample_fmt;
//        if ((ret = configure_audio_filters(is, afilters, 0)) < 0)
//            goto fail;
//        sink = is->out_audio_filter;
//        sample_rate = av_buffersink_get_sample_rate(sink);
//        nb_channels = av_buffersink_get_channels(sink);
//        channel_layout = av_buffersink_get_channel_layout(sink);
//    }
//#else
//        sample_rate = avctx->sample_rate;
//        nb_channels = avctx->channels;
//        channel_layout = avctx->channel_layout;
//#endif
//
//        /* prepare audio output */
//        if ((ret = audio_open(is, channel_layout, nb_channels, sample_rate, &is->audio_tgt)) < 0)
//            goto fail;
//        is->audio_hw_buf_size = ret;
//        is->audio_src = is->audio_tgt;
//        is->audio_buf_size = 0;
//        is->audio_buf_index = 0;
//
//        /* init averaging filter */
//        is->audio_diff_avg_coef = exp(log(0.01) / AUDIO_DIFF_AVG_NB);
//        is->audio_diff_avg_count = 0;
//        /* since we do not have a precise anough audio FIFO fullness,
//           we correct audio sync only if larger than this threshold */
//        is->audio_diff_threshold = (double)(is->audio_hw_buf_size) / is->audio_tgt.bytes_per_sec;
//
//        is->audio_stream = stream_index;
//        is->audio_st = ic->streams[stream_index];
//
//        decoder_init(&is->auddec, avctx, &is->audioq, is->continue_read_thread);
//        if ((is->ic->iformat->flags & (AVFMT_NOBINSEARCH | AVFMT_NOGENSEARCH | AVFMT_NO_BYTE_SEEK)) && !is->ic->iformat->read_seek) {
//            is->auddec.start_pts = is->audio_st->start_time;
//            is->auddec.start_pts_tb = is->audio_st->time_base;
//        }
//        if ((ret = decoder_start(&is->auddec, audio_thread, "audio_decoder", is)) < 0)
//            goto out;
//        SDL_PauseAudioDevice(audio_dev, 0);
//        break;
//    case AVMEDIA_TYPE_VIDEO:
//        is->video_stream = stream_index;
//        is->video_st = ic->streams[stream_index];
//
//        decoder_init(&is->viddec, avctx, &is->videoq, is->continue_read_thread);
//        if ((ret = decoder_start(&is->viddec, video_thread, "video_decoder", is)) < 0)
//            goto out;
//        is->queue_attachments_req = 1;
//        break;
//    case AVMEDIA_TYPE_SUBTITLE:
//        is->subtitle_stream = stream_index;
//        is->subtitle_st = ic->streams[stream_index];
//
//        decoder_init(&is->subdec, avctx, &is->subtitleq, is->continue_read_thread);
//        if ((ret = decoder_start(&is->subdec, subtitle_thread, "subtitle_decoder", is)) < 0)
//            goto out;
//        break;
//    default:
//        break;
//    }
//    goto out;
//
//fail:
//    avcodec_free_context(&avctx);
//out:
//    av_dict_free(&opts);
//
//    return ret;
//}
//
//int main(int argc, char **argv)
//{
//    int flags;
//    VideoState* is;
//    AVFormatContext* pFormatCtx = NULL;
//
//	const std::string file_name = "C:\\Work\\media\\NPO1.ts";
//
//	/*if ()*/
//
//	if (avformat_open_input(&pFormatCtx, file_name.c_str(), NULL, 0)!=0)
//	{
//		return -1;
//	}
//
//	if (avformat_find_stream_info(pFormatCtx, NULL) < 0)
//	{
//		return -1;
//	}
//
//	av_dump_format(pFormatCtx, 0, file_name.c_str(), 0);
//
//	int i;
//	//void* arg = NULL;
//	VideoState *is = NULL;
//	AVCodecContext* pCodecCtxOrig = NULL;
//	AVCodecContext* pCodecCtx = NULL;
//
//	AVCodecParameters* pCodecparOrig = NULL;
//	AVCodecParameters* pCodecpar = NULL;
//
//	AVFrame* pFrame = NULL;
//	AVFrame* pFrameRGB = NULL;
//
//	int videoStream = -1;
//	int st_index[AVMEDIA_TYPE_NB];
//
//    VideoState* is = NULL;
//
//	for (i = 0; i < pFormatCtx->nb_streams; i++)
//	{
//		AVStream* pSt = pFormatCtx->streams[i];
//
//		enum AVMediaType type = pSt->codecpar->codec_type;
//
//		pSt->discard = AVDISCARD_ALL;
//
//		if (type >= 0 && wanted_stream_spec[type] && st_index[type] == -1)
//		{
//			if (avformat_match_stream_specifier(pFormatCtx, pSt, wanted_stream_spec[type]) > 0)
//			{
//				st_index[type] = i;
//			}
//
//		}
//
//		for (i = 0; i < AVMEDIA_TYPE_NB; i++)
//		{
//			if (wanted_stream_spec[i] && st_index[i] == -1)
//			{
//				av_log(NULL, AV_LOG_ERROR, "Stream specifier %s does not match any stream\n", wanted_stream_spec[i]);
//				st_index[i] = INT_MAX;
//			}
//
//		}
//
//		if (!video_disable)
//		{
//			st_index[AVMEDIA_TYPE_VIDEO] = av_find_best_stream(pFormatCtx, AVMEDIA_TYPE_VIDEO,
//																st_index[AVMEDIA_TYPE_VIDEO], -1, NULL, 0);
//		}
//
//		if (!audio_disable)
//			st_index[AVMEDIA_TYPE_AUDIO] =
//			av_find_best_stream(pFormatCtx, AVMEDIA_TYPE_AUDIO,
//				st_index[AVMEDIA_TYPE_AUDIO],
//				st_index[AVMEDIA_TYPE_VIDEO],
//				NULL, 0);
//		if (!video_disable && !subtitle_disable)
//			st_index[AVMEDIA_TYPE_SUBTITLE] =
//			av_find_best_stream(pFormatCtx, AVMEDIA_TYPE_SUBTITLE,
//				st_index[AVMEDIA_TYPE_SUBTITLE],
//				(st_index[AVMEDIA_TYPE_AUDIO] >= 0 ?
//					st_index[AVMEDIA_TYPE_AUDIO] :
//					st_index[AVMEDIA_TYPE_VIDEO]),
//				NULL, 0);
//
//		//if (pFormatCtx->streams[i]->codecpar->codec_type==AVMEDIA_TYPE_VIDEO)
//		//{
//		//	videoStream = i;
//		//	break;
//		//}
//	}
//
//    /*is->show_mode = show_mode;*/
//    is->ic = pFormatCtx;
//
//    if (st_index[AVMEDIA_TYPE_VIDEO] >= 0)
//    {
//        AVStream *st = pFormatCtx->streams[st_index[AVMEDIA_TYPE_VIDEO]];
//        AVCodecParameters* codecpar = st->codecpar;
//        AVRational sar = av_guess_sample_aspect_ratio(pFormatCtx, st, NULL);
//        if (codecpar->width)
//        {
//            set_default_window_size(codecpar->width, codecpar->height, sar);
//        }
//
//    }
//
//    if (st_index[AVMEDIA_TYPE_AUDIO] >= 0)
//    {
//        stream_component_open()
//    }
//	pCodecpar = pFormatCtx->streams[videoStream]->codecpar;
//
//	return 0;
//}
//
//// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
//// Debug program: F5 or Debug > Start Debugging menu
//
//// Tips for Getting Started: 
////   1. Use the Solution Explorer window to add/manage files
////   2. Use the Team Explorer window to connect to source control
////   3. Use the Output window to see build output and other messages
////   4. Use the Error List window to view errors
////   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
////   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
