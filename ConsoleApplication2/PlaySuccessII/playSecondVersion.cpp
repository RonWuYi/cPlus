#include <stdio.h>
#include <iostream>
#include <string>

#ifdef __cplusplus
extern "C" {
#endif

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/parseutils.h>
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

void fill_yuv_image(uint8_t* data[4], int linesize[4],
	int width, int height, int frame_index)
{
	int x, y;

	/* Y */
	for (y = 0; y < height; y++)
		for (x = 0; x < width; x++)
			data[0][y * linesize[0] + x] = x + y + frame_index * 3;

	/* Cb and Cr */
	for (y = 0; y < height / 2; y++) {
		for (x = 0; x < width / 2; x++) {
			data[1][y * linesize[1] + x] = 128 + y + frame_index * 2;
			data[2][y * linesize[2] + x] = 64 + x + frame_index * 5;
		}
	}
}

//int main(int argc, char* argv[]) {
int main(int argc, char* argv[]) {
	AVFormatContext* pFormatCtx = NULL;
	int videoStream;
	unsigned int i;
	AVCodecContext* pCodecCtxOrig = NULL;
	AVCodecContext* pCodecCtx = NULL;
	AVCodecParameters* pCodecParOrig = NULL;
	AVCodec* pCodec = NULL;
	AVFrame* pFrame = NULL;
	AVFrame* pFrameYUV = NULL;
	AVPacket* packet;
	int frameFinished = 0;
	struct SwsContext* sws_ctx = NULL;

	uint8_t* buffer = NULL;

	SDL_Event event;
	SDL_Window* screen;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	SDL_Rect rect;

	Uint8* yPlane, * uPlane, * vPlane;
	size_t yPlaneSz, uvPlaneSz;
	int uvPitch;

	//if (argc < 2) {
	//	fprintf(stderr, "Usage: test <file>\n");
	//	exit(1);
	//}
	// Register all formats and codecs
	//av_register_all();

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
		fprintf(stderr, "Could not initialize SDL - %s\n", SDL_GetError());
		exit(1);
	}

	//std::string file_path = "‪C:/Work/media/Wildlife.wmv";

	const char* file_path = "C:/Work/media/Wildlife.wmv";

	// Open video file
	if (avformat_open_input(&pFormatCtx, file_path, NULL, NULL) != 0)
	//if (avformat_open_input(&pFormatCtx, argv[1], NULL, NULL) != 0)
		return -1; // Couldn't open file

	// Retrieve stream information
	if (avformat_find_stream_info(pFormatCtx, NULL) < 0)
		return -1; // Couldn't find stream information

	// Dump information about file onto standard error
	av_dump_format(pFormatCtx, 0, argv[1], 0);

	// Find the first video stream
	videoStream = -1;
	for (i = 0; i < pFormatCtx->nb_streams; i++)
		if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
			videoStream = i;
			break;
		}
	if (videoStream == -1)
		return -1; // Didn't find a video stream

	// Get a pointer to the codec context for the video stream
	pCodecParOrig = pFormatCtx->streams[videoStream]->codecpar;
	// Find the decoder for the video stream
	pCodec = avcodec_find_decoder(pCodecParOrig->codec_id);
	if (pCodec == NULL) {
		fprintf(stderr, "Unsupported codec!\n");
		return -1; // Codec not found
	}

	// Copy context
	pCodecCtx = avcodec_alloc_context3(pCodec);
	if (avcodec_parameters_to_context(pCodecCtx, pCodecParOrig) != 0) {
		fprintf(stderr, "Couldn't copy codec context");
		return -1; // Error copying codec context
	}

	uint8_t* src_data[4], * dst_data[4];
	int src_linesize[4], dst_linesize[4];

	enum AVPixelFormat src_pix_fmt = AV_PIX_FMT_YUV420P, dst_pix_fmt = AV_PIX_FMT_YUV420P;

	int src_w = pCodecCtx->width;
	int dst_w = pCodecCtx->width / 2;
	int src_h = pCodecCtx->height;
	int dst_h = pCodecCtx->height / 2;


	// Open codec
	if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0)
		return -1; // Could not open codec


	// Allocate video frame
	pFrame = av_frame_alloc();
	pFrameYUV = av_frame_alloc();

	buffer = (uint8_t*)av_malloc(av_image_get_buffer_size(
		AV_PIX_FMT_YUV420P,
		pCodecCtx->width, pCodecCtx->height, 1));
	//av_parse_video_size();
	av_image_fill_arrays(pFrameYUV->data, pFrameYUV->linesize,
		buffer,
		AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height, 1);

	// Init packet
	packet = (AVPacket*)av_malloc(sizeof(AVPacket));


	// Make a screen to put our video
	screen = SDL_CreateWindow(
		"FFmpeg Tutorial",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		dst_w,
		dst_h,
		0
	);

	if (!screen) {
		fprintf(stderr, "SDL: could not create window - exiting\n");
		exit(1);
	}

	renderer = SDL_CreateRenderer(screen, -1, 0);
	if (!renderer) {
		fprintf(stderr, "SDL: could not create renderer - exiting\n");
		exit(1);
	}

	// Allocate a place to put our YUV image on that screen
	texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_YV12,
		SDL_TEXTUREACCESS_STREAMING,
		dst_w,
		dst_h
	);

	if (!texture) {
		fprintf(stderr, "SDL: could not create texture - exiting\n");
		exit(1);
	}

	rect.x = 0;
	rect.y = 0;
	rect.w = dst_w;
	rect.h = dst_h;

	// initialize SWS context for software scaling
	sws_ctx = sws_getContext(src_w, src_h,
		pCodecCtx->pix_fmt, dst_w, dst_h,
		AV_PIX_FMT_YUV420P,
		SWS_BILINEAR,
		NULL,
		NULL,
		NULL);

	int ret = 0;

	/* allocate source and destination image buffers */
	if ((ret = av_image_alloc(src_data, src_linesize,
		src_w, src_h, src_pix_fmt, 16)) < 0) {
		fprintf(stderr, "Could not allocate source image\n");
		//fclose(dst_file);
		av_freep(&src_data[0]);
		av_freep(&dst_data[0]);
		sws_freeContext(sws_ctx);
		return ret < 0;
	}

	/* buffer is going to be written to rawvideo file, no alignment */
	if ((ret = av_image_alloc(dst_data, dst_linesize,
		dst_w, dst_h, dst_pix_fmt, 1)) < 0) {
		fprintf(stderr, "Could not allocate destination image\n");
		//fclose(dst_file);
		av_freep(&src_data[0]);
		av_freep(&dst_data[0]);
		sws_freeContext(sws_ctx);
		return ret < 0;
	}
	int dst_bufsize;
	dst_bufsize = ret;

	// set up YV12 pixel array (12 bits per pixel)
	yPlaneSz = pCodecCtx->width * pCodecCtx->height;
	uvPlaneSz = pCodecCtx->width * pCodecCtx->height / 4;
	yPlane = (Uint8*)malloc(yPlaneSz);
	uPlane = (Uint8*)malloc(uvPlaneSz);
	vPlane = (Uint8*)malloc(uvPlaneSz);

	if (!yPlane || !uPlane || !vPlane) {
		fprintf(stderr, "Could not allocate pixel buffers - exiting\n");
		exit(1);
	}

	uvPitch = pCodecCtx->width / 2;

	while (av_read_frame(pFormatCtx, packet) >= 0)
	{
		if (packet->stream_index == videoStream)
		{
			/*avcodec_decode_video2();*/
			new_decode(pCodecCtx, pFrame, &frameFinished, packet);

			// Did we get a video frame?
			if (frameFinished) {

				//pFrame->data[0] = yPlane;
				//pFrame->data[1] = uPlane;
				//pFrame->data[2] = vPlane;
				//pFrame->linesize[0] = pCodecCtx->width;
				//pFrame->linesize[1] = uvPitch;
				//pFrame->linesize[2] = uvPitch;

				// Convert the image into YUV format that SDL uses
				sws_scale(sws_ctx, (uint8_t const* const*)pFrame->data,
					pFrame->linesize, 0, pCodecCtx->height, pFrameYUV->data,
					pFrameYUV->linesize);

				SDL_UpdateYUVTexture(
					texture,
					NULL,
					pFrameYUV->data[0],
					pFrameYUV->linesize[0],
					pFrameYUV->data[1],
					pFrameYUV->linesize[1],
					pFrameYUV->data[2],
					pFrameYUV->linesize[2]
				);

				SDL_RenderClear(renderer);
				SDL_RenderCopy(renderer, texture, NULL, &rect);
				SDL_RenderPresent(renderer);

				//Delay 40ms
				SDL_Delay(40);
			}
		}

		// Free the packet that was allocated by av_read_frame
		av_packet_unref(packet);
		SDL_PollEvent(&event);
		switch (event.type) {
		case SDL_QUIT:
			SDL_DestroyTexture(texture);
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(screen);
			SDL_Quit();
			exit(0);
			break;
		default:
			break;

		}
	}

	// Free the YUV frame
	av_frame_free(&pFrame);
	free(yPlane);
	free(uPlane);
	free(vPlane);

	// Close the codec
	avcodec_close(pCodecCtx);
	avcodec_close(pCodecCtxOrig);

	// Close the video file
	avformat_close_input(&pFormatCtx);

	return 0;
}