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
enum XFilterType
{
	XBILATERAL //双边滤波
};

namespace cv
{
class Mat;
}
#include <string>
#include <map>
class XFilter
{
public:
	static XFilter * Get(XFilterType t = XBILATERAL);
	virtual bool Filter(cv::Mat *src, cv::Mat *des) = 0 ;
	virtual bool Set(std::string key, double value);
	virtual ~XFilter();
protected:
	std::map<std::string, double>paras;
	XFilter();
};

