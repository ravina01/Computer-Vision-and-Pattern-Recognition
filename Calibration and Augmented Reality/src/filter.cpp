/*
Ravina Lad
Date : 26th March 2022
CS 5330 Computer Vision
Spring 2021

This CPP file includes
- This is utility file used for mutilpe purpose, it has functions which are used through this project
*/

#include <iostream>
#include <cmath>
#include "filter.h"
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include <opencv2/aruco.hpp>

using namespace std;
using namespace cv;

//Calculates a perspective transform from four pairs of the corresponding points.
void PerspectiveTransform(cv::Mat Frame, std::vector<cv::Point2f> Coordinates, cv::Mat& TransformedFrame)
{
    int Height = Frame.rows, Width = Frame.cols;

    cv::Point2f InitialPoints[4], FinalPoints[4];
    InitialPoints[0] = cv::Point2f(0, 0);
    InitialPoints[1] = cv::Point2f(Width - 1, 0);
    InitialPoints[2] = cv::Point2f(0, Height - 1);
    InitialPoints[3] = cv::Point2f(Width - 1, Height - 1);

    FinalPoints[0] = cv::Point2f(Coordinates[0].x, Coordinates[0].y);
    FinalPoints[1] = cv::Point2f(Coordinates[1].x, Coordinates[1].y);
    FinalPoints[2] = cv::Point2f(Coordinates[3].x, Coordinates[3].y);
    FinalPoints[3] = cv::Point2f(Coordinates[2].x, Coordinates[2].y);
    
    cv::Mat ProjectiveMatrix(2, 4, CV_32FC1);
    ProjectiveMatrix = cv::Mat::zeros(Height, Width, Frame.type());
    ProjectiveMatrix = cv::getPerspectiveTransform(InitialPoints, FinalPoints);

    cv::warpPerspective(Frame, TransformedFrame, ProjectiveMatrix, TransformedFrame.size());
}

/* 
    This function will overlap the given frame onto a frame containing aruco markers wrt the given vertices of the face of the 
    rectangle on which we have to overlap the projected Video frame.
*/
void FramesOverlapping(cv::Mat BaseFrame, cv::Mat SecFrame, std::vector<cv::Point2f> BoxCoordinates, cv::Mat& OverlapedFrame)
{
    // Finding transformed image
    cv::Mat TransformedFrame = BaseFrame.clone();
    PerspectiveTransform(SecFrame, BoxCoordinates, TransformedFrame);


    // Overlaping frames
    cv::Mat SecFrame_Mask = cv::Mat::zeros(cv::Size(BaseFrame.cols, BaseFrame.rows), BaseFrame.type());
    std::vector<cv::Point> BoxCoordinates_Converted;
    for (std::size_t i = 0; i < BoxCoordinates.size(); i++)
        BoxCoordinates_Converted.push_back(cv::Point(BoxCoordinates[i].x, BoxCoordinates[i].y));
    
    //This function masks aruco Video frame
    cv::fillConvexPoly(SecFrame_Mask, BoxCoordinates_Converted, cv::Scalar(255, 255, 255));

    cv::Mat SecFrame_Mask_not;
    //Bitwise not performed on masked aruco frames and bitwise not + and + or gives us overlapped frames
    cv::bitwise_not(SecFrame_Mask, SecFrame_Mask_not);
    cv::bitwise_and(BaseFrame, SecFrame_Mask_not, BaseFrame);
    OverlapedFrame = BaseFrame.clone();
    cv::bitwise_or(BaseFrame, TransformedFrame, OverlapedFrame);

}

//Function to detect Harris Corners
int harrisCornerDetector( cv::Mat &src, cv::Mat &dst ){

    Mat gray, output, output_norm;
    cvtColor(src,gray,cv::COLOR_RGB2GRAY);

    output = Mat::zeros(src.size(), CV_32FC1);
    cornerHarris(gray, output, 3, 3, 0.04);
    normalize(output, output_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
    convertScaleAbs(output_norm, dst);

    //draw circles around the corners
    for(int j = 0; j < output_norm.rows; j++){
        for(int i = 0; i < output_norm.cols; i++){
            if((int)output_norm.at<float>(j,i) > 100){
                circle(src, Point(i,j), 4, Scalar(0,0,255), 2, 8, 0);
            }
        }
    }
    //cv::resize(src,src,cv::Size(), 1.5, 1.5);
    return 0;
}

//Function to detect shiTomasiCorners
int shiTomasiCornerDetector( cv::Mat &src, cv::Mat &dst ){

    Mat gray;
    cvtColor(src,gray,cv::COLOR_RGB2GRAY);

    vector<Point2f> corners;
    goodFeaturesToTrack(gray, corners, 70, 0.01, 10, Mat(), 3, false, 0.04);

    //draw circles around the corners
    for(int i = 0; i< corners.size(); i++){
        circle(src, corners[i], 4, Scalar(0,255,0), 2, 8, 0);
    } 

    return 0;
}


//This function will detect Aruco markers and find checkerboard co-ordinates
bool arucoDetector(cv::Mat Frame, std::vector<cv::Point2f>& box_Coordinates)
{
    
    cv::Mat gray;
    cv::cvtColor(Frame, gray, cv::COLOR_RGB2GRAY);
    
    cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);

    std::vector<int> IDs;
    std::vector<std::vector<cv::Point2f>> Corners, RejectedCandidates;
    cv::Ptr<cv::aruco::DetectorParameters> Parameters = cv::aruco::DetectorParameters::create();
    cv::aruco::detectMarkers(Frame, dictionary, Corners, IDs, Parameters, RejectedCandidates);

    // checking is 4 aurco markers detected or not
    if (IDs.size() != 4)
        return false;


    //here will store checkerboard coordinates in vector
    for(int i = 0 ; i < 4 ; i++)
        box_Coordinates.push_back(Corners[find(IDs.begin(), IDs.end(), i) - IDs.begin()][i]);

    return true;
}