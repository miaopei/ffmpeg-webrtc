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

#include <QtCore/QCoreApplication>
#include <iostream>
#include <QThread>
#include "XMediaEncode.h"
#include "XRtmp.h"
#include "XAudioRecord.h"
#include "XVideoCapture.h"
using namespace std;
int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	

	char *outUrl = "rtmp://192.168.1.44/live";
	int ret = 0;
	int sampleRate = 44100;
	int channels = 2;
	int sampleByte = 2;
	int nbSample = 1024;

	

	///打开摄像机
	XVideoCapture *xv = XVideoCapture::Get();
	if (!xv->Init(0))
	{
		cout << "open camera failed!" << endl;
		getchar();
		return -1;
	}
	cout << "open camera success!" << endl;
	xv->Start();

	///1 qt音频开始录制
	XAudioRecord *ar = XAudioRecord::Get();
	ar->sampleRate = sampleRate;
	ar->channels = channels;
	ar->sampleByte = sampleByte;
	ar->nbSamples = nbSample;
	if (!ar->Init())
	{
		cout << "XAudioRecord Init failed!" << endl;
		getchar();
		return -1;
	}
	ar->Start();
	
	///音视频编码类
	XMediaEncode *xe = XMediaEncode::Get();

	///2 初始化格式转换上下文
	///3 初始化输出的数据结构
	xe->inWidth = xv->width;
	xe->inHeight = xv->height;
	xe->outWidth = xv->width;
	xe->outHeight = xv->height;
	if (!xe->InitScale())
	{
		getchar();
		return -1;
	}
	cout << "初始化视频像素转换上下文成功!" << endl;
	
	///2 音频重采样 上下文初始化
	xe->channels = channels;
	xe->nbSample = nbSample;
	xe->sampleRate = sampleRate;
	xe->inSampleFmt = XSampleFMT::X_S16;
	xe->outSampleFmt = XSampleFMT::X_FLATP;
	if (!xe->InitResample())
	{
		getchar();
		return -1;
	}
	///4 初始化音频编码器
	if (!xe->InitAudioCode())
	{
		getchar();
		return -1;
	}

	///初始化视频编码器
	if (!xe->InitVideoCodec())
	{
		getchar();
		return -1;
	}


	///5 输出封装器和音频流配置
	//a 创建输出封装器上下文
	XRtmp *xr = XRtmp::Get(0);
	if (!xr->Init(outUrl))
	{
		getchar();
		return -1;
	}

	//添加视频流
	int vindex = 0;
	vindex = xr->AddStream(xe->vc);
	if (vindex<0)
	{
		getchar();
		return -1;
	}

	//b 添加音频流 
	int aindex = xr->AddStream(xe->ac);
	if (aindex<0)
	{
		getchar();
		return -1;
	}

	///打开rtmp 的网络输出IO
	//写入封装头
	if (!xr->SendHead())
	{
		getchar();
		return -1;
	}
	
	ar->Clear();
	xv->Clear();
	long long beginTime = GetCurTime();


	//一次读取一帧音频的字节数
	for (;;)
	{
		//一次读取一帧音频
		XData ad = ar->Pop();
		XData vd = xv->Pop();
		if (ad.size<=0 && vd.size<=0)
		{
			QThread::msleep(1);
			continue;
		}

		//处理音频
		if (ad.size > 0)
		{
			ad.pts = ad.pts - beginTime;

			//重采样源数据
			XData pcm = xe->Resample(ad);
			ad.Drop();
			
			XData pkt = xe->EncodeAudio(pcm);
			if (pkt.size > 0)
			{
				////推流
				if (xr->SendFrame(pkt,aindex))
				{
					cout << "#" << flush;
				}
			}
			
		}

		//处理视频
		if (vd.size > 0)
		{
			vd.pts = vd.pts - beginTime;
			XData yuv = xe->RGBToYUV(vd);
			vd.Drop();
			XData pkt = xe->EncodeVideo(yuv);
			if (pkt.size> 0)
			{
				////推流
				if (xr->SendFrame(pkt,vindex))
				{
					cout << "@" << flush;
				}
			}
		}
	}

	getchar();
	return a.exec();
}
