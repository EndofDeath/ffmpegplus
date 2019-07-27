#include "stdafx.h"
#include "avtransformer.h"

avtransformer::avtransformer(AVCodecContext* incodec, AVFrame*inframe, AVCodecContext* outcodec, AVFrame* outframe)
	:srccodec(incodec)
	,srcframe(inframe)
	,dstcodec(outcodec)
	,dstframe(outframe)
{
	if(dstcodec->codec_type == AVMEDIA_TYPE_VIDEO)
		duration = 1000000LL * dstcodec->time_base.num / dstcodec->time_base.den;
	else if(dstcodec->codec_type == AVMEDIA_TYPE_AUDIO)
		duration = 1000000LL * dstframe->linesize[0] / (dstcodec->sample_rate*av_get_bytes_per_sample(dstcodec->sample_fmt));
}

avtransformer::~avtransformer()
{
}

int avtransformer::compare_timestamps(avtransformer & other)
{
	if (next_pts < other.current_pts) return -1;
	if (current_pts > other.current_pts) return 1;
	return 0;
}

void avtransformer::updatepts()
{
	dstframe->pts = next_pts;
	next_pts += duration;
	current_pts = dstframe->pts;
	{
		AVRational  base{ 1,1000000 };//帧率
		//转换回去压缩器的时基
		dstframe->pts = av_rescale_q(dstframe->pts, base, dstcodec->time_base);
	}
}