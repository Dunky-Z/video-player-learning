#pragma once
#include <list>
#include <mutex>
#include <QThread>

#include "XDecodeThread.h"


class XResample;
class XAudioPlay;
struct AVCodecParameters;

class XAudioThread:public XDecodeThread
{
public:
	//当前音频播放的pts
	long long pts = 0;

	//打开，不管成功与否都清理
	virtual bool Open(AVCodecParameters *para,int sampleRate,int channels);

	//停止线程，清理资源
	virtual void Close();
	virtual void Clear();
	void run();

	void SetPause(bool isPause);
	bool isPause = false;

	XAudioThread();
	virtual ~XAudioThread();

protected:
	std::mutex amux;
	XAudioPlay *ap = 0;
	XResample *res = 0;

};

