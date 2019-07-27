#pragma once
#include "io_avformat.h"
#include "io_videostream.h"
#include "io_audiostream.h"
#include "in_avsource.h"

class in_avfile:public in_videosource,public in_audiosource
{
public:
	in_avfile();
	~in_avfile();
	//@return 1 if video only, or 2 if audio only, or 3 if video and audio, < 0 on error
	int create(const char* short_name, const char* url);
	//@return 1 if video only, or 2 if audio only, or 3 if video and audio, < 0 on error
	int create(const char* filename);
	//@return 1 if get video and decode success, or 2 if get audio and decode success, < 0 on error or eof
	int read();
private:
	int init_avstream();
};