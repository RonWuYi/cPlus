#include <stdio.h>
#include <iostream>
#include <string>
//
//#ifdef __cplusplus
//extern "C" {
//#endif // __cplusplus
//#include <libavutil/imgutils.h>
//#include <libavutil/parseutils.h>
//#include <libswscale/swscale.h>
//#ifdef __cplusplus
//}
//#endif // __cplusplus

extern "C" {
#define __STDC_CONSTANT_MACROS
#include <libavutil/imgutils.h>
#include <libavutil/parseutils.h>
#include <libswscale/swscale.h>

}


static void fill_yuv_image(uint8_t* data[4], int linesize[4],
	int width, int height, int frame_index)
{
	int x, y;

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			data[0][y * linesize[0] + x] = x + y + frame_index * 3;
		}
	}

	for (y = 0; y < height / 2; y++)
	{
		for (x = 0; x < width / 2; x++)
		{
			data[1][y * linesize[1] + x] = 128 + y + frame_index * 2;
			data[2][y * linesize[2] + x] = 64 + x + frame_index * 5;
		}
	}
}

//enum class AVPixelFormat
//{
//	src_pix_fmt = AV_PIX_FMT_YUV420P,
//	dst_pix_fmt = AV_PIX_FMT_RGB24
//};

int main()
{
	uint8_t* src_data[4], * dst_data[4];
	int src_linesize[4], dst_linesize[4];

	int src_w = 320, src_h = 240;

	/*int src_w = 320, src_h = 240, dst_w=0, dst_h=0;*/

	enum AVPixelFormat src_pix_fmt = AV_PIX_FMT_YUV420P, dst_pix_fmt = AV_PIX_FMT_RGB24;

	const char* dst_size = nullptr;
	//const char* dst_filename = nullptr;
	const std::string file_path = "C:\Work\media\Wildlife.wmv";
	FILE* dst_file;
	int dst_bufsize;

	struct SwsContext* sws_ctx;

	int i, ret;

	//if (argc != 3)
	//{
	//	fprintf(stderr, "Usage: %s output_file output_size\n"
	//		"API example program to show how to scale an image with libswscale.\n"
	//		"This program generates a series of pictures, rescales them to the given "
	//		"output_size and saves them to an output file named output_file\n."
	//		"\n", argv[0]);
	//	exit(1);
	//}

	if (av_parse_video_size(&dst_w))
	{

	}
	errno_t err;
	err = fopen_s(&dst_file, file_path.c_str(), "wb");

	if (err != 0)
	{
		fprintf(stderr, "Could not open destination file %s\n", file_path.c_str());
		exit(1);
	}

	sws_ctx = sws_getContext(src_w, src_h, src_pix_fmt,
		src_w / 2, src_h / 2, dst_pix_fmt,
		SWS_BILINEAR, nullptr, nullptr, nullptr);

	//std::cout << dst_w << std::endl;
	//std::cout << dst_h << std::endl;

	if (!sws_ctx)
	{
		fprintf(stderr,
			"Impossible to create scale context for the conversion "
			"fmt:%s s:%dx%d -> fmt:%s s:%dx%d\n",
			av_get_pix_fmt_name(src_pix_fmt), src_w, src_h,
			av_get_pix_fmt_name(dst_pix_fmt), src_w / 2, src_h /2);
		ret = AVERROR(EINVAL);
		fclose(dst_file);
		av_freep(&src_data[0]);
		av_freep(&dst_data[0]);
		sws_freeContext(sws_ctx);
		return ret < 0;
	}

	if ((ret = av_image_alloc(src_data, src_linesize,
		src_w, src_h, src_pix_fmt, 16)) < 0)
	{
		fprintf(stderr, "Could not allocate source image\n");
		fclose(dst_file);
		av_freep(&src_data[0]);
		av_freep(&dst_data[0]);
		sws_freeContext(sws_ctx);
		return ret < 0;
	}

	if ((ret = av_image_alloc(src_data, src_linesize,
		src_w, src_h, src_pix_fmt, 1)) < 0)
	{
		fprintf(stderr, "Could not allocate destination image\n");
		av_freep(&src_data[0]);
		av_freep(&dst_data[0]);
		sws_freeContext(sws_ctx);
		return ret < 0;
	}

	dst_bufsize = ret;

	for (size_t i = 0; i < 100; i++)
	{
		fill_yuv_image(src_data, src_linesize, src_w, src_h, i);

		sws_scale(sws_ctx, (const uint8_t* const*)src_data,
			src_linesize, 0, src_h, dst_data, dst_linesize);

		fwrite(dst_data[0], 1, dst_bufsize, dst_file);
	}

	fprintf(stderr, "Scaling succeeded. Play the output file with the command:\n"
		"ffplay -f rawvideo -pix_fmt %s -video_size %dx%d %s\n",
		av_get_pix_fmt_name(dst_pix_fmt), src_w / 2, src_h / 2, file_path.c_str());

	return 0;
}
