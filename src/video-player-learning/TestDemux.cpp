// TestDemux.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <thread>
#include <iostream>

extern "C"
{
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
}

using std::cout;
using std::endl;

#pragma comment(lib,"avformat.lib")
#pragma comment(lib,"avutil.lib")
#pragma comment(lib,"avcodec.lib")
#pragma comment(lib,"swscale.lib")
#pragma comment(lib,"swresample.lib")

const char *path = "../../video/cloud.mp4";

//分数运算
static double r2d(AVRational r)
{
	return r.den == 0 ? 0 : (double)r.num / (double)r.den;
}


void XSleep(int ms)
{
	//c++ 11
	std::chrono::milliseconds du(ms);
	std::this_thread::sleep_for(du);
}

int main()

{
	cout << "Test Demux FFmepeg" << endl;

	//初始化封装库
	av_register_all();

	//注册解码器
	avcodec_register_all();

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

	//音视频索引，读取时区分音视频
	int videoStream = 0;
	int audioStream = 1;

	//打印视频流详细信息
	av_dump_format(ic, 0, path, 0);

	for (size_t i = 0; i < ic->nb_streams; ++i)
	{
		AVStream *as = ic->streams[i];
		if (as->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
		{
			audioStream = i;
			cout << i << "音频信息" << endl;
			cout << "sample_rate = " << as->codecpar->sample_rate << endl;
			//AVSampleFormat;
			cout << "channels = " << as->codecpar->channels << endl;
			//一帧数据  = 单通道样本数 
			cout << "frame_size = " << as->codecpar->frame_size << endl;
			//1024 * 2 * 2 = 4096  fps = sample_rate/frame_size
		}
		else if (as->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			videoStream = i;
			cout << i << " " << "视频信息" << endl;
			cout << "width = " << as->codecpar->width << endl;
			cout << "height = " << as->codecpar->height << endl;
			//帧率 fps 分数转换
			cout << "video fps = " << r2d(as->avg_frame_rate) << endl;
		}
	}

	//获取视频流
	videoStream = av_find_best_stream(ic, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
	//ic->streams[videoStream]

	/**************************************************************************************/
	//找到视频解码器
	AVCodec *vcodec = avcodec_find_decoder(ic->streams[videoStream]->codecpar->codec_id);
	if (!vcodec)
	{
		cout << "Can't find the codec id: " << ic->streams[videoStream]->codecpar->codec_id << endl;
		getchar();
		return -1;
	}
	//创建解码器上下文
	AVCodecContext *vc = avcodec_alloc_context3(vcodec);

	//配置解码器上下文参数
	avcodec_parameters_to_context(vc, ic->streams[videoStream]->codecpar);

	//八线程解码
	vc->thread_count = 8;

	//打开解码器上下文
	re = avcodec_open2(vc, 0, 0);
	if (re != 0)
	{
		char buf[1024] = { 0 };
		av_strerror(re, buf, sizeof(buf) - 1);
		cout << "avcodec_open2  failed! :" << buf << endl;
		getchar();
		return -1;
	}
	cout << "video avcodec_open2 success!" << endl;

	/**************************************************************************************/
	//找到音频解码器
	AVCodec *acodec = avcodec_find_decoder(ic->streams[audioStream]->codecpar->codec_id);
	if (!acodec)
	{
		cout << "can't find the codec id " << ic->streams[audioStream]->codecpar->codec_id;
		getchar();
		return -1;
	}
	cout << "find the AVCodec " << ic->streams[audioStream]->codecpar->codec_id << endl;

	//创建解码器上下文呢
	AVCodecContext *ac = avcodec_alloc_context3(acodec);

	//配置解码器上下文参数
	avcodec_parameters_to_context(ac, ic->streams[audioStream]->codecpar);

	//八线程解码
	ac->thread_count = 8;

	//打开解码器上下文
	re = avcodec_open2(ac, 0, 0);
	if (re != 0)
	{
		char buf[1024] = { 0 };
		av_strerror(re, buf, sizeof(buf) - 1);
		cout << "avcodec_open2  failed! :" << buf << endl;
		getchar();
		return -1;
	}
	cout << "audio avcodec_open2 success!" << endl;

	//malloc AVPacket并初始化
	AVPacket *pkt = av_packet_alloc();
	AVFrame *frame = av_frame_alloc();


	//像素格式和尺寸转换上下文
	SwsContext *vctx = NULL;
	unsigned char *rgb = NULL;

	//音频重采样 上下文初始化
	SwrContext *actx = swr_alloc();
	actx = swr_alloc_set_opts(actx,
		av_get_default_channel_layout(2),			//输出格式
		AV_SAMPLE_FMT_S16,							//输出样本格式
		ac->sample_rate,							//输出采样率
		av_get_default_channel_layout(ac->channels),//输入格式
		ac->sample_fmt,
		ac->sample_rate,
		0, 0
	);
	re = swr_init(actx);
	if (re != 0)
	{
		char buf[1024] = { 0 };
		av_strerror(re, buf, sizeof(buf) - 1);
		cout << "swr_init  failed! :" << buf << endl;
		getchar();
		return -1;
	}
	unsigned char *pcm = NULL;


	while (1)
	{
		int re = av_read_frame(ic, pkt);
		if (re != 0)
		{
			//循环播放
			cout << "==============================end==============================" << endl;
			int ms = 3000; //三秒位置 根据时间基数（分数）转换
			long long pos = (double)ms / (double)1000 * r2d(ic->streams[pkt->stream_index]->time_base);
			//seek到前一个关键帧位置
			av_seek_frame(ic, videoStream, pos, AVSEEK_FLAG_BACKWARD | AVSEEK_FLAG_FRAME);
			continue;
		}
		cout << "pkt->size = " << pkt->size << endl;
		//显示的时间
		cout << "pkt->pts 显示时间 = " << pkt->pts << endl;

		//转换为毫秒，方便做同步
		cout << "pkt->pts ms 显示时间 毫秒 = " << pkt->pts * (r2d(ic->streams[pkt->stream_index]->time_base) * 1000) << endl;

		//解码时间
		cout << "pkt->dts = " << pkt->dts << endl;

		AVCodecContext *cc = 0;
		if (pkt->stream_index == videoStream)
		{
			cout << "图像" << endl;
			cc = vc;
		}
		if (pkt->stream_index == audioStream)
		{
			cout << "音频" << endl;
			cc = ac;
		}

		//解码视频
		//发送packet到解码线程  send传NULL后调用多次receive取出所有缓冲帧
		re = avcodec_send_packet(cc, pkt);
		//释放，引用计数-1 为0释放空间
		av_packet_unref(pkt);

		if (re != 0)
		{
			char buf[1024] = { 0 };
			av_strerror(re, buf, sizeof(buf) - 1);
			cout << "avcodec_send_packet  failed! :" << buf << endl;
			continue;
		}


		for (;;)
		{
			//从线程中获取解码接口,一次send可能对应多次receive
			re = avcodec_receive_frame(cc, frame);
			if (re != 0) break;
			cout << "recv frame " << frame->format << " " << frame->linesize[0] << endl;

			//视频
			if (cc == vc)
			{
				vctx = sws_getCachedContext(
					vctx,							//传NULL会新创建
					frame->width, frame->height,	//输入的宽高
					(AVPixelFormat)frame->format,	//输入格式 YUV420p
					frame->width, frame->height,	//输出的宽高
					AV_PIX_FMT_RGBA,				//输入格式RGBA
					SWS_BILINEAR,					//尺寸变化的算法
					0, 0, 0);
				//if(vctx)
					//cout << "像素格式尺寸转换上下文创建或者获取成功！" << endl;
				//else
				//	cout << "像素格式尺寸转换上下文创建或者获取失败！" << endl;
				if (vctx)
				{
					if (!rgb) rgb = new unsigned char[frame->width*frame->height * 4];
					uint8_t *data[2] = { 0 };
					data[0] = rgb;
					int lines[2] = { 0 };
					lines[0] = frame->width * 4;
					re = sws_scale(vctx,
						frame->data,		//输入数据
						frame->linesize,	//输入行大小
						0,					//切片的开始位置
						frame->height,		//输入高度
						data,				//输出数据和大小
						lines
					);
					cout << "sws_scale = " << re << endl;
				}
				else //音频
				{
					uint8_t *data[2] = { 0 };
					if (!pcm) pcm = new uint8_t[frame->nb_samples * 2 * 2];
					data[0] = pcm;
					re = swr_convert(actx,
						data, frame->nb_samples,						//输出
						(const uint8_t**)frame->data, frame->nb_samples	//输入
					);
					cout << "swr_convert = " << re << endl;
				}

			}

		}
		//XSleep(500);
	}

	av_frame_free(&frame);
	av_packet_free(&pkt);

	if (ic)
	{
		//释放清理上下文
		avformat_close_input(&ic);
	}

	getchar();
	return 0;
}
