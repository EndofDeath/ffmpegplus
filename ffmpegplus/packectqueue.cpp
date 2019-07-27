#include "stdafx.h"
#include "packectqueue.h"


packectqueue::packectqueue(unsigned max)
	:packets(max)
{
}

packectqueue::~packectqueue()
{
	packet free;
	while (!packets.empty())
	{
		packets.pop(free);
		delete free.pkt;
	}

}

int packectqueue::push(const avpacket & pkt, int64_t pts)
{
	int ret = -1;
	{
		std::unique_lock<std::mutex> lock(mutex);
		wait.wait(lock, [this] {return exit || !packets.full(); });
		if (!exit) {
			packet temp;
			temp.pkt = new avpacket(pkt);
			temp.pts = pts;
			packets.push(temp);
			ret = 0;
		}
	}
	cond.notify_one();
	return ret;
}

int64_t packectqueue::pop(avpacket & pkt)
{
	int64_t ret = -1;
	{
		std::unique_lock<std::mutex> lock(mutex);
		cond.wait(lock, [this] {return exit || !packets.empty(); });
		if (!exit) {
			packet temp;
			packets.pop(temp);
			ret = temp.pts;
			pkt = *temp.pkt;
			delete temp.pkt;
		}
	}
	wait.notify_one();
	return ret;
}

bool packectqueue::empty()
{
	std::lock_guard<std::mutex> lock(mutex);
	return packets.empty();
}

void packectqueue::stop()
{
	{
		std::lock_guard<std::mutex> lock(mutex);
		exit = true;
	}
	wait.notify_all();
	cond.notify_all();
}