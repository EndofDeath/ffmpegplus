#pragma once
class avpacket
{
public:
	avpacket();
	~avpacket();
	avpacket(const avpacket& other);
	avpacket& operator=(const avpacket& other);

	int receive(AVCodecContext* encodec, AVFrame* inputframe);
	int sendto(AVCodecContext* decodec, AVFrame* outputframe);
	operator AVPacket*()
	{
		return &pkt;
	}
	AVPacket* operator->()
	{
		return &pkt;
	}
private:
	AVPacket pkt = { 0 };
};