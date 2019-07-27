#pragma once
#include "avtransformer.h"
#include "io_audiostream.h"
class audiotransformer :public avtransformer
{
public:
	audiotransformer(AVCodecContext* incodec, AVFrame*inframe, AVCodecContext* outcodec, AVFrame* outframe);
	audiotransformer(in_audiostream& srcaudio, out_audiostream& dstaudio);
	~audiotransformer();
	int transform();
	bool enough();
private:
	SwrContext *swr{};
	AVFrame* newframe{};
	AVFrame* newframe2{};
	AVAudioFifo* fifo{};
	int des_nbsamples{};
};