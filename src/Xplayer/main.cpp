#include <QtWidgets/QApplication>


#include "XDemux.h"
#include "Xplayer.h"
#include "XDecode.h"

int main(int argc, char *argv[])
{
	//≤‚ ‘XDemux
	XDemux demux;
	//œ„∏€Œ¿ ”
	//char *url = "rtmp://live.hkstv.hk.lxdns.com/live/hks";
	//url = "v1080.mp4";
	char *url = "D:/ITabc/ITabc/video-player/video/friends.mp4";
	cout << "demux.Open = " << demux.Open(url);
	demux.Read();
	demux.Clear();
	demux.Close();
	cout << "demux.Open = " << demux.Open(url);
	cout << "CopyVPara = " << demux.CopyVPara() << endl;
	cout << "CopyAPara = " << demux.CopyAPara() << endl;
	cout << "seek=" << demux.Seek(0.5) << endl;

	XDecode vdecode;
	cout << "vdecode.Open() = " << vdecode.Open(demux.CopyVPara()) << endl;
	vdecode.Clear();
	vdecode.Close();
	XDecode adecode;
	cout << "adecode.Open() = " << adecode.Open(demux.CopyAPara()) << endl;

	for (;;)
	{
		AVPacket *pkt = demux.Read();
		if (!pkt)break;
	}


	QApplication a(argc, argv);
	Xplayer w;
	w.show();
	return a.exec();
}
