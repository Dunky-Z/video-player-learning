// TestDemux.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>

extern "C"
{
#include "libavformat/avformat.h"
}

using std::cout;
using std::endl;

#pragma comment(lib,"avformat.lib")
#pragma comment(lib,"avutil.lib")

const char *path = "../../video/cloud.mp4";

int main()

{
	cout << "Test Demux FFmepeg" << endl;

	//初始化封装库
	av_register_all();

	//初始化网络库 （可以打开rtsp rtmp http 协议的流媒体视频）
	avformat_network_init();
	
	//参数设置
	AVDictionary *opts = NULL;

	//设置rtsp流已tcp协议打开
	av_dict_set(&opts, "rtsp_transport", "tcp", 0);

	//网络延时时间，如果网络状况不好时，延时太小会导致视频中断
	av_dict_set(&opts, "max_delay", "500", 0);

	//解封装上下文
	AVFormatContext *ic = NULL;
	int re = avformat_open_input(
		&ic,
		path,	//视频流路径
		0,		// 0表示自动选择解封器
		&opts	//参数设置，比如rtsp的延时时间
	);

	//判断是否打开成功
	if (re != 0)
	{
		char buf[1024] = { 0 };
		av_strerror(re, buf, sizeof(buf) - 1);
		cout << "open " << path << " failed! :" << buf << endl;
		getchar();
		return -1;
	}
	cout << "open " << path << " success! " << endl;

	//获取流信息 
	re = avformat_find_stream_info(ic, 0);

	//总时长(毫秒)
	int totalMs = ic->duration / (AV_TIME_BASE / 1000);
	cout << "totalMs = " << totalMs << endl;

	//打印视频流详细信息
	av_dump_format(ic, 0, path, 0);



	if (ic)
	{
		//释放清理上下文
		avformat_close_input(&ic);
	}
	getchar();
	return 0;
}
