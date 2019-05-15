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
#include "XMediaEncode.h"
extern "C"
{
#include <libswscale/swscale.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
}

#pragma comment(lib, "swscale.lib")
#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "avutil.lib")
#pragma comment(lib,"swresample.lib")

#include <iostream>
using namespace std;



#if defined WIN32 || defined _WIN32
#include <windows.h>
#endif
//获取CPU数量
static int XGetCpuNum()
{
#if defined WIN32 || defined _WIN32
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);

	return (int)sysinfo.dwNumberOfProcessors;
#elif defined __linux__
	return (int)sysconf(_SC_NPROCESSORS_ONLN);
#elif defined __APPLE__
	int numCPU = 0;
	int mib[4];
	size_t len = sizeof(numCPU);

	// set the mib for hw.ncpu
	mib[0] = CTL_HW;
	mib[1] = HW_AVAILCPU;  // alternatively, try HW_NCPU;

						   // get the number of CPUs from the system
	sysctl(mib, 2, &numCPU, &len, NULL, 0);

	if (numCPU < 1)
	{
		mib[1] = HW_NCPU;
		sysctl(mib, 2, &numCPU, &len, NULL, 0);

		if (numCPU < 1)
			numCPU = 1;
	}
	return (int)numCPU;
#else
	return 1;
#endif
}

class CXMediaEncode :public XMediaEncode
{
public:
	void Close()
	{
		if (vsc)
		{
			sws_freeContext(vsc);
			vsc = NULL;
		}
		if (asc)
		{
			swr_free(&asc);
		}
		if (yuv)
		{
			av_frame_free(&yuv);
		}		
		if (vc)
		{
			avcodec_free_context(&vc);
		}

		if (pcm)
		{
			av_frame_free(&pcm);
		}


		vpts = 0;
		av_packet_unref(&apack);
		apts = 0;
		av_packet_unref(&vpack);
	}

	bool InitAudioCode()
	{
		if (!CreateCodec(AV_CODEC_ID_AAC))
		{
			return false;
		}
		ac->bit_rate = 40000;
		ac->sample_rate = sampleRate;
		ac->sample_fmt = AV_SAMPLE_FMT_FLTP;
		ac->channels = channels;
		ac->channel_layout = av_get_default_channel_layout(channels);
		return OpenCodec(&ac);
	}

	bool InitVideoCodec()
	{
		///4 初始化编码上下文
		//a 找到编码器
		if (!CreateCodec(AV_CODEC_ID_H264))
		{
			return false;
		}
		vc->bit_rate = 50 * 1024 * 8;//压缩后每秒视频的bit位大小 50kB
		vc->width = outWidth;
		vc->height = outHeight;
		vc->time_base = { 1,fps };
		vc->framerate = { fps,1 };

		//画面组的大小，多少帧一个关键帧
		vc->gop_size = 50;
		vc->max_b_frames = 0;
		vc->pix_fmt = AV_PIX_FMT_YUV420P;
		return OpenCodec(&vc);
	}
	AVPacket * EncodeAudio(AVFrame* frame)
	{
		pcm->pts = apts;
		apts += av_rescale_q(pcm->nb_samples, { 1,sampleRate }, ac->time_base);
		int ret = avcodec_send_frame(ac, pcm);
		if (ret != 0)
			return NULL;
		av_packet_unref(&apack);
		ret = avcodec_receive_packet(ac, &apack);
		if (ret != 0)
			return NULL;
		return &apack;
	}

	AVPacket * EncodeVideo(AVFrame* frame)
	{
		av_packet_unref(&vpack);
		///h264编码
		frame->pts = vpts;
		vpts++;
		int ret = avcodec_send_frame(vc, frame);
		if (ret != 0)
			return NULL;

		ret = avcodec_receive_packet(vc, &vpack);
		if (ret != 0 || vpack.size<= 0)
			return NULL;
		return &vpack;
	}
	bool InitScale()
	{
		///2 初始化格式转换上下文
		vsc = sws_getCachedContext(vsc,
			inWidth, inHeight, AV_PIX_FMT_BGR24,	 //源宽、高、像素格式
			outWidth, outHeight, AV_PIX_FMT_YUV420P,//目标宽、高、像素格式
			SWS_BICUBIC,  // 尺寸变化使用算法
			0, 0, 0
			);
		if (!vsc)
		{
			cout<<"sws_getCachedContext failed!";
			return false;
		}

		///3 初始化输出的数据结构
		yuv = av_frame_alloc();
		yuv->format = AV_PIX_FMT_YUV420P;
		yuv->width = inWidth;
		yuv->height = inHeight;
		yuv->pts = 0;
		//分配yuv空间
		int ret = av_frame_get_buffer(yuv, 32);
		if (ret != 0)
		{
			char buf[1024] = { 0 };
			av_strerror(ret, buf, sizeof(buf) - 1);
			throw exception(buf);
		}
		return true;
	}

	AVFrame* RGBToYUV(char *rgb)
	{
		///rgb to yuv
		//输入的数据结构
		uint8_t *indata[AV_NUM_DATA_POINTERS] = { 0 };
		//indata[0] bgrbgrbgr
		//plane indata[0] bbbbb indata[1]ggggg indata[2]rrrrr 
		indata[0] = (uint8_t*)rgb;
		int insize[AV_NUM_DATA_POINTERS] = { 0 };
		//一行（宽）数据的字节数
		insize[0] = inWidth * inPixSize;

		int h = sws_scale(vsc, indata, insize, 0, inHeight, //源数据
			yuv->data, yuv->linesize);
		if (h <= 0)
		{
			return NULL;
		}
		return yuv;
	}

	bool InitResample()
	{
		///2 音频重采样 上下文初始化
		asc = NULL;
		asc = swr_alloc_set_opts(asc,
			av_get_default_channel_layout(channels), (AVSampleFormat)outSampleFmt, sampleRate,//输出格式
			av_get_default_channel_layout(channels), (AVSampleFormat)inSampleFmt, sampleRate, 0, 0);//输入格式
		if (!asc)
		{
			cout << "swr_alloc_set_opts failed!";
			return false;
		}
		int ret = swr_init(asc);
		if (ret != 0)
		{
			char err[1024] = { 0 };
			av_strerror(ret, err, sizeof(err) - 1);
			cout << err << endl;
			return false;
		}
		cout << "音频重采样 上下文初始化成功!" << endl;

		///3 音频重采样输出空间分配
		pcm = av_frame_alloc();
		pcm->format = outSampleFmt;
		pcm->channels = channels;
		pcm->channel_layout = av_get_default_channel_layout(channels);
		pcm->nb_samples = nbSample; //一帧音频一通道的采用数量
		ret = av_frame_get_buffer(pcm, 0); // 给pcm分配存储空间
		if (ret != 0)
		{
			char err[1024] = { 0 };
			av_strerror(ret, err, sizeof(err) - 1);
			cout << err << endl;
			return false;
		}
		return true;
	}
	AVFrame *Resample(char *data)
	{
		const uint8_t *indata[AV_NUM_DATA_POINTERS] = { 0 };
		indata[0] = (uint8_t *)data;
		int len = swr_convert(asc, pcm->data, pcm->nb_samples, //输出参数，输出存储地址和样本数量
			indata, pcm->nb_samples
			);
		if (len <= 0)
		{
			return NULL;
		}
		return pcm;
	}
private:
	bool OpenCodec(AVCodecContext **c)
	{
		//打开音频编码器
		int ret = avcodec_open2(*c, 0, 0);
		if (ret != 0)
		{
			char err[1024] = { 0 };
			av_strerror(ret, err, sizeof(err) - 1);
			cout << err << endl;
			avcodec_free_context(c);
			return false;
		}
		cout << "avcodec_open2 success!" << endl;
		return true;
	}

	bool CreateCodec(AVCodecID cid)
	{
		///4 初始化编码器 AV_CODEC_ID_AAC
		AVCodec *codec = avcodec_find_encoder(cid);
		if (!codec)
		{
			cout << "avcodec_find_encoder  failed!" << endl;
			return false;
		}
		//音频编码器上下文
		ac = avcodec_alloc_context3(codec);
		if (!ac)
		{
			cout << "avcodec_alloc_context3  failed!" << endl;
			return false;
		}
		cout << "avcodec_alloc_context3 success!" << endl;

		ac->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
		ac->thread_count = XGetCpuNum();
		return true;
	}
	SwsContext *vsc = NULL;		//像素格式转换上下文
	SwrContext * asc = NULL;	//音频重采样上下文
	AVFrame *yuv = NULL;		//输出的YUV
	AVFrame *pcm = NULL;		//重采样输出PCM
	AVPacket vpack = {0};		//视频帧
	AVPacket apack = { 0 };		//音频帧
	int vpts = 0;
	int apts = 0;

};

XMediaEncode * XMediaEncode::Get(unsigned char index)
{

	static bool isFirst = true;
	if (isFirst)
	{
		//注册所有的编解码器
		avcodec_register_all();
		isFirst = false;
	}

	static CXMediaEncode cxm[255];
	return &cxm[index];
}

XMediaEncode::XMediaEncode()
{
}


XMediaEncode::~XMediaEncode()
{
}
