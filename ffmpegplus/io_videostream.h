#pragma once
#include "io_avstream.h"

class in_videostream:public in_avstream
{
public:
	in_videostream();
	~in_videostream();
	int create(AVFormatContext * format);
};

class out_videostream:public out_avstream
{
public:
	out_videostream();
	~out_videostream();
	void init(int width, int height, int framerate = 15, int64_t bitrate = 8000000, AVCodecID id = AV_CODEC_ID_H264);
	int create(AVFormatContext * format);
private:
	int width_;
	int height_;
	int framerate_;
	int64_t bitrate_;
	AVCodecID id_;
};