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

#include "XAudioRecord.h"
#include <QAudioInput>
#include <iostream>
#include <list>

using namespace std;
class CXAudioRecord :public XAudioRecord
{
public:
	void run()
	{
		cout << "进入音频录制线程" << endl;
		//一次读取一帧音频的字节数
		int readSize = nbSamples*channels*sampleByte;
		char *buf = new char[readSize];
		while (!isExit)
		{
			//读取已录制音频
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
			if (size != readSize)
			{
				continue;
			}
			long long pts = GetCurTime();
			//已经读取一帧音频
			Push(XData(buf, readSize, pts));
		}
		delete buf;
		cout << "退出音频录制线程" << endl;
	}

	bool Init()
	{
		Stop();
		///1 qt音频开始录制
		QAudioFormat fmt;
		fmt.setSampleRate(sampleRate);
		fmt.setChannelCount(channels);
		fmt.setSampleSize(sampleByte * 8);
		fmt.setCodec("audio/pcm");
		fmt.setByteOrder(QAudioFormat::LittleEndian);
		fmt.setSampleType(QAudioFormat::UnSignedInt);
		QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
		if (!info.isFormatSupported(fmt))
		{
			cout << "Audio format not support!" << endl;
			fmt = info.nearestFormat(fmt);
		}
		input = new QAudioInput(fmt);

		//开始录制音频
		io = input->start();
		if (!io)
			return false;
		return true;
	}
	void Stop()
	{
		XDataThread::Stop();
		if (input)
			input->stop();
		if (io)
			io->close();
		input = NULL;
		io = NULL;
	}
	QAudioInput *input = NULL;
	QIODevice *io = NULL;
};

XAudioRecord *XAudioRecord::Get(XAUDIOTYPE type, unsigned char index)
{
	static CXAudioRecord record[255];
	return &record[index];
}

XAudioRecord::XAudioRecord()
{
}


XAudioRecord::~XAudioRecord()
{
}
