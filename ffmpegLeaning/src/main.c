#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>

#include <stdio.h>


void SaveFrame(AVFrame *pFrame, int width, int height, int iFrame){
    FILE *pFile;
    char szFilename[32];
    int y;

    // Open file
    sprintf(szFilename, "frame%d.ppm", iFrame);
    pFile=fopen(szFilename, "wb+");
    if (pFile==NULL)
        return;

    // Write header
    fprintf(pFile, "P6\n%d %d\n255\n", width, height);

    // Write pixel data
    for (y = 0; y < height; ++y) {
        fwrite(pFrame->data[0]+y*pFrame->linesize[0], 1, width*3, pFile);
    }

    // Close file
    fclose(pFile);

}


int main(int argc, char *argv[]){
//    av_register_all();
    AVFormatContext *pFormatCtx = NULL;
    int i, videoStream;
    AVCodecContext *pCodecCtxOrig = NULL;
//    AVCodecContext *pCodecCtxOrig2 = NULL;
    AVCodecParameters *pCodecCtx = NULL;
    AVCodec *pCodec =NULL;
    AVFrame *pFrame = NULL;
    AVFrame *pFrameRGB = NULL;
    AVPacket *packet;
    int frameFinished = 0;
    int numBytesYUV;
    uint8_t *buffer = NULL;

    // Reading the Data
    struct SwsContext *sws_ctx =NULL;
    AVDictionary *optionDict =  NULL;
    char filepath[]="C:\\Users\\Public\\Videos\\Sample Videos\\Wildlife.wmv";

    // Open video file
    if (avformat_open_input(&pFormatCtx, filepath, NULL, NULL)!=0){
        return -1;
    }

    if (avformat_find_stream_info(pFormatCtx, NULL)<0)
        return -1;

    // Dump information
    av_dump_format(pFormatCtx, 0, argv[1], 0);

    for (i = 0; i < pFormatCtx->nb_streams; ++i) {
        if (pFormatCtx->streams[i]->codecpar->codec_type==AVMEDIA_TYPE_VIDEO){
            videoStream=i;
            break;
        }
    }

    if (videoStream==-1)
        return -1; // Didn't find a video stream

    pCodecCtx=pFormatCtx->streams[videoStream]->codecpar;

    // Find the decoder for the video stream
    pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
    if (pCodec==NULL){
        fprintf(stderr, "Unsupported codec!\n");
        return -1;
    }

    // Open codec
//     avcodec_decode_video2()
    if(avcodec_open2(pCodecCtxOrig, pCodec, NULL)<0)
        return -1;

    pFrame=av_frame_alloc();

    // Allocate an AVFrame structure

    pFrameRGB=av_frame_alloc();
    if (pFrameRGB==NULL)
        return -1;

    // Determine required buffer size and allocate buffer
    numBytesYUV = av_image_get_buffer_size(AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height, 1);

    buffer=(uint8_t *)av_malloc(numBytesYUV * sizeof(uint8_t));

    // Copy context
    pCodecCtxOrig = avcodec_alloc_context3(pCodec);

    // Decoder
    avcodec_parameters_to_context(pCodecCtxOrig, pCodecCtx);
    if (avcodec_parameters_to_context(pCodecCtxOrig, pCodecCtx)!=0){
        fprintf(stderr, "Couldn't copy codec context");
        return -1;
    };

    sws_ctx = sws_getContext(pCodecCtxOrig->width,
                             pCodecCtxOrig->height,
                             pCodecCtxOrig->pix_fmt,
                             pCodecCtxOrig->width,
                             pCodecCtxOrig->height,
                             AV_PIX_FMT_YUV420P,
                             SWS_BILINEAR,
                             NULL,
                             NULL,
                             NULL
    );

    av_image_fill_arrays(pFrame->data, pFrame->linesize, buffer,
                         AV_PIX_FMT_YUV420P, pCodecCtxOrig->width, pCodecCtxOrig->height, 1);

    /*
    int numBytesRGB;
    // Determin required buffer size and allocate buffer
    // deprecated function
//    avpicture_get_size();
//    avpicture_fill();

    numBytesRGB = av_image_get_buffer_size(AV_PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height, 1);

    buffer=(uint8_t *)av_malloc(numBytesRGB * sizeof(uint8_t));
    */


    // Assign appropriate parts of buffer to image planes in pFrameRGB
    // avpicture_fill();



    // Initialize SWS context for software scaling

    /*
    sws_ctx = sws_getContext(pCodecCtxOrig->width,
                             pCodecCtxOrig->height,
                             pCodecCtxOrig->pix_fmt,
                             pCodecCtxOrig->width,
                             pCodecCtxOrig->height,
                             AV_PIX_FMT_RGB24,
                             SWS_BILINEAR,
                             NULL,
                             NULL,
                             NULL
                            );
    */

    i=0;
    int ret;
    //int *got_frame;
    // ToDo bug here
    packet=(AVPacket *)av_malloc(sizeof(AVPacket));
    while (av_read_frame(pFormatCtx, packet)>=0){

        // Is this a packet from the video stream?
        if (packet->stream_index==videoStream){
            // Decode video frame
            // avcodec_decode_video2();
            ret = avcodec_send_packet(pCodecCtxOrig, packet);

            if (ret < 0){
                return ret == AVERROR_EOF ? 0 : ret;
            }

        ret = avcodec_receive_frame(pCodecCtxOrig, pFrame);

        if (ret < 0 &&ret!=AVERROR(EAGAIN)&&ret!=AVERROR_EOF)
            return ret;

        if (ret >= 0)
//            *got_frame = 1;
            frameFinished = 1;

        if (frameFinished){
            // Convert the image from its native format to RGB
//            sws_ctx = sws_getContext(pCodecCtxOrig->width, pCodecCtxOrig->height, pCodecCtxOrig->pix_fmt, pCodecCtxOrig->width, pCodecCtxOrig->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);
            sws_scale(sws_ctx, (uint8_t const * const *)pFrame->data,
                      pFrame->linesize, 0, pCodecCtxOrig->height,
                      pFrameRGB->data, pFrameRGB->linesize);
            // Save the frame to disk
            if (++i<=5)
                SaveFrame(pFrameRGB, pCodecCtxOrig->width, pCodecCtxOrig->height, 1);

        }
        }
        av_packet_unref(packet);
    }


    // Free the RGB image
    av_free(buffer);
    av_free(pFrameRGB);

    // Free the YUV frame
    av_free(pFrame);

    // Close the codecs
    avcodec_close(pCodecCtxOrig);

    // Close the vidoe file
    avformat_close_input(&pFormatCtx);

    return 0;

}