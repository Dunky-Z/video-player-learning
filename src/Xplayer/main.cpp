#include <QThread>
#include <QtWidgets/QApplication>


#include "XDemux.h"
#include "Xplayer.h"
#include "XDecode.h"


class TestThread :public QThread
{
public:
	void init()
	{
		//香港卫视
		//char *url = "rtmp://live.hkstv.hk.lxdns.com/live/hks";
		char *url = "D:/ITabc/ITabc/video-player/video/friends.mp4";
		cout << "demux.Open = " << demux.Open(url);
		demux.Read();
		demux.Clear();
		demux.Close();
		cout << "demux.Open = " << demux.Open(url);
		cout << "CopyVPara = " << demux.CopyVPara() << endl;
		cout << "CopyAPara = " << demux.CopyAPara() << endl;
		//cout << "seek=" << demux.Seek(0.5) << endl;

		cout << "vdecode.Open() = " << vdecode.Open(demux.CopyVPara()) << endl;
		//vdecode.Clear();
		//vdecode.Close();
		cout << "adecode.Open() = " << adecode.Open(demux.CopyAPara()) << endl;

	}
	void run()
	{
		for (;;)
		{
			AVPacket *pkt = demux.Read();
			if (demux.IsAudio(pkt))
			{
				adecode.Send(pkt);
				AVFrame *frame = adecode.Recv();
				cout << "Audio:" << frame << endl;
			}
			else
			{
				vdecode.Send(pkt);
				AVFrame *frame = vdecode.Recv();
				video->Repaint(frame);
				msleep(40);
				//cout << "Video:" << frame << endl;
			}
			if (!pkt)break;
		}

	}
	//测试XDemux
	XDemux demux;
	//解码测试
	XDecode vdecode;

	XDecode adecode;
	XVideoWidget *video;
private:

};

int main(int argc, char *argv[])
{

	TestThread tt;
	tt.init();

	QApplication a(argc, argv);
	Xplayer w;
	w.show();

	//初始化gl窗口
	w.ui.video->Init(tt.demux.width, tt.demux.height);
	tt.video = w.ui.video;
	tt.start();


	return a.exec();
}
