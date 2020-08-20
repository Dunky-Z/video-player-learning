#pragma once
#include <mutex>
#include <QThread>

#include "IVideoCall.h"
#include "XDecodeThread.h"

class XDemux;
class XVideoThread;
class XAudioThread;
class XDemuxThread :public XDecodeThread
{
public:
	//创建对象并打开
	virtual bool Open(const char *url, IVideoCall *call);

	//启动所有线程
	virtual void Start();

	void run();
	XDemuxThread();
	virtual ~XDemuxThread();
	bool isExit = false;

protected:
	std::mutex mux;
	XDemux *demux = 0;
	XVideoThread *vt = 0;
	XAudioThread *at = 0;
};
