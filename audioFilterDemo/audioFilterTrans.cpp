#include "stdafx.h"
#include "audioFilterTrans.h"

const char* get_channel_layout_name(uint64_t channel_layout)
{
	int index = 0;
	uint64_t layout = 0;
	const char* name = nullptr;
	while (av_get_standard_channel_layout(index++, &layout, &name) == 0)
	{
		if (channel_layout == layout)
			break;
	}
	return name;
}

audioFilterTrans::audioFilterTrans(in_audiostream & in, out_audiostream & out)
	:avtransformer(in.getcodec(), in.getframe(), out.getcodec(), out.getframe())
{
}

audioFilterTrans::~audioFilterTrans()
{
}

int audioFilterTrans::init()
{
	char descr[512]{};
	sprintf_s(descr, "aresample=%d,aformat=sample_fmts=%s:channel_layouts=%s",
		dstcodec->sample_rate,
		av_get_sample_fmt_name(dstcodec->sample_fmt),
		get_channel_layout_name(dstcodec->channel_layout));
	return filter.init(srccodec, dstcodec, descr);
}

int audioFilterTrans::transform()
{
	int ret = 0;
	if ((ret = filter.add(srcframe)) < 0)
		return ret;
	if ((ret = filter.get(dstframe)) < 0)
		return ret;
	updatepts();
	return 0;
}
