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

extern "C"
{
#include "libavformat/avformat.h"
#include "libavutil/time.h"
}
#include <iostream>
using namespace std;
#pragma comment(lib,"avformat.lib")
#pragma comment(lib,"avutil.lib")
#pragma comment(lib,"avcodec.lib")

int XError(int errNum)
{
	char buf[1024] = { 0 };
	av_strerror(errNum, buf, sizeof(buf));
	cout << buf << endl;
	getchar();
	return -1;
}
int main(int argc, char *argv[])
{

	char *inUrl = "test.flv";
	char *outUrl = "rtmp://192.168.1.44/live";

	//初始化所有封装和解封装 flv mp4 mov mp3
	av_register_all();

	//初始化网络库
	avformat_network_init();

	//////////////////////////////////////////////////////////////////////////
	//输入流 1 打开文件，解封装
	//输入封装上下文
	AVFormatContext *ictx = NULL;

	//打开文件，解封文件头
	int re = avformat_open_input(&ictx, inUrl, 0, 0);
	if (re != 0)
	{
		return XError(re);
	}
	cout << "open file " << inUrl << " Success." << endl;

	//获取音频视频流信息 ,h264 flv
	re = avformat_find_stream_info(ictx, 0);
	if (re != 0)
	{
		return XError(re);
	}
	av_dump_format(ictx, 0, inUrl, 0);
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	//输出流 

	//创建输出流上下文
	AVFormatContext *octx = NULL;
	re = avformat_alloc_output_context2(&octx, 0, "flv", outUrl);
	if (!octx)
	{
		return XError(re);
	}
	cout << "octx create success!" << endl;

	//配置输出流
	//遍历输入的AVStream
	for (int i = 0; i < ictx->nb_streams; i++)
	{
		//创建输出流
		AVStream *out = avformat_new_stream(octx, ictx->streams[i]->codec->codec);
		if (!out)
		{
			return XError(0);
		}
		//复制配置信息,同于MP4
		//re = avcodec_copy_context(out->codec, ictx->streams[i]->codec);
		re = avcodec_parameters_copy(out->codecpar, ictx->streams[i]->codecpar);
		out->codec->codec_tag = 0;
	}
	av_dump_format(octx, 0, outUrl, 1);
	//////////////////////////////////////////////////////////////////////////


	//rtmp推流

	//打开io
	re = avio_open(&octx->pb, outUrl, AVIO_FLAG_WRITE);
	if (!octx->pb)
	{
		return XError(re);
	}

	//写入头信息
	re = avformat_write_header(octx, 0);
	if (re < 0)
	{
		return XError(re);
	}
	cout << "avformat_write_header " << re << endl;
	AVPacket pkt;
	for (;;)
	{
		re = av_read_frame(ictx, &pkt);
		if (re != 0)
		{
			break;
		}
		cout << pkt.pts << " " << flush;
		//计算转换pts dts
		AVRational itime = ictx->streams[pkt.stream_index]->time_base;
		AVRational otime = octx->streams[pkt.stream_index]->time_base;
		pkt.pts = av_rescale_q_rnd(pkt.pts, itime, otime, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_NEAR_INF));
		pkt.dts = av_rescale_q_rnd(pkt.pts, itime, otime, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_NEAR_INF));
		pkt.duration = av_rescale_q_rnd(pkt.duration, itime, otime, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_NEAR_INF));
		pkt.pos = -1;

		//视频帧推送速度
		if (pkt.stream_index == 0)
			av_usleep(30*1000);

		re = av_interleaved_write_frame(octx, &pkt);
		if (re<0)
		{
			return XError(re);
		}
	}

	cout << "file to rtmp test" << endl;
	getchar();
	return 0;
}