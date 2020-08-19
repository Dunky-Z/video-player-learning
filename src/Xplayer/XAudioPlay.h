#pragma once
class XAudioPlay
{
public:
	int sampleRate = 44100;
	int sampleSize = 16;
	int channels = 2;

	//打开音频播放
	//纯虚函数，就不用再XAudio类中实现，只有它的子类用到了这个功能再在子类中去实现
	virtual bool Open() = 0;
	virtual void Close() = 0;

	//播放音频
	virtual bool Write(const unsigned char *data, int datasize) = 0;
	//判断是否有足够空间写入音频
	virtual int GetFree() = 0;
	static XAudioPlay *Get();
	XAudioPlay();
	virtual ~XAudioPlay();
};