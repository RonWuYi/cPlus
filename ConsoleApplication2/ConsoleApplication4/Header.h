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

#ifdef __cplusplus
}
#endif

static int decoder_reorder_pts = -1;
static AVPacket flush_pkt;

const static int video_width = 640; 
const static int video_height = 480;

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

#endif // !__HEADER

