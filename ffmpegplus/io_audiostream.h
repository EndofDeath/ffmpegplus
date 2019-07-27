#pragma once
#include "io_avstream.h"
class in_audiostream:public in_avstream
{
public:
	in_audiostream();
	~in_audiostream();
	int create(AVFormatContext * format);
};

class out_audiostream:public out_avstream
{
public:
	out_audiostream();
	~out_audiostream();
	void init(int64_t bitrate = 128000, AVCodecID id = AV_CODEC_ID_AAC);
	int create(AVFormatContext * format);
private:
	int64_t bitrate_;
	AVCodecID id_;
};