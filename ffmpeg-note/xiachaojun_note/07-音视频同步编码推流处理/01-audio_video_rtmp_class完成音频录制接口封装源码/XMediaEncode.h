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

