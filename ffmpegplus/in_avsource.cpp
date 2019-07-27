#include "stdafx.h"
#include "in_avsource.h"
#include "avpacket.h"

static int readframe(in_avformat& src, in_avstream& dst)
{
	avpacket pkt;
	int ret = src.read(pkt);
	if (ret >= 0 && dst.writepacket(pkt) < 0)
		ret = -1;
	return ret;
}

in_avsource::in_avsource()
{
}

in_avsource::~in_avsource()
{
}

int in_videosource::create(const char * short_name, const char * url)
{
	if (format.createfromdevice(short_name, url) < 0)
		return -1;
	return video.create(format);
}

int in_videosource::read()
{
	return readframe(format, video);
}

int in_audiosource::create(const char * short_name, const char * url)
{
	if (format.createfromdevice(short_name, url) < 0)
		return -1;
	return audio.create(format);
}

int in_audiosource::read()
{
	return readframe(format, audio);
}