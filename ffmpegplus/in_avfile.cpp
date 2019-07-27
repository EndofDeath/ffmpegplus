#include "stdafx.h"
#include "in_avfile.h"
#include "avpacket.h"

in_avfile::in_avfile()
{
}

in_avfile::~in_avfile()
{
}

int in_avfile::create(const char * short_name, const char * url)
{
	if (format.createfromdevice(short_name, url) < 0)
		return -1;
	return init_avstream();
}

int in_avfile::create(const char * filename)
{
	if (format.create(filename) < 0)
		return -1;
	return init_avstream();
}

int in_avfile::read()
{
	avpacket pkt;
	int ret = format.read(pkt);
	if (1 == ret && video.writepacket(pkt) < 0)
		ret = 0;
	else if (2 == ret && audio.writepacket(pkt) < 0)
		ret = 0;
	return ret;
}

int in_avfile::init_avstream()
{
	int ret = 3;
	if (video.create(format) < 0)
		ret -= 1;
	if (audio.create(format) < 0)
		ret -= 2;
	return 0 == ret ? -1 : ret;
}