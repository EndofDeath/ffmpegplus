#pragma once
#include "io_avformat.h"
#include "io_videostream.h"
#include "io_audiostream.h"

class in_avsource
{
public:
	in_avsource();
	~in_avsource();
	virtual int create(const char* short_name, const char* url) = 0;
	virtual int read() = 0;
protected:
	in_avformat format;
};

class in_videosource :virtual public in_avsource
{
public:
	int create(const char* short_name, const char* url);
	int read();
	in_videostream& getvideo()
	{
		return video;
	}
protected:
	in_videostream video;
};

class in_audiosource :virtual public in_avsource
{
public:
	int create(const char* short_name, const char* url);
	int read();
	in_audiostream& getaudio()
	{
		return audio;
	}
protected:
	in_audiostream audio;
};