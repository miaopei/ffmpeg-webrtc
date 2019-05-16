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
#include <QThread>
#include "XData.h"
#include <list>
class XDataThread:public QThread
{
public:
	//（缓冲列表大小）列表最大值，超出删除最旧的数据
	int maxList = 100;

	//在列表结尾插入
	virtual void Push(XData d);

	//读取列表中最早的数据,返回数据需要调用XData.Drop清理
	virtual XData Pop();

	//启动线程
	virtual bool Start();

	//退出线程，并等待线程退出（阻塞）
	virtual void Stop();

	virtual void Clear();
	XDataThread();
	virtual ~XDataThread();
protected:
	//存放交互数据 插入策略 先进先出
	std::list<XData> datas;

	//交互数据列表大小
	int dataCount = 0;
	//互斥访问 datas;
	QMutex mutex;

	//处理线程退出
	bool isExit = false;
};

