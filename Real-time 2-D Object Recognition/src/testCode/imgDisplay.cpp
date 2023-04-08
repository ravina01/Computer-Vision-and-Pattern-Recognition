#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include "curses.h"
#include "filter.h"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    
    cv::Mat src; 	
    cv::Mat temp,result;
        cv::Mat temp_gray;

    src = cv::imread("/home/ravina/Desktop/OpenCV/ObjectDetection/sample/img2P3.png", cv::IMREAD_COLOR);

    if (src.empty()) 
    {
        cout << "Could not load the image" << endl;
        return -1;
    }

    String windowName = "Enter q to quit \t ";
    
    
    imshow("SRC", src);

    src.copyTo(temp);
    cvtColor(src, temp, cv::COLOR_RGB2GRAY);
    temp_gray = Mat::zeros(src.rows,src.cols, CV_8UC1);
    result = Mat::zeros(src.rows,src.cols, CV_8UC1);
                    
    thresholdingGray(temp, temp_gray);
    imshow("Threshold", temp_gray);


    grassfireTransform(temp_gray, result);
    shrinkingImage(result, result);
    growingOperations(result, result);

    grassfireTransform(result, result);
    shrinkingImage(result, result);
    growingOperations(result, result);

    grassfireTransform(result, result);
    shrinkingImage(result, result);

    growingOperations(result, result);
    growingOperations(result, result);
    grassfireTransform(result, result);
    shrinkingImage(result, result);

    imshow("Target", result);

    char key = cv::waitKey(0);
    while (1)
    {
        if(key == 'q'){
            break;
        }
    }
}