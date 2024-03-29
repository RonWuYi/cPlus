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