#pragma once
#include <mutex>
#include <iostream>

extern "C"
{
#include<libavcodec/avcodec.h>
}

using std::cout;
using std::endl;

struct AVCodecParameters;
struct AVCodecContext;
struct AVFrame;

class XDecode
{
public:
	//�򿪽�����,���ܳɹ�����ͷ�para�ռ�
	virtual bool Open(AVCodecParameters *para);

	//���͵������̣߳����ܳɹ�����ͷ�pkt�ռ䣨�����ý�����ݣ�
	virtual bool Send(AVPacket *pkt);

	//��ȡ�������ݣ�һ��send������Ҫ���Recv����ȡ�����е�����Send NULL��Recv���
	//ÿ�θ���һ�ݣ��ɵ������ͷ� av_frame_free
	virtual AVFrame* Recv();

	virtual void Close();
	virtual void Clear();

	XDecode();
	virtual ~XDecode();
protected:
	AVCodecContext *codec = 0;
	std::mutex mux;
};

