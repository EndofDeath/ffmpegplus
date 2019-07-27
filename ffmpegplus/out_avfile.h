#pragma once
#include "io_avformat.h"
#include "io_videostream.h"
#include "io_audiostream.h"

class out_avfile
{
public:
	out_avfile();
	~out_avfile();
	int create(const char*filename);
	int addvideo(int width, int height, int framerate = 15, int64_t bitrate = 8000000);
	int addaudio(int64_t bitrate = 128000);
	int open();
	void close();
	int writevideo();
	int writeaudio();
	out_avformat& getformat()
	{
		return format;
	}
	out_videostream& getvideo()
	{
		return video;
	}
	out_audiostream& getaudio()
	{
		return audio;
	}
private:
	out_avformat format;
	out_videostream video;
	out_audiostream audio;
};