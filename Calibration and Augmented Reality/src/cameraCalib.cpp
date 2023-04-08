/*
Sequence Number : 1
Ravina Lad
Date : 26th March 2022
CS 5330 Computer Vision
Spring 2021

This CPP file includes

- Task 1 : Detect and Extract Chessboard Corners
- Task 2 : Select Calibration Images
- Task 3 : Calibrate the Camera
- Task 7 : Detect Robust Features : harrisCorner 
- Extensions 4 : implemented SIFT (Scale invariant Feature Transform) = Feature Detector and Descriptor
                and shiTomasi = feature detector that is corner

*/
#include <opencv2/opencv.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/core.hpp"
#include <opencv2/xfeatures2d.hpp>
#include "filter.h"
#include <iostream>

using namespace cv;
using namespace std;

int CHECKERBOARD[2]{6,9}; //Size(9,6)

//This function stores configuartion yaml file and saved camera matrix and distortion coefficients
int storeCameraConfig(cv::Mat &cameraMatrix, cv::Mat &distCoeffs){

    cv::FileStorage fs("/home/ravina/Desktop/OpenCV/Project4/sample/CameraConfig.yaml", cv::FileStorage::WRITE);
    fs << "cameraMat" << cameraMatrix;
    fs << "distCoeff" << distCoeffs;
    fs.release();
    return 0;
}

//This function reads configuartion yaml file and saved camera matrix and distortion coefficients
int readCameraConfig(cv::Mat &cameraMatrix, cv::Mat &distCoeffs){

    cv::FileStorage fs("/home/ravina/Desktop/OpenCV/Project4/sample/CameraConfig.yaml", cv::FileStorage::READ);
    fs ["cameraMat"] >> cameraMatrix;
    fs ["distCoeff"] >> distCoeffs;
    std::cout<<"Camera Matrix "<<cameraMatrix << endl;
    std::cout<<"Distoration Coeff "<<distCoeffs << endl;
    fs.release();
    return 0;
}

int main(int argc, char *argv[]) {
        cv::VideoCapture *capdev;

        // open the video device
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

        cv::Mat frame,gray;
        
        bool success;
        bool flag = false;
        bool found = false;
        string img_filter = "PREVIEW";

        // Creating vector to store vectors of 3D points for each checkerboard image
        std::vector<std::vector<cv::Point3f> > point_list; //objpoints

        // Creating vector to store vectors of 2D points for each checkerboard image
        std::vector<std::vector<cv::Point2f> > corner_list; //imgpoints

        // vector to store the pixel coordinates of detected checker board corners 
        std::vector<cv::Point2f> corner_pts;

        // Defining the world coordinates for 3D points
        std::vector<cv::Point3f> point_set;
        float BoardBoxSize = 7;
        for(int i{0}; i<CHECKERBOARD[1]; i++)
        {
            for(int j{0}; j<CHECKERBOARD[0]; j++){
                point_set.push_back(cv::Point3f(j*BoardBoxSize,i*BoardBoxSize,0));
            }
            
        }
 
        for(;;) {
                *capdev >> frame; 
                
                if( frame.empty() ) {
                  printf("frame is empty\n");
                  break;
                }
                if (img_filter == "PREVIEW")
                    frame = frame;
                else if (img_filter == "SELECT CALIBRATION"){

                    cv::cvtColor(frame,gray,cv::COLOR_RGB2GRAY);
                    success = cv::findChessboardCorners(gray, cv::Size(CHECKERBOARD[0], CHECKERBOARD[1]), corner_pts, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_FAST_CHECK | CALIB_CB_NORMALIZE_IMAGE);
                    if(success)
                    {
                        cv::TermCriteria criteria(cv::TermCriteria::EPS | cv::TermCriteria::MAX_ITER, 30, 0.001);
                        
                        // refining pixel coordinates for given 2d points.
                        cv::cornerSubPix(gray,corner_pts,cv::Size(11,11), cv::Size(-1,-1),criteria);
                        
                        // Displaying the detected corner points on the checker board
                        cv::drawChessboardCorners(frame, cv::Size(CHECKERBOARD[0], CHECKERBOARD[1]), corner_pts, success);

                        //printing out : first corners points and cordinates
                        cout<<"First corner Coordinates = "<<corner_pts[0].x<<" "<<corner_pts[0].y<<endl;

                        if(flag){
                            point_list.push_back(point_set);
                            corner_list.push_back(corner_pts);
                            flag = false;
                        }
                        
                    }
                }
                else if(img_filter == "CAMERA CALIBRATION"){
                    
                    //cameraMatrix = Mat::eye(3, 3, CV_64F);
                    cv::Mat cameraMatrix,distCoeffs,R,T;

                    //float data [3][3] = {{1,0,(frame.cols/2)},{0,1,(frame.rows/2)},{0,0,1}};
                    //cameraMatrix = Mat(3, 3, CV_64FC1, &data);
                    //distCoeffs = Mat::zeros(8, 1, CV_64FC1);

                    int flags = cv::CALIB_FIX_ASPECT_RATIO + cv:: CALIB_ZERO_TANGENT_DIST + cv::CALIB_FIX_PRINCIPAL_POINT + cv::CALIB_FIX_K3;
                    /*
                      Performing camera calibration by passing the value of known 3D points (point_list) and corresponding pixel coordinates of the 
                      detected corners (corner_list)
                    */
                    float error = cv::calibrateCamera(point_list, corner_list, cv::Size(frame.rows,frame.cols), cameraMatrix, distCoeffs, R, T, flags);

                    std::cout<< "Calibrating : " <<endl;
                    std::cout << "cameraMatrix : " << cameraMatrix << std::endl;
                    std::cout << "distCoeffs : " << distCoeffs << std::endl;
                    std::cout << "Rotation vector : " << R << std::endl;
                    std::cout << "Translation vector : " << T << std::endl;
                    std::cout << "Reprojection Calibration error : " << error << std::endl;

                    //stores cameraMatrix and distortion coefficients in yaml file
                    storeCameraConfig(cameraMatrix,distCoeffs);
                } 
                else if(img_filter == "HARRISCORNER"){
                    cv::Mat result;
                    harrisCornerDetector(frame, result);
                }
                else if(img_filter == "SHITOMASI"){
                    cv::Mat result;
                    shiTomasiCornerDetector(frame, result);
                }
                else if(img_filter == "SIFT"){
                    Mat GrayFrame;
                    cvtColor(frame, GrayFrame, cv::COLOR_RGB2GRAY);
                    cv::Ptr<cv::SiftFeatureDetector> detector = cv::SiftFeatureDetector::create();
                    std::vector<cv::KeyPoint> keypoints;

                    detector->detect(GrayFrame, keypoints);
                    cv::drawKeypoints(GrayFrame, keypoints, frame);
                }
                
                cv::imshow("Video", frame);
                char key = cv::waitKey(10);

                switch (key)
                {
                    case 's':
                        {
                            bool isSuccess = imwrite("/home/ravina/sample/MyImage.jpg", frame);
                            if (isSuccess == false)
                            {
                                cout << "Failed to save the image" << endl;
                                return -1;
                            }
                        }
                    case 'c':
                        {
                            img_filter = "SELECT CALIBRATION";
                            flag = true;                  
                            continue;
                        }
                    case 'm':
                        {
                            img_filter = "CAMERA CALIBRATION";                    
                            continue;
                        }
                    case 'h':
                        {
                            img_filter = "HARRISCORNER";                    
                            continue;
                        }
                    case 't':
                        {
                            img_filter = "SHITOMASI";                    
                            continue;
                        }
                    case 'f':
                        {
                            img_filter = "SIFT";                    
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