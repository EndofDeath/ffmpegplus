#include "stdafx.h"
#include "videoFrameFilter.h"

const char *drawtext = "drawtext = fontfile = arial.ttf:fontcolor = red : fontsize = 30 : text = 'this is a test text!!'";
const char *lutyuv = "lutyuv='u=128:v=128'";//去色调
const char *boxblur = "boxblur";//模糊
const char *hflip = "hflip";//水平翻转
const char *drawbox = "drawbox=x=100:y=100:w=100:h=100:color=pink@0.5";
const char *logo = "movie=logo.png [watermark]; [in][watermark] overlay=100:100 [out]";

int main()
{
	const char *in_filename = "in.mp4";
	const char *out_filename = "filter_out.mov";
	in_avfile infile;
	if (infile.create(in_filename) < 0)
		return -1;
	out_avfile outfile;
	AVStream* st = infile.getvideo();
	if (outfile.create(out_filename) < 0)
		return -1;
	if (outfile.addvideo(st->codecpar->width, st->codecpar->height, st->avg_frame_rate.num / st->avg_frame_rate.den, st->codecpar->bit_rate) < 0)
		return -1;
	if (outfile.addaudio() < 0)
		return -1;
	if (outfile.open() < 0)
		return -1;
	videotransformer videoc(infile.getvideo(), outfile.getvideo());
	audiotransformer audioc(infile.getaudio(), outfile.getaudio());
	videoFrameFilter filter(infile.getvideo());
	if (filter.init(drawtext) < 0)
		return -1;
	int ret = 0;
	int frame_index = 0;
	while (1)
	{
		if (audioc.enough())
		{
			outfile.writeaudio();
			continue;
		}
		ret = infile.read();//read a frame
		if (ret < 0) break;
		if (1 == ret)
		{
			filter.filt();
			videoc.transform();
			outfile.writevideo();
			frame_index++;
		}
		else if (2 == ret)
		{
			if (audioc.transform() >= 0)
				outfile.writeaudio();
		}
	}
	outfile.close();
	if (ret < 0 && ret != AVERROR_EOF)
		return -1;
	return 0;
}