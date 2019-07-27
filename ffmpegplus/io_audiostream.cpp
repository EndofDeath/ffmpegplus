#include "stdafx.h"
#include "io_audiostream.h"
#include "avstream_helper.h"

in_audiostream::in_audiostream()
{
}

in_audiostream::~in_audiostream()
{
}

int in_audiostream::create(AVFormatContext * format)
{
	st = find_media(format, AVMEDIA_TYPE_AUDIO);
	if (nullptr==st)
	{
		//can't find audio stream
		return -1;
	}
	//查找解密器
	AVCodec *decodec = avcodec_find_decoder(st->codecpar->codec_id);
	if (decodec == NULL) {
		//codec not found
		return -1;
	}
	codec = avcodec_alloc_context3(decodec);
	avcodec_parameters_to_context(codec, st->codecpar);
	if (avcodec_open2(codec, decodec, NULL) < 0) {
		//codec not open
		return -1;
	}
	frame = alloc_audio_frame((AVSampleFormat)st->codecpar->format, st->codecpar->channel_layout, st->codecpar->channels, st->codecpar->sample_rate, st->codecpar->frame_size);
	codec->channel_layout = av_get_default_channel_layout(codec->channels);
	return 0;
}

out_audiostream::out_audiostream()
{
}

out_audiostream::~out_audiostream()
{
}

void out_audiostream::init(int64_t bitrate, AVCodecID id)
{
	bitrate_ = bitrate;
	id_ = id;
}

int out_audiostream::create(AVFormatContext * format)
{
	AVOutputFormat *fmt = format->oformat;
	fmt->audio_codec = id_;

	AVCodec *encodec = avcodec_find_encoder(id_);
	if (!encodec) {
		//not found encoder
		avcodec_get_name(id_);
		return -1;
	}

	st = avformat_new_stream(format, encodec);
	st->id = format->nb_streams - 1;

	codec = avcodec_alloc_context3(encodec);
	codec->codec_id = id_;
	codec->sample_fmt = encodec->sample_fmts ? encodec->sample_fmts[0] : AV_SAMPLE_FMT_FLT;
	codec->bit_rate = bitrate_;
	codec->sample_rate = 44100;
	if (encodec->supported_samplerates) {//查找是否支持
		codec->sample_rate = encodec->supported_samplerates[0];
		for (int i = 0; encodec->supported_samplerates[i]; i++)
		{
			if (encodec->supported_samplerates[i] == 44100) { codec->sample_rate = 44100; }
		}
	}
	codec->strict_std_compliance = FF_COMPLIANCE_EXPERIMENTAL;

	codec->channel_layout = AV_CH_LAYOUT_STEREO;
	if (encodec->channel_layouts) {//查找是否支持
		codec->channel_layout = encodec->channel_layouts[0];
		for (int i = 0; encodec->channel_layouts[i]; i++)
		{
			if (encodec->channel_layouts[i] == AV_CH_LAYOUT_STEREO) { codec->channel_layout = AV_CH_LAYOUT_STEREO; }
		}
	}
	codec->channels = av_get_channel_layout_nb_channels(codec->channel_layout);
	//播放器播放不出来
	if (fmt->flags&AVFMT_GLOBALHEADER) {
		codec->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
	}
	AVDictionary* opt{};
	int ret = avcodec_open2(codec, encodec, &opt);
	av_dict_free(&opt);
	if (ret < 0) {
		//not open audio codec
		return -1;
	}
	//add关联 把编辑器参数传递给输出流
	avcodec_parameters_from_context(st->codecpar, codec);
	int nb_samples = codec->codec->capabilities &AV_CODEC_CAP_VARIABLE_FRAME_SIZE ? 1024 : codec->frame_size;
	frame = alloc_audio_frame(codec->sample_fmt, codec->channel_layout, codec->channels, codec->sample_rate, nb_samples);
	return 0;
}