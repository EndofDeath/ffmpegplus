#pragma once

class avformat
{
public:
	avformat(){}
	virtual ~avformat(){}
	virtual int create(const char* filename) = 0;
	operator AVFormatContext*()
	{
		return format;
	}
	AVFormatContext* operator->()
	{
		return format;
	}
protected:
	AVFormatContext* format{};
};

class in_avformat :public avformat
{
public:
	in_avformat();
	~in_avformat();
	int create(const char* filename);
	int createfromdevice(const char*short_name, const char* url);
	//@return 1 if get video, or 2 if get audio,< 0 on error or eof
	int read(AVPacket* outpkt);
private:
	void init();
	int videoindex;
	int audioindex;
};

class out_avformat:public avformat
{
public:
	out_avformat();
	~out_avformat();
	int create(const char* filename);
	int createfromrtmp(const char* rtmp);
	int open();
	int write(AVPacket* inpkt);
	void close();
private:
	const char* url{};
};
