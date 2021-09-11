#include "CommonHeader.h"
#include "Header.h"

int new_decode(AVCodecContext* pAvctx, AVFrame* pFrame, int* got_frame, AVPacket* pkt)
{
    int ret;
    *got_frame = 0;

    if (pkt)
    {
        ret = avcodec_send_packet(pAvctx, pkt);

        if (ret < 0)
        {
            return ret == AVERROR_EOF ? 0 : ret;
        }
    }

    ret = avcodec_receive_frame(pAvctx, pFrame);

    if (ret < 0 && ret == AVERROR(EAGAIN) && ret != AVERROR_EOF)
    {
        return ret;
    }

    if (ret >= 0)
    {
        *got_frame = 1;

    }
    return 0;
}

static int decoder_decode_frame(Decoder* d, AVFrame* frame, AVSubtitle* sub) {
    int ret = AVERROR(EAGAIN);

    for (;;) {
        AVPacket pkt;

        if (d->queue->serial == d->pkt_serial) {
            do {
                if (d->queue->abort_request)
                    return -1;

                switch (d->avctx->codec_type) {
                case AVMEDIA_TYPE_VIDEO:
                    ret = avcodec_receive_frame(d->avctx, frame);
                    if (ret >= 0) {
                        if (decoder_reorder_pts == -1) {
                            frame->pts = frame->best_effort_timestamp;
                        }
                        else if (!decoder_reorder_pts) {
                            frame->pts = frame->pkt_dts;
                        }
                    }
                    break;
                case AVMEDIA_TYPE_AUDIO:
                    ret = avcodec_receive_frame(d->avctx, frame);
                    if (ret >= 0) {
                        /*AVRational*/
                        /*AVRational tb = (AVRational){ 1, frame->sample_rate };*/
                        AVRational tb = { 1, frame->sample_rate };
                        if (frame->pts != AV_NOPTS_VALUE)
                            frame->pts = av_rescale_q(frame->pts, d->avctx->pkt_timebase, tb);
                        else if (d->next_pts != AV_NOPTS_VALUE)
                            frame->pts = av_rescale_q(d->next_pts, d->next_pts_tb, tb);
                        if (frame->pts != AV_NOPTS_VALUE) {
                            d->next_pts = frame->pts + frame->nb_samples;
                            d->next_pts_tb = tb;
                        }
                    }
                    break;
                }
                if (ret == AVERROR_EOF) {
                    d->finished = d->pkt_serial;
                    avcodec_flush_buffers(d->avctx);
                    return 0;
                }
                if (ret >= 0)
                    return 1;
            } while (ret != AVERROR(EAGAIN));
        }

        do {
            if (d->queue->nb_packets == 0)
                SDL_CondSignal(d->empty_queue_cond);
            if (d->packet_pending) {
                av_packet_move_ref(&pkt, &d->pkt);
                d->packet_pending = 0;
            }
            else {
                if (packet_queue_get(d->queue, &pkt, 1, &d->pkt_serial) < 0)
                    return -1;
            }
        } while (d->queue->serial != d->pkt_serial);

        if (pkt.data == flush_pkt.data) {
            avcodec_flush_buffers(d->avctx);
            d->finished = 0;
            d->next_pts = d->start_pts;
            d->next_pts_tb = d->start_pts_tb;
        }
        else {
            if (d->avctx->codec_type == AVMEDIA_TYPE_SUBTITLE) {
                int got_frame = 0;
                ret = avcodec_decode_subtitle2(d->avctx, sub, &got_frame, &pkt);
                if (ret < 0) {
                    ret = AVERROR(EAGAIN);
                }
                else {
                    if (got_frame && !pkt.data) {
                        d->packet_pending = 1;
                        av_packet_move_ref(&d->pkt, &pkt);
                    }
                    ret = got_frame ? 0 : (pkt.data ? AVERROR(EAGAIN) : AVERROR_EOF);
                }
            }
            else {
                if (avcodec_send_packet(d->avctx, &pkt) == AVERROR(EAGAIN)) {
                    av_log(d->avctx, AV_LOG_ERROR, "Receive_frame and send_packet both returned EAGAIN, which is an API violation.\n");
                    d->packet_pending = 1;
                    av_packet_move_ref(&d->pkt, &pkt);
                }
            }
            av_packet_unref(&pkt);
        }
    }
}


int fail_exit(void *arg)
{
    VideoState* is = (VideoState*)arg;
    if (1) {
        SDL_Event event;
        event.type = FF_QUIT_EVENT;
        event.user.data1 = is;
        SDL_PushEvent(&event);
    }
    return 0;
}

int packet_queue_put(PacketQueue* q, AVPacket* pkt) {
    AVPacketList* pkt1;
    AVPacket* tmp_pkt;
    // av_packet_ref or av_packet_make_refcounted

    //if (av_dup_packet())
    //{

    //}
    if (av_packet_ref(tmp_pkt, pkt) < 0)
    {
        return -1;
    }

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
}

int decode_thread(void* arg)
{
    VideoState* is = (VideoState*)arg;
    AVFormatContext* pFormatCtx;
    AVPacket pkt1;
    AVPacket* packet = &pkt1;
    int audio_index = -1;
    int video_index = -1;
    int i;

    is->videoStream = -1;
    is->audioStream = -1;

    global_video_state = is;

    if (avformat_open_input(&pFormatCtx, is->filename, NULL, NULL) != 0)
    {
#ifndef _DEBUG
        LOG(ERROR) << "Couldn't open file!\n";
#endif // !DEBUG
        return -1;
    }

    is->pFormatCtx = pFormatCtx;

    if (avformat_find_stream_info(pFormatCtx, NULL) < 0)
    {
#ifndef _DEBUG
        LOG(ERROR) << "Couldn't find stream information!\n";
#endif // !DEBUG
        return -1;
    }

    av_dump_format(pFormatCtx, 0, is->filename, 0);

    for (i = 0; i < pFormatCtx->nb_streams; ++i)
    {
        if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO &&
            video_index < 0) {
            video_index = i;
        }
        if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO &&
            audio_index < 0) {
            audio_index = i;
        }
    }

    if (audio_index > 0)
    {
        stream_component_open(is, audio_index);
    }

    if (video_index >= 0)
    {
        stream_component_open(is, video_index);
    }

    if (is->videoStream < 0 || is->audioStream < 0) {
#ifndef _DEBUG
        LOG(ERROR) << "%s: could not open codecs\n", is->filename;
#endif // !DEBUG
        fprintf(stderr, "%s: could not open codecs\n", is->filename);
        fail_exit(is);
    }

    for (;;)
    {
        if (is->quit)
        {
            break;
        }

        if (is->audioq.size > MAX_AUDIOQ_SIZE ||
            is->videoq.size > MAX_VIDEOQ_SIZE)
        {
            SDL_Delay(10);
            continue;
        }

        if (av_read_frame(is->pFormatCtx, packet) < 0)
        {
            if (is->pFormatCtx->pb->error == 0)
            {
                SDL_Delay(100);
                continue;
            }
            else
            {
                break;
            }
        }

        if (packet->stream_index == is->videoStream)
        {
            packet_queue_put()
        }
        {

        }
    }

    return 0;
}

static int packet_queue_get(PacketQueue* q, AVPacket* pkt, int block, int* serial)
{
    MyAVPacketList* pkt1;
    int ret;

    SDL_LockMutex(q->mutex);

    for (;;) {
        if (q->abort_request) {
            ret = -1;
            break;
        }

        pkt1 = q->first_pkt;
        if (pkt1) {
            q->first_pkt = pkt1->next;
            if (!q->first_pkt)
                q->last_pkt = NULL;
            q->nb_packets--;
            q->size -= pkt1->pkt.size + sizeof(*pkt1);
            q->duration -= pkt1->pkt.duration;
            *pkt = pkt1->pkt;
            if (serial)
                *serial = pkt1->serial;
            av_free(pkt1);
            ret = 1;
            break;
        }
        else if (!block) {
            ret = 0;
            break;
        }
        else {
            SDL_CondWait(q->cond, q->mutex);
        }
    }
    SDL_UnlockMutex(q->mutex);
    return ret;
}

int stream_component_open(VideoState* is, int stream_index) {
    AVFormatContext* pFormatCtx = is->pFormatCtx;
    AVCodecContext* codecCtx;

    //*sizeof(AVCodecParameters) is not a part of the public ABI, this struct must
    //    * be allocated with avcodec_parameters_alloc() and freed with
    //    * avcodec_parameters_free().
    AVCodecParameters* pAudioCodecPar = is->pAudioCodecPar;
    AVCodecParameters* pVideoCodecPar = is->pVideoCodecPar;
    AVCodec* codec;
    SDL_AudioSpec wanted_spec, spec;

    if (stream_index < 0 || stream_index >= pFormatCtx->nb_streams)
    {
        return -1;
    }

    codec = avcodec_find_decoder(pFormatCtx->streams[stream_index]->codecpar->codec_id);
    if (!codec)
    {
        LOG(ERROR) << "Unsupported codec!\n";
        fprintf(stderr, "Unsupported codec!\n");
        return -1;
    }
    unsigned int i;
    //for (i = 0; i < pFormatCtx->nb_streams; i++)
    if (pFormatCtx->streams[stream_index]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
        if (avcodec_parameters_to_context(codecCtx, pFormatCtx->streams[stream_index]->codecpar) != 0 )
        {
            fprintf(stderr, "Couldn't copy codec context");
            LOG(ERROR) << "Couldn't copy codec context!\n";
            return -1; // Error copying codec context
        }
        //videoStream = i;
        //break;
    }
    else if (pFormatCtx->streams[stream_index]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO){
        if (avcodec_parameters_to_context(codecCtx, pFormatCtx->streams[stream_index]->codecpar) != 0)
        {
            fprintf(stderr, "Couldn't copy codec context");
            LOG(ERROR) << "Couldn't copy codec context!\n";
            return -1; // Error copying codec context
        }

        wanted_spec.freq = codecCtx->sample_rate;
        wanted_spec.callback = audio_callback;
        wanted_spec.userdata = is;

        if (SDL_OpenAudio(&wanted_spec, &spec) < 0)
        {
#ifndef _DEBUG
            LOG(ERROR) << "SDL_OpenAudio error!\n";
            fprintf(stderr, "SDL_OpenAudio: %s\n", SDL_GetError());
#endif // !DEBUG
            return -1;
        }

        if (avcodec_open2(codecCtx, codec, NULL) < 0)
        {
#ifndef _DEBUG
            LOG(ERROR) << "Unsupported codec for audio stream!\n";
            fprintf(stderr, "Unsupported codec for audio stream!\n");
#endif // !DEBUG
            return -1;
        }

        switch (codecCtx->codec_type)
        {
        case AVMEDIA_TYPE_AUDIO:
            is->audioStream = stream_index;
            is->audio_st = pFormatCtx->streams[stream_index];
            is->audio_ctx = codecCtx;
            is->pAudioCodecPar = pFormatCtx->streams[stream_index]->codecpar;
            is->audio_buf_size = 0;
            is->audio_buf_index = 0;
            memset(&is->audio_pkt, 0, sizeof(is->audio_pkt));
            packet_queue_init(&is->audioq);
            SDL_PauseAudio(0);
            break;
        case AVMEDIA_TYPE_VIDEO:
            is->videoStream = stream_index;
            is->video_st = pFormatCtx->streams[stream_index];
            is->video_ctx = codecCtx;
            is->pVideoCodecPar = pFormatCtx->streams[stream_index]->codecpar;
            packet_queue_init(&is->videoq);
            is->video_tid = SDL_CreateThread(video_thread, is);

            break;
        default:
            break;
        }

        //if (new_decode(codecCtx, codec, NULL) < 0)
        //{

        //}

    }

    else if (pFormatCtx->streams[stream_index]->codecpar->codec_type == AVMEDIA_TYPE_SUBTITLE) {
        //if (avcodec_parameters_to_context(codecCtx, pFormatCtx->streams[stream_index]->codecpar) != 0)
        //{
        //    fprintf(stderr, "Couldn't copy codec context");
        //    LOG(ERROR) << "Couldn't copy codec context!\n";
        //    return -1; // Error copying codec context
        //}

        return -1; // Error copying codec context

    }


    else if (pFormatCtx->streams[stream_index]->codecpar->codec_type == AVMEDIA_TYPE_UNKNOWN) {
        return -1; // Error copying codec context
    }

    else
    {
        return -1;
    }



    //if (videoStream == -1)


    codecCtx = avcodec_alloc_context3(codec);

    // avcodec_parameters_to_context

    if (avcodec_copy_context())
    {

    }
}