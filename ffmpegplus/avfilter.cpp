#include "stdafx.h"
#include "avfilter.h"


avfilter::avfilter()
{
}

avfilter::~avfilter()
{
	avfilter_graph_free(&filter_graph);
}

int avfilter::add(AVFrame * frame)
{
	return av_buffersrc_add_frame(buffersrc_ctx, frame);
}

int avfilter::get(AVFrame * frame)
{
	av_frame_unref(frame);
	return av_buffersink_get_frame(buffersink_ctx, frame);
}

videofilter::videofilter()
{
}

videofilter::~videofilter()
{
}

int videofilter::init(AVCodecContext * decodec, AVCodecContext * encodec, const char * filters_descr)
{
	if (decodec->codec_type != AVMEDIA_TYPE_VIDEO)
		return AVERROR_UNKNOWN;
	char args[512];
	int ret = 0;
	const AVFilter *buffersrc = avfilter_get_by_name("buffer");
	const AVFilter *buffersink = avfilter_get_by_name("buffersink");
	AVFilterInOut *outputs = avfilter_inout_alloc();
	AVFilterInOut *inputs = avfilter_inout_alloc();
	enum AVPixelFormat pix_fmts[] = { encodec->pix_fmt, AV_PIX_FMT_NONE };
	do {
		filter_graph = avfilter_graph_alloc();
		if (!outputs || !inputs || !filter_graph) {
			ret = AVERROR(ENOMEM);
			break;
		}
		/* buffer video source: the decoded frames from the decoder will be inserted here. */
		snprintf(args, sizeof(args), "video_size=%dx%d:pix_fmt=%d:time_base=%d/%d:pixel_aspect=%d/%d", decodec->width, decodec->height, decodec->pix_fmt, decodec->framerate.den, decodec->framerate.num, decodec->sample_aspect_ratio.num, decodec->sample_aspect_ratio.den);
		ret = avfilter_graph_create_filter(&buffersrc_ctx, buffersrc, "in", args, NULL, filter_graph);
		if (ret < 0)
		{
			//Cannot create buffer source      
			break;
		}
		/* buffer video sink: to terminate the filter chain. */
		AVBufferSinkParams * buffersink_params = av_buffersink_params_alloc();
		buffersink_params->pixel_fmts = pix_fmts;
		ret = avfilter_graph_create_filter(&buffersink_ctx, buffersink, "out", NULL, buffersink_params, filter_graph);
		av_free(buffersink_params);
		if (ret < 0)
		{
			//Cannot create buffer sink   
			break;
		}
		/* Endpoints for the filter graph. */
		outputs->name = av_strdup("in");
		outputs->filter_ctx = buffersrc_ctx;
		outputs->pad_idx = 0;
		outputs->next = NULL;
		inputs->name = av_strdup("out");
		inputs->filter_ctx = buffersink_ctx;
		inputs->pad_idx = 0;
		inputs->next = NULL;
		if ((ret = avfilter_graph_parse_ptr(filter_graph, filters_descr, &inputs, &outputs, NULL)) < 0)
			break;
		if ((ret = avfilter_graph_config(filter_graph, NULL)) < 0)
			break;
	} while (0);
	avfilter_inout_free(&inputs);
	avfilter_inout_free(&outputs);
	return ret;
}

audiofilter::audiofilter()
{
}

audiofilter::~audiofilter()
{
}

int audiofilter::init(AVCodecContext * decodec, AVCodecContext *encodec, const char * filters_descr)
{
	if (decodec->codec_type != AVMEDIA_TYPE_AUDIO)
		return AVERROR_UNKNOWN;
	char args[512];
	int ret = 0;
	const AVFilter *abuffersrc = avfilter_get_by_name("abuffer");
	const AVFilter *abuffersink = avfilter_get_by_name("abuffersink");
	AVFilterInOut *outputs = avfilter_inout_alloc();
	AVFilterInOut *inputs = avfilter_inout_alloc();
	AVRational time_base = decodec->time_base;
	do
	{
		filter_graph = avfilter_graph_alloc();
		if (!outputs || !inputs || !filter_graph) {
			ret = AVERROR(ENOMEM);
			break;
		}
		/* buffer audio source: the decoded frames from the decoder will be inserted here. */
		if (!decodec->channel_layout)
			decodec->channel_layout = av_get_default_channel_layout(decodec->channels);
		snprintf(args, sizeof(args),
			"time_base=%d/%d:sample_rate=%d:sample_fmt=%s:channel_layout=0x%I64u",
			time_base.num, time_base.den, decodec->sample_rate,
			av_get_sample_fmt_name(decodec->sample_fmt), decodec->channel_layout);
		ret = avfilter_graph_create_filter(&buffersrc_ctx, abuffersrc, "in",
			args, NULL, filter_graph);
		if (ret < 0) {
			//Cannot create audio buffer source
			break;
		}
		/* buffer audio sink: to terminate the filter chain. */
		ret = avfilter_graph_create_filter(&buffersink_ctx, abuffersink, "out",
			NULL, NULL, filter_graph);
		if (ret < 0) {
			//Cannot create audio buffer sink
			break;
		}
		ret = av_opt_set_bin(buffersink_ctx, "sample_fmts",
			(uint8_t*)&encodec->sample_fmt, sizeof(encodec->sample_fmt),
			AV_OPT_SEARCH_CHILDREN);
		if (ret < 0) {
			//Cannot set output sample format
			break;
		}
		ret = av_opt_set_bin(buffersink_ctx, "channel_layouts",
			(uint8_t*)&encodec->channel_layout,
			sizeof(encodec->channel_layout), AV_OPT_SEARCH_CHILDREN);
		if (ret < 0) {
			//Cannot set output channel layout
			break;
		}
		ret = av_opt_set_bin(buffersink_ctx, "sample_rates",
			(uint8_t*)&encodec->sample_rate, sizeof(encodec->sample_rate),
			AV_OPT_SEARCH_CHILDREN);
		if (ret < 0) {
			//Cannot set output sample rate
			break;
		}
		outputs->name = av_strdup("in");
		outputs->filter_ctx = buffersrc_ctx;
		outputs->pad_idx = 0;
		outputs->next = NULL;
		inputs->name = av_strdup("out");
		inputs->filter_ctx = buffersink_ctx;
		inputs->pad_idx = 0;
		inputs->next = NULL;
		if ((ret = avfilter_graph_parse_ptr(filter_graph, filters_descr,
			&inputs, &outputs, NULL)) < 0)
			break;
		if ((ret = avfilter_graph_config(filter_graph, NULL)) < 0)
			break;
	} while (0);
	avfilter_inout_free(&inputs);
	avfilter_inout_free(&outputs);
	return ret;
}