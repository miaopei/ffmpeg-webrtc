/*************************************************************************
	> File Name: main.cpp
	> Author: Mr.Miaow
	> Mail: miaopei163@163.com 
	> Created Time: 二  5/14 12:14:13 2019
 ************************************************************************/

#include <iostream>
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

