#pragma once
#include "avtransformer.h"
#include "io_videostream.h"

class videotransformer :public avtransformer
{
public:
	videotransformer(AVCodecContext* incodec, AVFrame*inframe, AVCodecContext* outcodec, AVFrame* outframe);
	videotransformer(in_videostream& srcvideo, out_videostream& dstvideo);
	~videotransformer();
	int transform();
private:
	SwsContext *convert{};
};