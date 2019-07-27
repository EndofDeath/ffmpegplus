#include "stdafx.h"
#include "audioframe_transformer.h"
#include "avstream_helper.h"

audiotransformer::audiotransformer(AVCodecContext * incodec, AVFrame * inframe, AVCodecContext * outcodec, AVFrame * outframe)
	:avtransformer(incodec, inframe, outcodec, outframe)
{
	swr = swr_alloc();
	av_opt_set_int(swr, "in_channel_count", srccodec->channels, 0);
	av_opt_set_int(swr, "in_channel_layout", srccodec->channel_layout, 0);
	av_opt_set_int(swr, "in_sample_rate", srccodec->sample_rate, 0);
	av_opt_set_sample_fmt(swr, "in_sample_fmt", srccodec->sample_fmt, 0);

	uint64_t desc_channel_layout = dstcodec->channel_layout;
	int desc_channel_count = dstcodec->channels;
	int desc_sample_rate = dstcodec->sample_rate;
	AVSampleFormat desc_fmt = dstcodec->sample_fmt;

	av_opt_set_int(swr, "out_channel_count", desc_channel_count, 0);
	av_opt_set_int(swr, "out_channel_layout", desc_channel_layout, 0);
	av_opt_set_int(swr, "out_sample_rate", desc_sample_rate, 0);
	av_opt_set_sample_fmt(swr, "out_sample_fmt", desc_fmt, 0);

	newframe = alloc_audio_frame(desc_fmt, desc_channel_layout, desc_channel_count, desc_sample_rate, 0);
	newframe2 = alloc_audio_frame(desc_fmt, desc_channel_layout, desc_channel_count, desc_sample_rate, 0);
	fifo = av_audio_fifo_alloc(desc_fmt, desc_channel_count, dstframe->nb_samples * 10);
	des_nbsamples = dstframe->nb_samples;
}

audiotransformer::audiotransformer(in_audiostream& srcaudio, out_audiostream& dstaudio)
	:audiotransformer(srcaudio.getcodec(), srcaudio.getframe(), dstaudio.getcodec(), dstaudio.getframe())
{
}

audiotransformer::~audiotransformer()
{
	av_frame_free(&newframe);
	av_frame_free(&newframe2);
	av_audio_fifo_free(fifo);
}

int audiotransformer::transform()
{
	swr_convert_frame(swr, newframe, srcframe);
	av_audio_fifo_write(fifo, (void**)newframe->data, newframe->nb_samples);

	int64_t dealy = swr_get_delay(swr, dstcodec->sample_rate);
	if (dealy > 0) {
		swr_convert_frame(swr, newframe2, NULL);
		av_audio_fifo_write(fifo, (void**)newframe2->data, newframe2->nb_samples);
	}
	if (!enough())return -1;
	return 0;
}

bool audiotransformer::enough()
{
	int size = av_audio_fifo_size(fifo);
	if (size >= des_nbsamples) {//³¬¹ı
		size = des_nbsamples;
		av_audio_fifo_read(fifo, (void**)dstframe->data, size);
		updatepts();
		return true;
	}
	return false;
}