#pragma once

class avstream
{
public:
	avstream();
	virtual ~avstream();
	avstream(const avstream&) = delete;
	avstream& operator=(const avstream&) = delete;
	AVCodecContext * getcodec()
	{
		return codec;
	}
	AVFrame * getframe()
	{
		return frame;
	}
	operator AVStream*()
	{
		return st;
	}
	AVStream* operator->()
	{
		return st;
	}
	virtual int create(AVFormatContext * format) = 0;
protected:
	AVStream* st{};
	AVCodecContext *codec{};
	AVFrame *frame{};
};

class in_avstream:public avstream
{
public:
	in_avstream();
	~in_avstream();
	int writepacket(AVPacket* inpkt);
};

class out_avstream :public avstream
{
public:
	out_avstream();
	~out_avstream();
	int readpacket(AVPacket* outpkt);
};