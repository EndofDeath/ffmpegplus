#include "stdafx.h"
#include "avpacket.h"

avpacket::avpacket()
{
	av_init_packet(&pkt);
}

avpacket::~avpacket()
{
	av_packet_unref(&pkt);
}

avpacket::avpacket(const avpacket & other)
{
	av_init_packet(&pkt);
	av_packet_ref(&pkt, &other.pkt);
}

avpacket & avpacket::operator=(const avpacket & other)
{
	if (this != &other)
	{
		av_packet_ref(&pkt, &other.pkt);
	}
	return *this;
}

int avpacket::receive(AVCodecContext* encodec, AVFrame* inputframe)
{
	if (avcodec_send_frame(encodec, inputframe) < 0)
		return -1;
	return avcodec_receive_packet(encodec, &pkt);
}

int avpacket::sendto(AVCodecContext * decodec, AVFrame * outputframe)
{
	if (avcodec_send_packet(decodec, &pkt) < 0)
		return -1;
	return avcodec_receive_frame(decodec, outputframe);
}