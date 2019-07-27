#include "stdafx.h"
#include "io_avstream.h"

avstream::avstream()
{
}

avstream::~avstream()
{
	if (frame)
		av_frame_free(&frame);
	if (codec)
		avcodec_free_context(&codec);
}

in_avstream::in_avstream()
{
}

in_avstream::~in_avstream()
{
}

int in_avstream::writepacket(AVPacket * inpkt)
{
	if (avcodec_send_packet(codec, inpkt) < 0)
		return -1;
	return avcodec_receive_frame(codec, frame);
}

out_avstream::out_avstream()
{
}

out_avstream::~out_avstream()
{
}

int out_avstream::readpacket(AVPacket * outpkt)
{
	if (avcodec_send_frame(codec, frame) < 0)
		return -1;
	if (avcodec_receive_packet(codec, outpkt) < 0)
		return -1;
	av_packet_rescale_ts(outpkt, codec->time_base, st->time_base);
	outpkt->stream_index = st->id;
	return 0;
}