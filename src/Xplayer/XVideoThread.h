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
	virtual void Push(AVPacket *pkt);
	void run();

	XVideoThread();
	virtual ~XVideoThread();
	//������
	int maxList = 100;
	bool isExit = false;
	//ͬ��ʱ�䣬���ⲿ����
	long long synpts = 0;
protected:
	std::mutex vmux;
	IVideoCall *call = 0;


};

