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
extern "C"
{
#include <libswresample/swresample.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

#pragma comment(lib,"swresample.lib")
#pragma comment(lib,"avutil.lib")
#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "avformat.lib")
#include <QThread>
using namespace std;
int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	//注册所有的编解码器
	avcodec_register_all();

	//注册所有的封装器
	av_register_all();

	//注册所有网络协议
	avformat_network_init();

	char *outUrl = "rtmp://192.168.1.44/live";

	int sampleRate = 44100;
	int channels = 2;
	int sampleByte = 2;
	AVSampleFormat inSampleFmt = AV_SAMPLE_FMT_S16;
	AVSampleFormat outSampleFmt = AV_SAMPLE_FMT_FLTP;


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
	QAudioInput *input = new QAudioInput(fmt);
	//开始录制音频
	QIODevice *io = input->start();


	///2 音频重采样 上下文初始化
	SwrContext * asc = NULL;
	asc = swr_alloc_set_opts(asc,
		av_get_default_channel_layout(channels), outSampleFmt, sampleRate,//输出格式
		av_get_default_channel_layout(channels), inSampleFmt, sampleRate, 0, 0);//输入格式
	if (!asc)
	{
		cout << "swr_alloc_set_opts failed!";
		getchar();
		return -1;
	}
	int ret = swr_init(asc);
	if (ret != 0)
	{
		char err[1024] = { 0 };
		av_strerror(ret, err, sizeof(err) - 1);
		cout << err << endl;
		getchar();
		return -1;
	}
	cout << "音频重采样 上下文初始化成功!" << endl;


	///3 音频重采样输出空间分配
	AVFrame *pcm = av_frame_alloc();
	pcm->format = outSampleFmt;
	pcm->channels = channels;
	pcm->channel_layout = av_get_default_channel_layout(channels);
	pcm->nb_samples = 1024; //一帧音频一通道的采用数量
	ret = av_frame_get_buffer(pcm, 0); // 给pcm分配存储空间
	if (ret != 0)
	{
		char err[1024] = { 0 };
		av_strerror(ret, err, sizeof(err) - 1);
		cout << err << endl;
		getchar();
		return -1;
	}

	///4 初始化音频编码器
	AVCodec *codec = avcodec_find_encoder(AV_CODEC_ID_AAC);
	if (!codec)
	{
		cout << "avcodec_find_encoder AV_CODEC_ID_AAC failed!" << endl;
		getchar();
		return -1;
	}
	//音频编码器上下文
	AVCodecContext *ac = avcodec_alloc_context3(codec);
	if (!ac)
	{
		cout << "avcodec_alloc_context3 AV_CODEC_ID_AAC failed!" << endl;
		getchar();
		return -1;
	}
	cout << "avcodec_alloc_context3 success!" << endl;

	ac->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
	ac->thread_count = 8;
	ac->bit_rate = 40000;
	ac->sample_rate = sampleRate;
	ac->sample_fmt = AV_SAMPLE_FMT_FLTP;
	ac->channels = channels;
	ac->channel_layout = av_get_default_channel_layout(channels);

	//打开音频编码器
	ret = avcodec_open2(ac, 0, 0);
	if (ret != 0)
	{
		char err[1024] = { 0 };
		av_strerror(ret, err, sizeof(err) - 1);
		cout << err << endl;
		getchar();
		return -1;
	}
	cout << "avcodec_open2 success!" << endl;


	///5 输出封装器和音频流配置
	//a 创建输出封装器上下文
	AVFormatContext *ic = NULL;
	ret = avformat_alloc_output_context2(&ic, 0, "flv", outUrl);
	if (ret != 0)
	{
		char buf[1024] = { 0 };
		av_strerror(ret, buf, sizeof(buf) - 1);
		cout << buf << endl;
		getchar();
		return -1;
	}
	//b 添加音频流 
	AVStream *as = avformat_new_stream(ic, NULL);
	if (!as)
	{
		char buf[1024] = { 0 };
		av_strerror(ret, buf, sizeof(buf) - 1);
		cout << buf << endl;
		getchar();
		return -1;
	}
	as->codecpar->codec_tag = 0;
	//从编码器复制参数
	avcodec_parameters_from_context(as->codecpar, ac);
	av_dump_format(ic, 0, outUrl, 1);

	///打开rtmp 的网络输出IO
	ret = avio_open(&ic->pb, outUrl, AVIO_FLAG_WRITE);
	if (ret != 0)
	{
		char buf[1024] = { 0 };
		av_strerror(ret, buf, sizeof(buf) - 1);
		cout << buf << endl;
		getchar();
		return -1;
	}

	//写入封装头
	ret = avformat_write_header(ic, NULL);
	if (ret != 0)
	{
		char buf[1024] = { 0 };
		av_strerror(ret, buf, sizeof(buf) - 1);
		cout << buf << endl;
		getchar();
		return -1;
	}

	//一次读取一帧音频的字节数
	int readSize = pcm->nb_samples*channels*sampleByte;
	char *buf = new char[readSize];
	int apts = 0;
	AVPacket pkt = { 0 };
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
		const uint8_t *indata[AV_NUM_DATA_POINTERS] = { 0 };
		indata[0] = (uint8_t *)buf;
		int len = swr_convert(asc, pcm->data, pcm->nb_samples, //输出参数，输出存储地址和样本数量
			indata, pcm->nb_samples
			);


		//pts 运算
		//  nb_sample/sample_rate  = 一帧音频的秒数sec
		// timebase  pts = sec * timebase.den
		pcm->pts = apts;
		apts += av_rescale_q(pcm->nb_samples, { 1,sampleRate }, ac->time_base);

		int ret = avcodec_send_frame(ac, pcm);
		if (ret != 0)continue;

		av_packet_unref(&pkt);
		ret = avcodec_receive_packet(ac, &pkt);
		if (ret != 0)continue;

		cout << pkt.size << " " << flush;
		//推流
		pkt.pts = av_rescale_q(pkt.pts, ac->time_base, as->time_base);
		pkt.dts = av_rescale_q(pkt.dts, ac->time_base, as->time_base);
		pkt.duration = av_rescale_q(pkt.duration, ac->time_base, as->time_base);
		ret = av_interleaved_write_frame(ic, &pkt);
		if (ret == 0)
		{
			cout << "#" << flush;
		}

	}
	delete buf;

	getchar();
	return a.exec();
}
