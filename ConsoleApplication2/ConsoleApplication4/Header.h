#pragma once
#ifndef __HEADER__
#define __HEADER__

#include <SDL2/SDL.h>
#include <SDL2/SDL_thread.h>

#ifdef __cplusplus
extern "C"
{
#endif

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libavutil/avstring.h>

#ifdef __cplusplus
}
#endif


#define SDL_AUDIO_BUFFER_SIZE 1024
#define MAX_AUDIO_FRAME_SIZE 192000

#define MAX_AUDIOQ_SIZE (5 * 16 * 1024)
#define MAX_VIDEOQ_SIZE (5 * 256 * 1024)

#define FF_REFRESH_EVENT (SDL_USEREVENT)
#define FF_QUIT_EVENT (SDL_USEREVENT + 1)

#define VIDEO_PICTURE_QUEUE_SIZE 1

static int decoder_reorder_pts = -1;
static AVPacket flush_pkt;

const static int video_width = 640; 
const static int video_height = 480;

/* Since we only have one decoding thread, the Big Struct
   can be global in case we need it. */
VideoState* global_video_state;

typedef struct MyAVPacketList {
    AVPacket pkt;
    struct MyAVPacketList* next;
    int serial;
} MyAVPacketList;

typedef struct PacketQueue {
    MyAVPacketList* first_pkt, * last_pkt;
    int nb_packets;
    int size;
    int64_t duration;
    int abort_request;
    int serial;
    SDL_mutex* mutex;
    SDL_cond* cond;
} PacketQueue;

typedef struct VideoState {
    AVFormatContext*    pFormatCtx;
    int                 videoStream, audioStream;
    AVStream*           audio_st;
    AVCodecContext*     audio_ctx;
    AVCodecParameters*  pAudioCodecPar;
    PacketQueue         audioq;
    uint8_t             audio_buf[(MAX_AUDIO_FRAME_SIZE * 3) / 2];
    unsigned int        audio_buf_size;
    unsigned int        audio_buf_index;
    AVPacket            audio_pkt;
    uint8_t*            audio_pkt_data;
    int                 audio_pkt_size;
    AVStream*           video_st;
    AVCodecContext*     video_ctx;
    AVCodecParameters*  pVideoCodecPar;
    PacketQueue         videoq;

    /*VideoPicture        pictq[VIDEO_PICTURE_QUEUE_SIZE];*/
    int                 pictq_size, pictq_rindex, pictq_windwx;
    SDL_mutex*          pictq_mutex;
    SDL_cond*           pictq_cond;

    SDL_Thread*         parse_tid;
    SDL_Thread*         video_tid;

    SDL_Texture* vis_texture;
    /*SDL_Texture* sub_texture;*/
    SDL_Texture* vid_texture;

    struct SwrContext* swr_ctx;
    struct SwsContext* img_convert_ctx;
    /*struct SwsContext* sub_convert_ctx;*/

    char* filename;

    int quit;


} VideoState;




typedef struct Decoder {
    AVPacket pkt;
    PacketQueue* queue;
    AVCodecContext* avctx;
    int pkt_serial;
    int finished;
    int packet_pending;
    SDL_cond* empty_queue_cond;
    int64_t start_pts;
    AVRational start_pts_tb;
    int64_t next_pts;
    AVRational next_pts_tb;
    SDL_Thread* decoder_tid;
} Decoder;

/* return < 0 if aborted, 0 if no packet and > 0 if packet.  */
static int packet_queue_get(PacketQueue* q, AVPacket* pkt, int block, int* serial);
//{
//    MyAVPacketList* pkt1;
//    int ret;
//
//    SDL_LockMutex(q->mutex);
//
//    for (;;) {
//        if (q->abort_request) {
//            ret = -1;
//            break;
//        }
//
//        pkt1 = q->first_pkt;
//        if (pkt1) {
//            q->first_pkt = pkt1->next;
//            if (!q->first_pkt)
//                q->last_pkt = NULL;
//            q->nb_packets--;
//            q->size -= pkt1->pkt.size + sizeof(*pkt1);
//            q->duration -= pkt1->pkt.duration;
//            *pkt = pkt1->pkt;
//            if (serial)
//                *serial = pkt1->serial;
//            av_free(pkt1);
//            ret = 1;
//            break;
//        }
//        else if (!block) {
//            ret = 0;
//            break;
//        }
//        else {
//            SDL_CondWait(q->cond, q->mutex);
//        }
//    }
//    SDL_UnlockMutex(q->mutex);
//    return ret;
//}
int new_decode(AVCodecContext* pAvctx, AVFrame* pFrame, int* got_frame, AVPacket* pkt);

static int decoder_decode_frame(Decoder* d, AVFrame*, AVSubtitle*);

int stream_component_open(VideoState* is, int stream_index);

int decode_thread(void* arg);

int packet_queue_put(PacketQueue* q, AVPacket* pkt);

#endif // !__HEADER__

