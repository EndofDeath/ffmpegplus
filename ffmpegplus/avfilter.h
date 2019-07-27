#pragma once

class avfilter
{
public:
	avfilter();
	virtual ~avfilter();
	virtual int init(AVCodecContext* decodec, AVCodecContext * encodec, const char *filters_descr) = 0;
	int add(AVFrame* frame);
	int get(AVFrame* frame);
protected:
	AVFilterContext *buffersink_ctx;
	AVFilterContext *buffersrc_ctx;
	AVFilterGraph *filter_graph;
};

class videofilter:public avfilter
{
public:
	videofilter();
	~videofilter();
	int init(AVCodecContext* decodec, AVCodecContext * encodec, const char *filters_descr);
};

class audiofilter:public avfilter
{
public:
	audiofilter();
	~audiofilter();
	int init(AVCodecContext *decodec, AVCodecContext *encodec, const char *filters_descr);
};