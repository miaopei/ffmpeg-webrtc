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

#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#pragma comment(lib,"opencv_world320.lib")

using namespace std;
using namespace cv;
int main(int argc, char argv[])
{
	Mat src = imread("001.jpg");
	if (!src.data)
	{
		cout << "open file failed!" << endl;
		getchar();
		return -1;
	}
	namedWindow("src");
	moveWindow("src", 100, 100);
	imshow("src", src);
	Mat image;
	int d = 3;
	namedWindow("image");
	moveWindow("image", 600, 100);
	for (;;)
	{
	
		long long b = getTickCount();
		bilateralFilter(src, image, d, d * 2, d / 2);
		double sec=(double)(getTickCount() - b) / (double)getTickFrequency();
		cout <<"d = "<< d << " sec is " << sec << endl;
		imshow("image", image);
		int k = waitKey(0);
		if (k == 'd') d += 2;
		else if (k == 'f')d -= 2;
		else break;

	}
	



	waitKey(0);


	return 0;
}