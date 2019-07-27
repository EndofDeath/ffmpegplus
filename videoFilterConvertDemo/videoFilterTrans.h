#pragma once
class videoFilterTrans :
	public avtransformer
{
public:
	videoFilterTrans(in_videostream& invideo, out_videostream& outvideo);

	int init(const char* descr);
	int transform();
private:
	videofilter filter;
};