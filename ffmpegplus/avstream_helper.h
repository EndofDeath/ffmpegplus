#pragma once

AVFrame *alloc_picture(enum AVPixelFormat pix_fmt, int width, int height);

AVFrame *alloc_audio_frame(enum AVSampleFormat sample_fmt,
	uint64_t channel_layout, int chanels,
	int sample_rate, int nb_samples);

AVStream* find_media(AVFormatContext *fmt_ctx, AVMediaType type);