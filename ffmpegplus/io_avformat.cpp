#include "stdafx.h"
#include "io_avformat.h"

in_avformat::in_avformat()
	:videoindex(-1)
	,audioindex(-1)
{
}

in_avformat::~in_avformat()
{
	avformat_close_input(&format);
}

int in_avformat::create(const char * filename)
{
	format = avformat_alloc_context();
	if (avformat_open_input(&format, filename, 0, 0) < 0)
	{
		//Could not open input file
		return -1;
	}
	if (avformat_find_stream_info(format, 0) < 0)
	{
		return -1;
	}
	init();
	return 0;
}

int in_avformat::createfromdevice(const char * short_name, const char * url)
{
	format = avformat_alloc_context();
	AVInputFormat *ifmt = av_find_input_format(short_name);//设备类型
	AVDictionary* options = NULL;
	av_dict_set(&options, "framerate", "15", 0);
	if (avformat_open_input(&format, url, ifmt, &options) != 0) {
		//open input device fail
		return -1;
	}
	av_dict_free(&options);
	if (avformat_find_stream_info(format, NULL) < 0) {
		//avformat_find_stream_info faill
		return -1;
	}
	init();
	return 0;
}

int in_avformat::read(AVPacket * outpkt)
{
	int ret = av_read_frame(format, outpkt);
	if (ret < 0) return ret;
	if (outpkt->stream_index == videoindex)
		return 1;
	if (outpkt->stream_index == audioindex)
		return 2;
	return 0;
}

void in_avformat::init()
{
	for (unsigned i = 0; i < format->nb_streams; i++)
	{
		if (format->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			videoindex = i;
		}
		else if (format->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
		{
			audioindex = i;
		}
	}
}

out_avformat::out_avformat()
{
}

out_avformat::~out_avformat()
{
	if (format)
		avformat_close_input(&format);
}

int out_avformat::create(const char* filename)
{
	url = filename;
	avformat_alloc_output_context2(&format, nullptr, nullptr, filename);
	if (nullptr == format)return -1;
	return 0;
}

int out_avformat::createfromrtmp(const char * rtmp)
{
	url = rtmp;
	avformat_alloc_output_context2(&format, nullptr, "flv", rtmp);
	if (nullptr == format)return -1;
	return 0;
}

int out_avformat::open()
{
	//打开写头信息
	if (!(format->oformat->flags&AVFMT_NOFILE)) {
		if (avio_open(&format->pb, url, AVIO_FLAG_WRITE) < 0) {
			//avio_open fail
			return -1;
		}
	}
	int ret = avformat_write_header(format, nullptr);
	if (ret < 0) {
		//open output file error
		return -1;
	}
	return 0;
}

int out_avformat::write(AVPacket * inpkt)
{
	return av_interleaved_write_frame(format, inpkt);
}

void out_avformat::close()
{
	av_write_trailer(format);
	if (!(format->oformat->flags&AVFMT_NOFILE)) {
		avio_closep(&format->pb);
	}
}