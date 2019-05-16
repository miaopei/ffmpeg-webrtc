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
#include "XData.h"
class AVCodecContext;
class AVPacket;
class XRtmp
{
public:

	//工厂生产方法
	static XRtmp * Get(unsigned char index = 0);
	
	//初始化封装器上下文
	virtual bool Init(const char *url) = 0;

	//添加视频或者音频流 失败返回-1 成功返回流索引
	virtual int AddStream(const AVCodecContext *c) = 0;

	//打开rtmp网络IO，发送封装头
	virtual bool SendHead() = 0;

	//rtmp 帧推流
	virtual bool SendFrame(XData d,int streamIndex = 0) = 0;

	virtual void Close() = 0;
	virtual ~XRtmp();
protected:
	XRtmp();
};

