// ConsoleApplication4.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "CommonHeader.h"
#include "Header.h"
#include <iostream>

// sdl2 main
#ifdef _WIN32

#undef main

#endif // _WIN32

void SaveFrame(AVFrame* pFrame, int width, int height, int iFrame)
{
    FILE* pFile;
    errno_t err;
    char szFilename[32];
    //char* nszFilename = NULL;
    int y;

    //sprintf(szFilename, "fame%d.ppm", iFrame);
    sprintf_s(szFilename, "fame%d.ppm", iFrame);
    //pFile = fopen(szFilename, "wb");
    err = fopen_s(&pFile, szFilename, "wb");

    if (pFile==NULL)
    {
        return;
    }

    fprintf(pFile, "P6\n%d %d\n255\n", width, height);

    for (y = 0; y < height; y++)
    {
        fwrite(pFrame->data[0] + y * pFrame->linesize[0], 1, width * 3, pFile);
    }

    // Close file
    fclose(pFile);
}

int stream_component_open(VideoState* is, int stream_index) {

}
int main(int argc, char *argv[])
{
    SDL_Event event;
    VideoState* is;

    is = (VideoState*)av_mallocz(sizeof(VideoState));
    //is = (VideoState*)av_malloc(sizeof(VideoState));

    av_strlcpy(is->filename, argv[1], sizeof(is->filename));
    is->pictq_mutex = SDL_CreateMutex();
    is->pictq_cond = SDL_CreateCond();

    schedule_refresh(is, 40);

    is->parse_tid = SDL_CreateThread(decode_thread, is);

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER))
    {
        fprintf(stderr, "could not initialize SDL - %s\n", SDL_GetError());
        exit(1);
    }

    static SDL_Window* window;
    static SDL_Renderer* renderer;
    static SDL_RendererInfo* renderer_info = { 0 };

    window = SDL_CreateWindow("player", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        video_width, video_height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);


    if (window == NULL)
    {
        fprintf(stderr, "could not SDL_CreateWindow on SDL - %s\n", SDL_GetError());
        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, 0);

    if (renderer == NULL)
    {
        fprintf(stderr, "could not SDL_CreateRenderer on SDL - %s\n", SDL_GetError());
        exit(1);
    }

    if (SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255) < 0)
    {
        fprintf(stderr, "could not SDL_SetRenderDrawColor on SDL - %s\n", SDL_GetError());
        exit(1);
    }
    
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    SDL_Delay(3000);

    AVFormatContext*    pFormatCtx = NULL;
    int                 i, videoStream;
    AVCodecContext* pCodecCtxOrig = NULL;
    AVCodecContext* pCodecCtx = NULL;

    AVCodecParameters* pCodecParOrig = NULL;
    AVCodecParameters* pCodecPar = NULL;

    AVCodec* pCodec = NULL;
    AVFrame* pFrame = NULL;
    AVFrame* pFrameRGB = NULL;

    AVDictionary* opts = NULL;

    AVPacket packet;
    int frameFinished;
    int numBytes;
    uint8_t* buffer = NULL;
    struct SwsContext* sws_ctx = NULL;

    if (argc < 2)
    {
        printf("Please provide a movie file\n");
        return -1;
    }

    // Register all formats and codecs
    /*av_register_all();*/

        /* register all codecs, demux and protocols */
#if CONFIG_AVDEVICE
    avdevice_register_all();
#endif
    avformat_network_init();

    if (avformat_open_input(&pFormatCtx, argv[1], NULL, NULL)!=0)
    {
        return -1;
    }

    if (avformat_find_stream_info(pFormatCtx, NULL) < 0)
        return -1;

    av_dump_format(pFormatCtx, 0, argv[1], 0);

    videoStream = -1;
    for (unsigned int i = 0; i < pFormatCtx->nb_streams; i++)
    {
        if (pFormatCtx->streams[i]->codecpar->codec_type==AVMEDIA_TYPE_VIDEO)
        {
            videoStream = i;
            break;
        }
    }

    if (videoStream==-1)
    {
        return -1;
    }

    // Get a pointer to the codec context for the video stream
    pCodecParOrig = pFormatCtx->streams[videoStream]->codecpar;

    pCodec = avcodec_find_decoder(pCodecParOrig->codec_id);
    if (pCodec==NULL)
    {
        fprintf(stderr, "Unsupported codec!\n");
        return -1; // Codec not found
    }

    // Copy context
    pCodecCtx = avcodec_alloc_context3(pCodec);
    if (avcodec_parameters_to_context(pCodecCtx, pCodecParOrig) < 0)
    {
        fprintf(stderr, "Couldn't copy codec context");
        return -1;
    }

    // Open codec
    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0)
    {
        return -1;
    }

    pFrame = av_frame_alloc();
    pFrameRGB = av_frame_alloc();
    if (pFrameRGB==NULL)
    {
        return -1;
    }

    /*numBytes = avpicture_get_size(AV_PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height);*/
    int tmp_align = 1;
    numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24, pCodecCtx->width,
                                        pCodecCtx->height, tmp_align);
    buffer = (uint8_t*)av_malloc(numBytes * sizeof(uint8_t));

    //avpicture_fill((AVPicture*));

    // av_image_fill_arrays
    av_image_fill_arrays(pFrameRGB->data, pFrameRGB->linesize,
                        buffer, AV_PIX_FMT_RGB24,
                        pCodecCtx->width, pCodecCtx->height, 
                        1);

    // initialize SWS context for software scaling
    sws_ctx = sws_getContext(pCodecCtx->width,
        pCodecCtx->height,
        pCodecCtx->pix_fmt,
        pCodecCtx->width,
        pCodecCtx->height,
        AV_PIX_FMT_RGB24,
        SWS_BILINEAR,
        NULL,
        NULL,
        NULL);

    // Read frames and save first five frames to disk
    i = 0;
    Decoder* video_decoder = NULL;
    AVSubtitle* tmp_subtitle = NULL;
    while (av_read_frame(pFormatCtx, &packet)>=0)
    {
        LOG(INFO) << "start read frame";
        if (packet.stream_index==videoStream)
        {
            //decoder_decode_frame();
            new_decode(pCodecCtx, pFrame, &frameFinished, &packet);

            if (frameFinished)
            {
                sws_scale(sws_ctx, (uint8_t const* const*)pFrame->data,
                    pFrame->linesize, 0, pCodecCtx->height, pFrameRGB->data, pFrameRGB->linesize);

                if (++i<=5)
                {
                    SaveFrame(pFrameRGB, pCodecCtx->width, pCodecCtx->height, i);
                }
            }

        }
        // Free the packet
        /*av_free_packet(&packet);*/

        LOG(INFO) << "free packet";
        av_packet_unref(&packet);

    }

    LOG(INFO) << "finnal free";
    // Free the RGB image
    av_free(buffer);
    av_frame_free(&pFrameRGB);

    // Free the YUV frame
    av_frame_free(&pFrame);

    // Close the codecs
    avcodec_close(pCodecCtx);
    avcodec_close(pCodecCtxOrig);

    // Close the video file
    avformat_close_input(&pFormatCtx);
    //new_decode(pCodecCtx, pFrame, &frameFinished, &packet);


    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
