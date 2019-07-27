工程ffmpegplus以C++类的方式封装了一些常用的ffmpeg结构.
各个demo工程给出了简单的桌面录像、录音、同步录像录音、音视频转换、视频中提取音频等例子。
其中桌面录像、录音需要先安装screen capture recorder。

当前使用的ffmpeg版本为4.0，下载地址https://ffmpeg.zeranoe.com/builds/win32/dev/
注意：下载后请先修改PropertySheet.props中的ffmpeg目录信息，再打开ffmpegplus.sln进行编译。
