#include "stdafx.h"
#include "avstream_helper.h"

AVFrame *alloc_picture(enum AVPixelFormat pix_fmt, int width, int height)
{
	AVFrame * picture = av_frame_alloc();
	if (!picture)
		return nullptr;

	picture->format = pix_fmt;
	picture->width = width;
	picture->height = height;
	// allocate the buffers for the frame data
	int ret = av_frame_get_buffer(picture, 32);
	if (ret < 0) //Could not allocate frame data
	{
		av_frame_free(&picture);
		return nullptr;
	}
	return picture;
}

AVFrame *alloc_audio_frame(enum AVSampleFormat sample_fmt,
	uint64_t channel_layout, int chanels,
	int sample_rate, int nb_samples)
{
	AVFrame *frame = av_frame_alloc();
	if (!frame) {
		return nullptr;
	}
	frame->format = sample_fmt;
	frame->channel_layout = channel_layout;
	frame->channels = chanels;
	frame->sample_rate = sample_rate;
	frame->nb_samples = nb_samples;
	if (nb_samples) {
		if (av_frame_get_buffer(frame, 0) < 0) {
			return nullptr;
		}
	}
	return frame;
}

AVStream * find_media(AVFormatContext * fmt_ctx, AVMediaType type)
{
	for (unsigned i = 0; i < fmt_ctx->nb_streams; i++)
	{
		if (fmt_ctx->streams[i]->codecpar->codec_type == type)
		{
			return fmt_ctx->streams[i];
		}
	}
	return nullptr;
}
