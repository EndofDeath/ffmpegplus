#pragma once
class videoFrameFilter
{
public:
	videoFrameFilter(in_videostream& video);
	int init(const char* descr);
	int filt();
private:
	videofilter filter;
	AVCodecContext* codec;
	AVFrame* frame;
};