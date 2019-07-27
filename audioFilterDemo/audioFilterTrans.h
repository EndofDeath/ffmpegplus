#pragma once
#include "E:\ffmpegplus\ffmpegplus\avtransformer.h"
class audioFilterTrans :
	public avtransformer
{
public:
	audioFilterTrans(in_audiostream& in, out_audiostream& out);
	~audioFilterTrans();
	int init();
	int transform();
private:
	audiofilter filter;
};

