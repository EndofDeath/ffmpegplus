
#ifndef STDAFX_H
#define STDAFX_H

#include "..\ffmpeg.h"
#include "..\ffmpegplus.h"
#ifdef _DEBUG
#pragma comment(lib,"..\\lib\\Debug\\ffmpegplus.lib")
#else
#pragma comment(lib,"..\\lib\\Release\\ffmpegplus.lib")
#endif

#endif //STDAFX_H