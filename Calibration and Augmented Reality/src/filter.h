#ifndef __FILTER_H_
#define __FILTER_H_

#include<iostream>
#include<opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

int harrisCornerDetector( cv::Mat &src, cv::Mat &dst );
int shiTomasiCornerDetector( cv::Mat &src, cv::Mat &dst );

bool arucoDetector(cv::Mat Frame, std::vector<cv::Point2f>& box_Coordinates);
void PerspectiveTransform(cv::Mat Frame, std::vector<cv::Point2f> Coordinates, cv::Mat& TransformedFrame);
void FramesOverlapping(cv::Mat BaseFrame, cv::Mat SecFrame, std::vector<cv::Point2f> BoxCoordinates, cv::Mat& OverlapedFrame);
#endif