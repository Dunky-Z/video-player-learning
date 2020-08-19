#pragma once
#include <mutex>

struct AVFrame;
struct SwrContext;
struct AVCodecParameters;

class XResample
{
public:
	//����������������һ�³��˲�����ʽ�����ΪS16
	virtual bool Open(AVCodecParameters *para);
	virtual void Close();
	//�����ز������С,���ܳɹ�����ͷ�indata�ռ�
	virtual int Resample(AVFrame *indata, unsigned char *data);
	//AV_SAMPLE_FMT_S16
	int outFormat = 1;
	XResample();
	~XResample();

protected:
	std::mutex mux;
	SwrContext *actx = 0;
};

