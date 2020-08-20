#pragma once
#include <list>
#include <mutex>
#include <QThread>
#include "IVideoCall.h"
#include "XDecodeThread.h"

class XDecode;
struct AVPacket;
struct AVCodecParameters;

class XVideoThread:public XDecodeThread
{
public:
	//�򿪣����ܳɹ��������
	virtual bool Open(AVCodecParameters *para,IVideoCall *call,int width,int height);
	//����pts��������յ��Ľ�������pts >= seekpts return true ������ʾ����
	virtual bool RepaintPts(AVPacket *pkt, long long seekpts);
	void run();

	XVideoThread();
	virtual ~XVideoThread();
	//ͬ��ʱ�䣬���ⲿ����
	long long synpts = 0;
	void SetPause(bool isPause);
	bool isPause = false;
protected:
	IVideoCall *call = 0;
	std::mutex vmux;
};

