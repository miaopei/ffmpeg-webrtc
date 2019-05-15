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
#include <QAudioInput>
#include <iostream>
#include <QThread>
#include "XMediaEncode.h"
#include "XRtmp.h"
using namespace std;
int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	

	char *outUrl = "rtmp://192.168.1.44/live";
	int ret = 0;
	int sampleRate = 44100;
	int channels = 2;
	int sampleByte = 2;

	///1 qt音频开始录制
	QAudioFormat fmt;
	fmt.setSampleRate(sampleRate);
	fmt.setChannelCount(channels);
	fmt.setSampleSize(sampleByte *8);
	fmt.setCodec("audio/pcm");
	fmt.setByteOrder(QAudioFormat::LittleEndian);
	fmt.setSampleType(QAudioFormat::UnSignedInt);
	QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
	if (!info.isFormatSupported(fmt))
	{
		cout << "Audio format not support!" << endl;
		fmt = info.nearestFormat(fmt);
	}
	QAudioInput *input = new QAudioInput(fmt);
	//开始录制音频
	QIODevice *io = input->start();


	///2 音频重采样 上下文初始化
	XMediaEncode *xe = XMediaEncode::Get();
	xe->channels = channels;
	xe->nbSample = 1024;
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
	


	///5 输出封装器和音频流配置
	//a 创建输出封装器上下文
	XRtmp *xr = XRtmp::Get(0);
	if (!xr->Init(outUrl))
	{
		getchar();
		return -1;
	}
	//b 添加音频流 
	if (!xr->AddStream(xe->ac))
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
	//一次读取一帧音频的字节数
	int readSize = xe->nbSample*channels*sampleByte;
	char *buf = new char[readSize];
	for (;;)
	{
		//一次读取一帧音频
		if (input->bytesReady() < readSize)
		{
			QThread::msleep(1);
			continue;
		}
		int size = 0;
		while (size != readSize)
		{
			int len = io->read(buf + size, readSize - size);
			if (len < 0)break;
			size += len;
		}

		if (size != readSize)continue;

		//已经读一帧源数据
		//cout << size << " ";
		//重采样源数据
		AVFrame *pcm = xe->Resample(buf);

		//pts 运算
		//  nb_sample/sample_rate  = 一帧音频的秒数sec
		// timebase  pts = sec * timebase.den
		AVPacket *pkt = xe->EncodeAudio(pcm);
		if (!pkt)continue;
		////推流
		xr->SendFrame(pkt);
		if (ret == 0)
		{
			cout << "#" << flush;
		}

	}
	delete buf;

	getchar();
	return a.exec();
}
