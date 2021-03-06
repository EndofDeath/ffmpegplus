#include "stdafx.h"

int main()
{
	avdevice_register_all();
	const char* filename = "record1.aac";
	out_avfile file;
	if (file.create(filename) < 0)
		return -1;
	if (file.addaudio() < 0)
		return -1;
	if (file.open() < 0)
		return -1;

	in_audiosource input;
	if (input.create("dshow", "audio=virtual-audio-capturer") < 0)
		return -1;

	audiotransformer adapter(input.getaudio(), file.getaudio());
	int64_t needtime = 10000000;
	while (needtime>= adapter.getcurrentpts()) {
		if (adapter.enough())
		{
			file.writeaudio();
			continue;
		}
		if (input.read() >= 0)
		{
			if (adapter.transform() >= 0)
				file.writeaudio();
		}
	}
	file.close();
	return 0;
}