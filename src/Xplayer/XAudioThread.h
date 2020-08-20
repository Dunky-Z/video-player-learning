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
	//��ǰ��Ƶ���ŵ�pts
	long long pts = 0;

	//�򿪣����ܳɹ��������
	virtual bool Open(AVCodecParameters *para,int sampleRate,int channels);

	//ֹͣ�̣߳�������Դ
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

