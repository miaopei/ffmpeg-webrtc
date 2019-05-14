/*******************************************************************************
**                                                                            **
**                     Jiedi(China nanjing)Ltd.                               **
**	               创建：夏曹俊，此代码可用作为学习参考                       **
*******************************************************************************/

/*****************************FILE INFOMATION***********************************
**
**  Project       : FFmpegSDK实战课程
**  Description   : 基于FFMpegSDK的实战课程
**  Contact       : xiacaojun@qq.com
**  博客   : http://blog.csdn.net/jiedichina
**  视频课程
**  http://edu.csdn.net/lecturer/lecturer_detail?lecturer_id=961
**  http://edu.51cto.com/lecturer/12016059.html
**  http://study.163.com/u/xiacaojun
**  https://jiedi.ke.qq.com/
**  购买课程后可以加群咨询课程学习问题
**  群号 132323693 fmpeg的直播推流课程
**  微信公众号  : jiedi2007
**	头条号	 : 夏曹俊
**
*******************************************************************************/

#include "XRtmp.h"
#include <iostream>
#include <string>
using namespace std;
extern "C"
{
	#include <libavformat/avformat.h>
	#include <libavutil/time.h>
}
#pragma comment(lib, "avformat.lib")
class CXRtmp :public XRtmp
{
public:
	
	void Close()
	{
		if (ic)
		{
			avformat_close_input(&ic);
			vs = NULL;
		}
		vc = NULL;
		url = "";
	}
	bool Init(const char *url)
	{
		///5 输出封装器和视频流配置
		//a 创建输出封装器上下文
		int ret = avformat_alloc_output_context2(&ic, 0, "flv", url);
		this->url = url;
		if (ret != 0)
		{
			char buf[1024] = { 0 };
			av_strerror(ret, buf, sizeof(buf) - 1);
			cout <<buf;
			return false;
		}
		return true;
	}

	bool AddStream(const AVCodecContext *c)
	{
		if (!c)return false;
		
		//b 添加视频流 
		AVStream *st = avformat_new_stream(ic, NULL);
		if (!st)
		{
			cout << "avformat_new_stream failed" << endl;
			return false;
		}
		st->codecpar->codec_tag = 0;
		//从编码器复制参数
		avcodec_parameters_from_context(st->codecpar, c);
		av_dump_format(ic, 0, url.c_str(), 1);

		if (c->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			vc = c;
			vs = st;
		}
		return true;
	}

	bool SendHead()
	{
		///打开rtmp 的网络输出IO
		int ret = avio_open(&ic->pb, url.c_str(), AVIO_FLAG_WRITE);
		if (ret != 0)
		{
			char buf[1024] = { 0 };
			av_strerror(ret, buf, sizeof(buf) - 1);
			cout << buf << endl;
			return false;
		}

		//写入封装头
		ret = avformat_write_header(ic, NULL);
		if (ret != 0)
		{
			char buf[1024] = { 0 };
			av_strerror(ret, buf, sizeof(buf) - 1);
			cout << buf << endl;
			return false;
		}
		return true;
	}

	bool SendFrame(AVPacket *pack)
	{
		if (pack->size <= 0 || !pack->data)return false;
		//推流
		pack->pts = av_rescale_q(pack->pts, vc->time_base, vs->time_base);
		pack->dts = av_rescale_q(pack->dts, vc->time_base, vs->time_base);
		pack->duration = av_rescale_q(pack->duration, vc->time_base, vs->time_base);
		int ret = av_interleaved_write_frame(ic, pack);
		if (ret == 0)
		{
			cout << "#" << flush;
			return true;
		}
	}
private:
	//rtmp flv 封装器
	AVFormatContext *ic = NULL;

	//视频编码器流
	const AVCodecContext *vc = NULL;

	AVStream *vs = NULL;

	string url = "";
};
//工厂生产方法
XRtmp * XRtmp::Get(unsigned char index)
{
	static CXRtmp cxr[255];

	static bool isFirst = true;
	if (isFirst)
	{
		//注册所有的封装器
		av_register_all();

		//注册所有网络协议
		avformat_network_init();
		isFirst = false;
	}
	return &cxr[index];
}
XRtmp::XRtmp()
{
}


XRtmp::~XRtmp()
{
}
