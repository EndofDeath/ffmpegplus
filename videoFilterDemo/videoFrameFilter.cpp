#include "stdafx.h"
#include "videoFrameFilter.h"

videoFrameFilter::videoFrameFilter(in_videostream & video)
	:codec(video.getcodec())
	, frame(video.getframe())
{
}

int videoFrameFilter::init(const char * descr)
{
	return filter.init(codec, codec, descr);
}

int videoFrameFilter::filt()
{
	if (filter.add(frame) < 0)
		return -1;
	return filter.get(frame);
}