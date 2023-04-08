#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "filter.h"

using namespace cv;

int main(int argc, char** argv )
{
    
    Mat image;
    image = imread("/home/ravina/Desktop/OpenCV/ObjectDetection/sample/img2P3.png");
    //image = imread("/home/ravina/Desktop/OpenCV/ObjectDetection/Pics/originalT1H1B1.png");

    if (!image.data)
    {
        printf("No image data \n");
        return -1;
    }

    //printf("rows=%d, cols=%d\n", image.rows, image.cols);

    ///*
    namedWindow("Display Image", WINDOW_AUTOSIZE );
    imshow("Target Image", image);
    waitKey(0);
    //*/

    cv::Mat gray_mat(image.size(), CV_8U);
    cv::cvtColor(image, gray_mat, cv::COLOR_RGB2GRAY);

    Mat image_th;
    cv::Mat bin_mat(gray_mat.size(), gray_mat.type());
  //  cv::adaptiveThreshold(gray_mat, image_th, 255,
	//		  cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 3, 5);

    ///*
    //frame.copyTo(temp);
    image_th = Mat::zeros(image.rows,image.cols, CV_8UC1);
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

    namedWindow("Display Image", WINDOW_AUTOSIZE );
    imshow("Clean Binary Image", image_th);
    waitKey(0);
    //*/
    
    Mat labels;
    Mat stats;
    Mat centroids;
    cv::connectedComponentsWithStats(image_th, labels, stats, centroids);
    
    //std::cout << labels << std::endl;
    std::cout << "stats.size()=" << stats.size() << std::endl;
    //std::cout << centroids << std::endl;
    
    for(int i=0; i<stats.rows; i++)
    {
      int x = stats.at<int>(Point(0, i));
      int y = stats.at<int>(Point(1, i));
      int w = stats.at<int>(Point(2, i));
      int h = stats.at<int>(Point(3, i));
      
      std::cout << "x=" << x << " y=" << y << " w=" << w << " h=" << h << std::endl;
      
      Scalar color(255,0,0);
      Rect rect(x,y,w,h);
      cv::rectangle(image_th, rect, color);
    }
    
    ///*
    namedWindow("Display Image", WINDOW_AUTOSIZE );
    imshow("Segmenation", image_th);
    //waitKey(0);
    char key = cv::waitKey(0);
    while (1)
    {
      if(key == 'q'){
        break;
      }
    }
    
    

    return 0;
}