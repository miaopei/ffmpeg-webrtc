/*******************************************************************************
**                                                                            **
**                    Jiedi(China nanjing)Ltd.                                **
**	      版权归夏曹俊所有，此代码只能作为学习参考                            **                                                                            **
*******************************************************************************/

/*****************************FILE INFOMATION***********************************
**
** Project       : XVideoEdit
** Description   : 3d视频播放控件
** Contact       : xiacaojun@qq.com
**		  QQ群	 ：559312905 (可以咨询问题)
**   微信公众号  : jiedi2007
**
*******************************************************************************/

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
using namespace cv;
int main(int argc, char *argv[])
{
	Mat image = imread("1.png");
	namedWindow("img");
	imshow("img", image);
	waitKey(0);
	return 0;
}