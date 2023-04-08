/*
Sequence Number : 3
Ravina Lad
Date : 26th March 2022
CS 5330 Computer Vision
Spring 2021

This CPP file includes
- Task Extension 1 : Implemented : Multi-object detection on aruco markers
                     and prints their respective ids and draws boxes around each marker
                     
*/
#include <opencv2/aruco.hpp>
#include <iostream>
#include "opencv2/core.hpp"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int readCameraConfig(cv::Mat &cameraMatrix, cv::Mat &distCoeffs){

    cv::FileStorage fs("/home/ravina/Desktop/OpenCV/Project4/sample/CameraConfig.yaml", cv::FileStorage::READ);
    fs ["cameraMat"] >> cameraMatrix;
    fs ["distCoeff"] >> distCoeffs;
    std::cout<<"Camera Matrix "<<cameraMatrix << endl;
    std::cout<<"Distoration Coeff "<<distCoeffs << endl;
    fs.release();
    return 0;
}

int main(int argc, char **argv)
{
    cv::VideoCapture *capdev;

    capdev = new cv::VideoCapture(0);
    if( !capdev->isOpened() ) {
        printf("Unable to open video device\n");
        return(-1);
    }

    // get some properties of the image
    cv::Size refS( (int) capdev->get(cv::CAP_PROP_FRAME_WIDTH ),
            (int) capdev->get(cv::CAP_PROP_FRAME_HEIGHT));
    printf("Expected size: %d %d\n", refS.width, refS.height);

    cv::namedWindow("Video", 1);

    cv::Mat cameraMatrix, distCoeffs,frame;
        
    readCameraConfig(cameraMatrix, distCoeffs);
    cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);

    //to create grid boxes around detected markers
    cv::Ptr<cv::aruco::GridBoard> board = cv::aruco::GridBoard::create(5, 7, 0.04, 0.01, dictionary);

    
    for(;;) {
        *capdev >> frame; 
                
        if( frame.empty() ) {
            printf("frame is empty\n");
            break;
        }

        std::vector<int> ids;
        std::vector<std::vector<cv::Point2f> > corners;
        cv::aruco::detectMarkers(frame, dictionary, corners, ids);
        if (ids.size() > 0) {
            cv::aruco::drawDetectedMarkers(frame, corners, ids);
            cv::Vec3d rvec, tvec;
            int valid = estimatePoseBoard(corners, ids, board, cameraMatrix, distCoeffs, rvec, tvec);
            // if at least one board marker detected
            if(valid > 0)
                cv::drawFrameAxes(frame, cameraMatrix, distCoeffs, rvec, tvec, 0.1);
        }
        cv::imshow("Video", frame);
        cv::waitKey(1);
  
    }
}