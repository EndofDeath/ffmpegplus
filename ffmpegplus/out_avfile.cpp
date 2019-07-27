#include "stdafx.h"
#include "out_avfile.h"
#include "avpacket.h"

out_avfile::out_avfile()
{
}

out_avfile::~out_avfile()
{
}

int out_avfile::create(const char*filename)
{
	if (format.create(filename) < 0)
		return -1;
	return 0;
}

int out_avfile::addvideo(int width, int height, int framerate, int64_t bitrate)
{
	video.init(width, height, framerate, bitrate, format->oformat->video_codec);
	if (video.create(format) < 0)
		return -1;
	return 0;
}

int out_avfile::addaudio(int64_t bitrate)
{
	audio.init(bitrate, format->oformat->audio_codec);
	if (audio.create(format) < 0)
		return -1;
	return 0;
}

int out_avfile::open()
{
	return format.open();
}

void out_avfile::close()
{
	return format.close();
}

static int writeframe(out_avstream& src, out_avformat& dst)
{
	avpacket pkt;
	if (src.readpacket(pkt) < 0)
		return -1;
	return dst.write(pkt);
}

int out_avfile::writevideo()
{
	return writeframe(video, format);
}

int out_avfile::writeaudio()
{
	return writeframe(audio, format);
}