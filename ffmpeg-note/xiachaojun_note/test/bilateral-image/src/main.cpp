/*************************************************************************
	> File Name: main.cpp
	> Author: Mr.Miaow
	> Mail: miaopei163@163.com 
	> Created Time: 五  5/17 10:50:31 2019
 ************************************************************************/

#include <iostream>
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core.hpp"

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    Mat src = imread("001.jpg");
	if (!src.data)
	{
		cout << "open file failed!" << endl;
		//getchar();
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

