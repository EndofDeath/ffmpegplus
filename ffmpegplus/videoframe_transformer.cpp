#include "stdafx.h"
#include "videoframe_transformer.h"

videotransformer::videotransformer(AVCodecContext * incodec, AVFrame * inframe, AVCodecContext * outcodec, AVFrame * outframe)
	:avtransformer(incodec,inframe,outcodec,outframe)
{
	convert = sws_getContext(srccodec->width, srccodec->height, srccodec->pix_fmt,
		dstcodec->width, dstcodec->height, dstcodec->pix_fmt, SWS_BICUBIC, NULL, NULL, NULL);
}

videotransformer::videotransformer(in_videostream& srcvideo, out_videostream& dstvideo)
	:videotransformer(srcvideo.getcodec(),srcvideo.getframe(), dstvideo.getcodec(),dstvideo.getframe())
{
}

videotransformer::~videotransformer()
{
}

int videotransformer::transform()
{
	sws_scale(convert, srcframe->data, srcframe->linesize, 0, srcframe->height, dstframe->data, dstframe->linesize);
	updatepts();
	return 0;
}