
#include <mutex>
#include <QAudioFormat>
#include <QAudioOutput>

#include "XAudioPlay.h"

class CXAudioPlay :public XAudioPlay
{
public:
	QAudioOutput *output = NULL;
	QIODevice *io = NULL;
	std::mutex mux;
	virtual void Close()
	{
		//�򿪺͹رտ��ܲ���һ���߳������
		mux.lock();
		if (io)
		{
			io->close();
			io = NULL;
		}
		if (output)
		{
			output->stop();
			delete output;
			output = 0;
		}
		mux.unlock();
	}
	virtual bool Open()
	{
		Close();
		QAudioFormat fmt;
		fmt.setSampleRate(sampleRate);
		fmt.setSampleSize(sampleSize);
		fmt.setChannelCount(channels);
		fmt.setCodec("audio/pcm");
		fmt.setByteOrder(QAudioFormat::LittleEndian);
		fmt.setSampleType(QAudioFormat::UnSignedInt);
		mux.lock();
		output = new QAudioOutput(fmt);
		io = output->start(); //��ʼ����
		mux.unlock();
		if (io)
			return true;
		return false;
	}
};
XAudioPlay *XAudioPlay::Get()
{
	static CXAudioPlay play;
	return &play;
}

XAudioPlay::XAudioPlay()
{
}


XAudioPlay::~XAudioPlay()
{
}