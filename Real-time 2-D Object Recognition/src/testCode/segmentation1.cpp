
#include <algorithm>
#include <iostream>
#include<opencv2/opencv.hpp>
#include "filter.h"


using namespace std;

int main(int argc, char* argv[]) {

  cv::Mat img, img_edge, labels, centroids, img_color, stats;
  cv::Mat temp,result;
  // load image or show help if no image was provided

  img = cv::imread("/home/ravina/Desktop/OpenCV/ObjectDetection/sample/img1p3.png");
 // cv::threshold(img, img_edge, 128, 255, cv::THRESH_BINARY);
 // cv::imshow("Image after threshold", img_edge);
    
    cv::Mat gray_mat(img.size(), CV_8U);
    cv::cvtColor(img, gray_mat, cv::COLOR_RGB2GRAY);

    Mat image_th;
  
    cv::Mat bin_mat(gray_mat.size(), gray_mat.type());
  //  cv::adaptiveThreshold(gray_mat, image_th, 255,
	//		  cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 3, 5);

    ///*
    //frame.copyTo(temp);
    image_th = Mat::zeros(img.rows,img.cols, CV_8UC1);
   // cvtColor(frame, temp, cv::COLOR_RGB2GRAY);
    thresholdingGray(gray_mat, image_th);

    grassfireTransform(image_th, image_th);
    shrinkingImage(image_th, image_th);
    grassfireTransform(image_th, image_th);
    shrinkingImage(image_th, image_th);
    growingOperations(image_th, image_th);
    growingOperations(image_th, image_th);
    grassfireTransform(image_th, image_th);
    shrinkingImage(image_th, image_th);
    growingOperations(image_th, image_th);


  int i, nccomps = cv::connectedComponentsWithStats (
	image_th, 
	labels,
	stats, 
	centroids
  );
  cout << "Total Connected Components Detected: " << nccomps << endl;

  vector<cv::Vec3b> colors(nccomps+1);
  colors[0] = cv::Vec3b(0,0,0); // background pixels remain black.
  for( i = 1; i <= nccomps; i++ ) {
    colors[i] = cv::Vec3b(rand()%256, rand()%256, rand()%256);
    if( stats.at<int>(i-1, cv::CC_STAT_AREA) < 5 )
    colors[i] = cv::Vec3b(0,0,0); // small regions are painted with black too.
  }
  img_color = cv::Mat::zeros(img.size(), CV_8UC3);
  for( int y = 0; y < img_color.rows; y++ )
    for( int x = 0; x < img_color.cols; x++ )
    {
      int label = labels.at<int>(y, x);
      CV_Assert(0 <= label && label <= nccomps);
      img_color.at<cv::Vec3b>(y, x) = colors[label];
    }

  cv::imshow("Labeled map", img_color);
  cv::waitKey();
  return 0;
}