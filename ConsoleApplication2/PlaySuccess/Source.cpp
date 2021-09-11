#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>

#ifdef __cplusplus
}
#endif
#include <SDL2/SDL.h>
#include <SDL2/SDL_thread.h>


#ifdef _WIN32
#undef main /* Prevents SDL from overriding main() */
#endif

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

//int new_decode(AVCodecContext *avctx, AVFrame *frame,
//	int got_frame, AVPacket *pkt) {
//	int ret;
//
//	got_frame = 0;
//
//	if (pkt)
//	{
//		ret = avcodec_send_packet(avctx, pkt);
//
//		// decoded frames with avcodec_send_packet
//		if (ret < 0)
//		{
//			return ret == AVERROR_EOF ? 0 : ret;
//		}
//	}
//
//	ret = avcodec_receive_frame(avctx, frame);
//	if (ret<0 && ret != AVERROR(EAGAIN) && ret != AVERROR_EOF)
//	{
//		return ret;
//	}
//
//	if (ret >= 0)
//		got_frame = 1;
//	return 0;
//}


int main(int argc, char* argv[]) {
	AVFormatContext* pFormatCtx = NULL;
	int						i, videoStream;
	AVCodecContext* pCodecCtx = NULL;
	AVCodecParameters* pCodecPar = NULL;
	AVCodec* pCodec = NULL;
	AVFrame* pFrame = NULL;
	AVFrame* pFrameYUV = NULL;
	AVPacket* packet;
	int						frameNotFinished;
	int						screen_w = 1280;
	int						screen_h = 720;
	uint8_t* buffer = NULL;

	AVDictionary* optionsDict = NULL;
	struct SwsContext* sws_ctx = NULL;

	//SDL_Overlay				*bmp = NULL;
	SDL_Window* pWindow;
	SDL_Texture* sdlTexture;
	SDL_Renderer* sdlRenderer;
	SDL_Surface* screen = NULL;
	SDL_Rect				rect;
	SDL_Event				event;

	// char filepath[] = "C:/Work/media/Wildlife.wmv";
	/*char filepath[] = "C:/Users/ron.wu/Downloads/IMG-4383.JPG";*/

	if (argc < 2) {
		fprintf(stderr, "Usage: test <file>\n");
		exit(1);
	}
	// Register all formats and codecs, not use anymore
	//av_register_all();

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

				   // Find the first video stream
	videoStream = -1;
	for (unsigned int i = 0; i < pFormatCtx->nb_streams; i++)
		if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
			videoStream = i;
			break;
		}
	if (videoStream == -1)
		return -1; // Didn't find a video stream

				   // Get a AVCodecParameters from AVFormatContext
	pCodecPar = pFormatCtx->streams[videoStream]->codecpar;

	// Find the decoder for the video stream
	pCodec = avcodec_find_decoder(pCodecPar->codec_id);
	if (pCodec == NULL) {
		fprintf(stderr, "Unsupported codec!\n");
		return -1; // Codec not found
	}

	// Allocate an AVCodecContext and set its fields to default values
	pCodecCtx = avcodec_alloc_context3(pCodec);

	if (avcodec_parameters_to_context(pCodecCtx, pCodecPar) != 0) {
		fprintf(stderr, "Couldn't Fill the codec context based on the values from the supplied codec parameters.");
		return -1; // Error copying codec context
	}

	// Open codec
	if (avcodec_open2(pCodecCtx, pCodec, &optionsDict) < 0)
		return -1; // Could not open codec

				   // Allocate video frame
	pFrame = av_frame_alloc();
	pFrameYUV = av_frame_alloc();

	buffer = (uint8_t*)av_malloc(av_image_get_buffer_size(
		AV_PIX_FMT_YUV420P,
		pCodecPar->width, pCodecPar->height, 1));

	av_image_fill_arrays(pFrameYUV->data, pFrameYUV->linesize,
		buffer,
		AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height, 1);

	// Init packet
	packet = (AVPacket*)av_malloc(sizeof(AVPacket));

	//Output Info-----------------------------
	printf("--------------- File Information ----------------\n");
	av_dump_format(pFormatCtx, 0, argv[1], 0);
	printf("-------------------------------------------------\n");

	sws_ctx = sws_getContext
	(
		pCodecCtx->width,
		pCodecCtx->height,
		pCodecCtx->pix_fmt,
		pCodecCtx->width,
		pCodecCtx->height,
		AV_PIX_FMT_YUV420P,
		SWS_BILINEAR,
		NULL,
		NULL,
		NULL
	);

	pWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		screen_w, screen_h, SDL_WINDOW_SHOWN);

	if (pWindow == NULL) {
		fprintf(stderr, "SDL: could not set video mode - exiting\n");
		exit(1);
	}

	sdlRenderer = SDL_CreateRenderer(pWindow, -1, 0);

	//IYUV: Y + U + V  (3 planes)
	//YV12: Y + V + U  (3 planes)
	sdlTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_IYUV,
		SDL_TEXTUREACCESS_STREAMING, pCodecCtx->width,
		pCodecCtx->height);


	rect.x = 0;
	rect.y = 0;
	rect.w = screen_w;
	rect.h = screen_h;

	// Read frames and save first five frames to disk
	i = 0;
	while (av_read_frame(pFormatCtx, packet) >= 0) {
		// Is this a packet from the video stream?
		if (packet->stream_index == videoStream) {
			// Decode video frame

			new_decode(pCodecCtx, pFrame, &frameNotFinished,
				packet);

			// Did we get a video frame?
			if (frameNotFinished) {
				// Convert the image into YUV format that SDL uses
				sws_scale(sws_ctx,
					(const unsigned char* const*)pFrame->data,
					pFrame->linesize, 0, pCodecCtx->height,
					pFrameYUV->data, pFrameYUV->linesize);

				SDL_UpdateYUVTexture(sdlTexture, &rect,
					pFrameYUV->data[0], pFrameYUV->linesize[0],
					pFrameYUV->data[1], pFrameYUV->linesize[1],
					pFrameYUV->data[2], pFrameYUV->linesize[2]);

				//SDL_UnlockYUVOverlay(bmp);

				SDL_RenderClear(sdlRenderer);
				SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, &rect);
				SDL_RenderPresent(sdlRenderer);

				//Delay 40ms
				SDL_Delay(40);

			}
		}

		// Free the packet that was allocated by av_read_frame
		av_packet_unref(packet);
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

	// Close the codec
	avcodec_close(pCodecCtx);

	// Close the video file
	avformat_close_input(&pFormatCtx);

	return 0;

}