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

#pragma once

struct AVFrame;
struct AVPacket;
class AVCodecContext;
enum XSampleFMT
{
	X_S16 = 1,
	X_FLATP =8
};

///音视频编码接口类
class XMediaEncode
{
public:
	///输入参数
	int inWidth = 1280;
	int inHeight = 720;
	int inPixSize = 3;
	int channels = 2;
	int sampleRate = 44100;
	XSampleFMT inSampleFmt = X_S16;

	///输出参数
	int outWidth = 1280;
	int outHeight = 720;
	int bitrate = 4000000;//压缩后每秒视频的bit位大小 50kB
	int fps = 25;
	int nbSample = 1024;
	XSampleFMT outSampleFmt = X_FLATP;


	//工厂生产方法
	static XMediaEncode * Get(unsigned char index = 0);
	
	//初始化像素格式转换的上下文初始化
	virtual bool InitScale() = 0;

	//音频重采样上下文初始化
	virtual bool InitResample() = 0;

	//返回值无需调用者清理
	virtual AVFrame *Resample(char *data) = 0;

	//返回值无需调用者清理
	virtual AVFrame* RGBToYUV(char *rgb) = 0;

	//视频编码器初始化
	virtual bool InitVideoCodec() = 0;

	//音频编码初始化
	virtual bool InitAudioCode() = 0;

	//视频编码 返回值无需调用者清理
	virtual AVPacket * EncodeVideo(AVFrame* frame) = 0;

	//音频编码 返回值无需调用者清理
	virtual AVPacket * EncodeAudio(AVFrame* frame) = 0;

	virtual ~XMediaEncode();

	AVCodecContext *vc = 0;	//音频编码器上下文
	AVCodecContext *ac = 0; //音频编码器上下文
protected:
	XMediaEncode();
};

