#ifndef __FILTER_H_
#define __FILTER_H_

#include<iostream>
#include<opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

int greyscale( cv::Mat &src, cv::Mat &dst );
int blur5x5( cv::Mat &src, cv::Mat &dst );
int sobelX3x3( cv::Mat &src, cv::Mat3s &dst );
int sobelY3x3( cv::Mat &src, cv::Mat3s &dst );
int magnitude( cv::Mat &sx, cv::Mat &sy, cv::Mat &dst );
int blurQuantize( cv::Mat &src, cv::Mat &dst, int levels );
int cartoon( cv::Mat &src, cv::Mat &dst, int levels, int magThreshold );

//Task 10
int brightness(cv::Mat &src, cv::Mat &dst);
int negative(cv::Mat &src, cv::Mat &dst);

//Extension Task
int average_blur3x3( cv::Mat &src, cv::Mat &dst );
int imgSharpening(cv::Mat &src, cv::Mat3s &dst);
int sketch(cv::Mat &src, cv::Mat &dst);
int upsideDownImg(cv::Mat &src, cv::Mat &dst);
int reflectImage( cv::Mat &src, cv::Mat &dst );
int prewitX3( cv::Mat &src, cv::Mat3s &dst );
int prewitY3( cv::Mat &src, cv::Mat3s &dst );
int prewitMagnitude( cv::Mat &px, cv::Mat &py, cv::Mat &dst );

#endif