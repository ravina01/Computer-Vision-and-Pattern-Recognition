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

    src = cv::imread("/home/ravina/Desktop/OpenCV/ObjectDetection/sample/pics/img2P3.png", cv::IMREAD_COLOR);

    if (src.empty()) 
    {
        cout << "Could not load the image" << endl;
        return -1;
    }

    String windowName = "Enter q to quit \t ";
    
    
    imshow("SRC", src);


        Mat image_th;
        cv::Mat gray_mat(src.size(), CV_8U);

        image_th = Mat::zeros(src.rows,src.cols, CV_8UC1);

        cv::cvtColor(src, gray_mat, cv::COLOR_RGB2GRAY);
        //greyscale(src,gray_mat);
        thresholdingGray(gray_mat, image_th);
        
     

        grassfireTransform(image_th, image_th);
  
    

    imshow("Target", image_th);

    char key = cv::waitKey(0);
    while (1)
    {
        if(key == 'q'){
            break;
        }
    }
}