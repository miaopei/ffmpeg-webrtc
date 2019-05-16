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
#include "XDataThread.h"
enum XAUDIOTYPE
{
	X_AUDIO_QT
};

class XAudioRecord:public XDataThread
{
public:
	int channels = 2;		//声道数
	int sampleRate = 44100;	//样本率
	int sampleByte = 2;		//样板字节大小
	int nbSamples = 1024;	//一帧音频每个通道的样板数量
	//开始录制
	virtual bool Init() = 0;

	//停止录制
	virtual void Stop() = 0;

	static XAudioRecord *Get(XAUDIOTYPE type = X_AUDIO_QT, unsigned char index = 0);
	virtual ~XAudioRecord();
protected:
	XAudioRecord();
};

