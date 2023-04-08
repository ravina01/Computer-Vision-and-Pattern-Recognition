/*
Ravina Lad
Date : 6th March 2022
CS 5330 Computer Vision
Spring 2021

This CPP file includes
- Task 1 : thresholding
- Task 2 : cleanup Binary Image
- Task 3 : Segment Binary Image
It shows all task 3 output on video feed.

*/

#include <opencv2/opencv.hpp>
#include<opencv2/highgui/highgui.hpp>
#include "filter.h"
#include <iostream>

using namespace cv;
using namespace std;


int main(int argc, char *argv[]) {
        cv::VideoCapture *capdev;

        // open the video device
        capdev = new cv::VideoCapture(0);
        if( !capdev->isOpened() ) {
                printf("Unable to open video device\n");
                return(-1);
        }
        cv::Size refS( (int) capdev->get(cv::CAP_PROP_FRAME_WIDTH ),
                       (int) capdev->get(cv::CAP_PROP_FRAME_HEIGHT));
        printf("Expected size: %d %d\n", refS.width, refS.height);

        cv::namedWindow("Video", 1);

        cv::Mat frame;
        cv::Mat temp,result;
        cv::Mat temp_gray;
        
        string img_filter = "PREVIEW";
        
        for(;;) {
                *capdev >> frame; 
                
                if( frame.empty() ) {
                  printf("frame is empty\n");
                  break;
                }
                if (img_filter == "PREVIEW")
                    result = frame;
                else if (img_filter == "THRESHOLD_GRAY"){
                    frame.copyTo(temp);
                    result = Mat::zeros(frame.rows,frame.cols, CV_8UC1);
                    //cvtColor(frame, temp, cv::COLOR_RGB2GRAY);
                    greyscale(frame,temp);
                    thresholdingGray(temp, result);
                }
                else if (img_filter == "THRESHOLD_HSV"){
                    frame.copyTo(temp);
                    frame.copyTo(result);
                    cvtColor(frame, result, cv::COLOR_RGB2HSV);
                    blur5x5(temp, temp);
                    result = Mat::zeros(frame.rows,frame.cols, CV_8UC1);
                    thresholdingHSV(temp, result);
                }
                else if (img_filter == "CLEANUPBINARY"){
                    frame.copyTo(temp);
                    cvtColor(frame, temp, cv::COLOR_RGB2GRAY);
                    thresholdingGray(temp, temp);
                    result = Mat::zeros(frame.rows,frame.cols, CV_8UC1);
                    
                    grassfireTransform(temp, result);
                    shrinkingImage(result, result, 2);
                    
                    growingOperations(result, result,2);
                    grassfireTransform(result, result);
                    shrinkingImage(result, result, 1);
                    growingOperations(result, result,1);
                  
                }
                else if (img_filter == "SEGMENT"){
                    frame.copyTo(result);
                    segmenetation(frame,result);
                  
                }

                cv::imshow("Video", result);
                char key = cv::waitKey(10);

                switch (key)
                {
                                       
                    case 'g':
                    {                        
                        img_filter = "THRESHOLD_GRAY";                    
                        continue;
                    } 
                    case 'h':
                    {                        
                        img_filter = "THRESHOLD_HSV";                    
                        continue;
                    }
                    case 'c':
                    {                        
                        img_filter = "CLEANUPBINARY";                    
                        continue;
                    }
                    case 's':
                    {                        
                        img_filter = "SEGMENT";                   
                        continue;
                    }
                    case 'q':
                        delete capdev;
                        break;
                    default:
                        continue;
                }    
        }
        return(0);
}