#include "stdafx.h"
#include "io_videostream.h"
#include "avstream_helper.h"

in_videostream::in_videostream()
{
}

in_videostream::~in_videostream()
{
}

int in_videostream::create(AVFormatContext * format)
{
	st = find_media(format, AVMEDIA_TYPE_VIDEO);
	if (nullptr == st)
	{
		//can't find video stream
		return -1;
	}
	//查找解密器
	AVCodec *decoder = avcodec_find_decoder(st->codecpar->codec_id);
	if (decoder == nullptr) {
		//codec not found
		return -1;
	}
	codec = avcodec_alloc_context3(decoder);
	if (avcodec_parameters_to_context(codec, st->codecpar) < 0)
		return -1;
	codec->framerate = av_guess_frame_rate(format, st, nullptr);
	if (avcodec_open2(codec, decoder, nullptr) < 0) {
		//codec not open
		return -1;
	}
	frame = alloc_picture((AVPixelFormat)st->codecpar->format, st->codecpar->width, st->codecpar->height);
	return 0;
}

out_videostream::out_videostream()
{
}

out_videostream::~out_videostream()
{
}

void out_videostream::init(int width, int height, int framerate, int64_t bitrate, AVCodecID id)
{
	width_ = width;
	height_ = height;
	framerate_ = framerate;
	bitrate_ = bitrate;
	id_ = id;
}

int out_videostream::create(AVFormatContext * format)
{
	AVCodec *encoder = avcodec_find_encoder(id_);
	if (!encoder) {
		//not found encoder for id
		avcodec_get_name(id_);
		return -1;
	}
	st = avformat_new_stream(format, encoder);
	st->id = format->nb_streams - 1;

	codec = avcodec_alloc_context3(encoder);
	codec->codec_id = id_;
	codec->bit_rate = bitrate_;//码率
	codec->width = width_;
	codec->height = height_;
	codec->time_base = AVRational{ 1,framerate_ };//帧率
	codec->gop_size = 12;
	codec->max_b_frames = 0;
	codec->pix_fmt = *encoder->pix_fmts;
	codec->gop_size = 12;
	codec->max_b_frames = 0;
	//减少延迟
	av_opt_set(codec->priv_data, "preset", "superfast", 0);
	av_opt_set(codec->priv_data, "tune", "zerolatency", 0);

	if (format->oformat->flags&AVFMT_GLOBALHEADER) {
		codec->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
	}
	AVDictionary* opt{};
	int ret = avcodec_open2(codec, encoder, &opt);
	av_dict_free(&opt);
	if (ret < 0) {
		//not open video codec
		return -1;
	}
	//add关联 把编辑器参数传递给输出流
	avcodec_parameters_from_context(st->codecpar, codec);
	frame = alloc_picture(codec->pix_fmt, codec->width, codec->height);
	return 0;
}