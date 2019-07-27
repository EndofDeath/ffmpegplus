#include "stdafx.h"
#include "videoFilterTrans.h"

videoFilterTrans::videoFilterTrans(in_videostream & invideo, out_videostream & outvideo)
	:avtransformer(invideo.getcodec(), invideo.getframe(), outvideo.getcodec(), outvideo.getframe())
{}

int videoFilterTrans::init(const char * descr)
{
	return filter.init(srccodec, dstcodec, descr);
}

int videoFilterTrans::transform()
{
	if (filter.add(srcframe) < 0)
		return -1;
	if (filter.get(dstframe) < 0)
		return -1;
	updatepts();
	return 0;
}
