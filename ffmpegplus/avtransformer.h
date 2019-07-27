#pragma once

class avtransformer
{
public:
	avtransformer(AVCodecContext* incodec, AVFrame*inframe, AVCodecContext* outcodec, AVFrame* outframe);
	~avtransformer();
	virtual int transform() = 0;
	int64_t getcurrentpts()
	{
		return current_pts;
	}
	int64_t getnextpts()
	{
		return next_pts;
	}
	int64_t getduration()
	{
		return duration;
	}
	//@return -1 if this's behind other, or 1 if this's ahead of other, or 0 this syncs with other
	int compare_timestamps(avtransformer& other);
	void updatepts();
protected:
	AVCodecContext* srccodec{};
	AVFrame* srcframe{};
	AVCodecContext* dstcodec{};
	AVFrame* dstframe{};
	int64_t current_pts{};
	int64_t next_pts{};
	int64_t duration{};
};