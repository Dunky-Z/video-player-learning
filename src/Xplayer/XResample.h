#pragma once
#include <mutex>

struct AVFrame;
struct SwrContext;
struct AVCodecParameters;

class XResample
{
public:
	//输出参数和输入参数一致除了采样格式，输出为S16
	virtual bool Open(AVCodecParameters *para);
	virtual void Close();
	//返回重采样后大小,不管成功与否都释放indata空间
	virtual int Resample(AVFrame *indata, unsigned char *data);
	//AV_SAMPLE_FMT_S16
	int outFormat = 1;
	XResample();
	~XResample();

protected:
	std::mutex mux;
	SwrContext *actx = 0;
};

