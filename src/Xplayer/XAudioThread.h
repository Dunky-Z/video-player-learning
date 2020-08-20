#pragma once
#include <QThread>
#include <mutex>
#include <list>

class XDecode;
class AVPacket;
class XResample;
class XAudioPlay;
struct AVCodecParameters;

class XAudioThread:public QThread
{
public:
	//��ǰ��Ƶ���ŵ�pts
	long long pts = 0;

	//�򿪣����ܳɹ��������
	virtual bool Open(AVCodecParameters *para,int sampleRate,int channels);
	virtual void Push(AVPacket *pkt);
	void run();
	XAudioThread();
	virtual ~XAudioThread();
	
	//������
	int maxList = 100;
	bool isExit = false;
protected:

	std::list <AVPacket *> packs;
	std::mutex mux;
	XDecode *decode = 0;
	XAudioPlay *ap = 0;
	XResample *res = 0;

};

