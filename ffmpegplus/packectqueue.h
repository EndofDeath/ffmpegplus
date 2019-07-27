#pragma once
#include "avpacket.h"
#include "ringbuffer.h"
#include <mutex>

class packectqueue
{
public:
	packectqueue(unsigned max);
	~packectqueue();

	int push(const avpacket& pkt, int64_t pts);
	int64_t pop(avpacket& pkt);
	bool empty();
	void stop();
	bool campare_pts(packectqueue& other);
private:
	struct packet
	{
		avpacket* pkt;
		int64_t pts;
	};
	fixedringbuffer<packet> packets;
	std::mutex mutex;
	std::condition_variable cond;
	std::condition_variable wait;
	bool exit;
};

