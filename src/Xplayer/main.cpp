#include <QThread>
#include <iostream>
#include <QtWidgets/QApplication>

#include "XDemux.h"
#include "Xplayer.h"
#include "XDecode.h"
#include "XResample.h"
#include "XAudioPlay.h"
#include "XDemuxThread.h"
#include "XAudioThread.h"
#include "XVideoThread.h"

using std::cout;
using std::endl;

class TestThread :public QThread
{
public:
	XAudioThread at;
	XVideoThread vt;
	void Init()
	{
		//香港卫视
		char *url1 = "D:/ITabc/ITabc/video-player/video/friends.mp4";
		cout << "demux.Open = " << demux.Open(url1);
		demux.Read();
		demux.Clear();
		demux.Close();
		char *url = "D:/ITabc/ITabc/video-player/video/friends.mp4";
		cout << "demux.Open = " << demux.Open(url);
		//cout << "CopyVPara = " << demux.CopyVPara() << endl;
		//cout << "CopyAPara = " << demux.CopyAPara() << endl;
		//cout << "seek=" << demux.Seek(0.95) << endl;

		/////////////////////////////
		//cout << "vdecode.Open() = " << vdecode.Open(demux.CopyVPara()) << endl;
		//vdecode.Clear();
		//vdecode.Close();
		//cout << "adecode.Open() = " << adecode.Open(demux.CopyAPara()) << endl;
		//cout << "resample.Open = " << resample.Open(demux.CopyAPara()) << endl;
		//XAudioPlay::Get()->channels = demux.channels;
		//XAudioPlay::Get()->sampleRate = demux.sampleRate;

		//cout << "XAudioPlay::Get()->Open() = " << XAudioPlay::Get()->Open()<<endl;

		cout << "at.Open = "<<at.Open(demux.CopyAPara(), demux.sampleRate, demux.channels);
		vt.Open(demux.CopyVPara(), video, demux.width, demux.height);
		at.start();
		vt.start();
	}
	unsigned char *pcm = new unsigned char[1024 * 1024];
	void run()
	{
		for (;;)
		{
			AVPacket *pkt = demux.Read();
			if (demux.IsAudio(pkt))
			{
				at.Push(pkt);
				/*adecode.Send(pkt);
				AVFrame *frame = adecode.Recv();
				int len = resample.Resample(frame, pcm);
				cout<<"Resample:"<<len<<" ";
				while (len > 0)
				{
					if (XAudioPlay::Get()->GetFree() >= len)
					{
						XAudioPlay::Get()->Write(pcm, len);
						break;
					}
					msleep(1);
				}*/
				//cout << "Audio:" << frame << endl;
			}
			else
			{
				vt.Push(pkt);
				//vdecode.Send(pkt);
				//AVFrame *frame = vdecode.Recv();
				//video->Repaint(frame);
				//msleep(40);
				//cout << "Video:" << frame << endl;
			}
			if (!pkt)
			{
				demux.Seek(0);
				//break;
			}
		}
	}
	///测试XDemux
	XDemux demux;
	///解码测试
	//XDecode vdecode;
	//XDecode adecode;
	//XResample resample;
	XVideoWidget *video = 0;

};

//v1080.mp4
int main(int argc, char *argv[])
{

	//初始化显示
	/*

	for (;;)
	{
		AVPacket *pkt = demux.Read();
		if (demux.IsAudio(pkt))
		{
			adecode.Send(pkt);
			AVFrame *frame = adecode.Recv();
			//cout << "Audio:" << frame << endl;
		}
		else
		{
			vdecode.Send(pkt);
			AVFrame *frame = vdecode.Recv();
			//cout << "Video:" << frame << endl;
		}
		if (!pkt)break;
	}*/
	//TestThread tt;
	QApplication a(argc, argv);
	Xplayer w;
	w.show();
	//XDemuxThread dt;
	//dt.Open("D:/ITabc/ITabc/video-player/video/friends.mp4",w.ui.video);
	//dt.Start();
	//初始化gl窗口
	//w.ui.video->Init(tt.demux.width, tt.demux.height);
	//tt.video = w.ui.video;
	//tt.Init();
	//tt.start();

	return a.exec();
}
