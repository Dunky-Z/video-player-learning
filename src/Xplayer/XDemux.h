#pragma once
#include <iostream>
#include <mutex>


struct AVPacket;
struct AVFormatContext;
struct AVCodecParameters;

using std::cout;
using std::endl;

//���װ��
class XDemux
{
public:

	//��ý�� ��������ý�� rtmp http rstp
	virtual bool Open(const char *url);

	//�ռ���Ҫ�������ͷ� ���ͷ�AVPacket����ռ䣬�����ݿռ� av_packet_free
	virtual AVPacket *Read();

	//��ȡ��Ƶ����  ���صĿռ���Ҫ����  avcodec_parameters_free
	virtual AVCodecParameters *CopyVPara();

	//��ȡ��Ƶ����  ���صĿռ���Ҫ���� avcodec_parameters_free
	virtual AVCodecParameters *CopyAPara();

	//seek λ�� pos 0.0 ~1.0
	virtual bool Seek(double pos);

	//��ն�ȡ����
	virtual void Clear();
	virtual void Close();

	XDemux();
	virtual ~XDemux();
	//ý����ʱ��������
	int totalMs = 0;
protected:
	std::mutex mux;
	//���װ������
	AVFormatContext *ic = NULL;
	//����Ƶ��������ȡʱ��������Ƶ
	int videoStream = 0;
	int audioStream = 1;
};

