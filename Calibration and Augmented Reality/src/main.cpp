/*
Sequence Number : 2
Ravina Lad
Date : 26th March 2022
CS 5330 Computer Vision
Spring 2021

This CPP file includes

- Task 4 : Calculate Current Position of the Camera
- Task 5 : Project Outside Corners or 3D Axes
- Task 6 : Create a Virtual Object : I have drawn Cube on checkerboard

*/

#include <opencv2/opencv.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/core.hpp"
#include "filter.h"
#include <iostream>

using namespace cv;
using namespace std;

const string z = "Z";
const string y = "Y";
const string x = "X";

//This function reads camera configuartion file and extracts saved camera matrix and Distortion coefficients
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

    cv::Mat camera_matrix,dist_coeffs;
    readCameraConfig(camera_matrix, dist_coeffs);

	vector<Point2f>corner_pts;
	Mat rvecs, tvecs;

	VideoCapture capture(0);
	Mat frame, gray;
	
	vector<Point3f>  objectPoints;
	bool found = false;
	vector<Point3d> imagePoints3D;
    vector<Point2d> imagePoints;

    //width of checkerboard 
	int width=9;
    //height of checkerboard 
	int height=6;
    //size of checkerboard
	Size patSize(width,height);
    //distance between 2 corners in mm
	float boxSize = 4;
	for (int j=0; j<patSize.height;j++)
	{
		for( int i=0; i < patSize.width;i++)
	    {
		    objectPoints.push_back(Point3f(i*boxSize,j*boxSize,0));
        }
	}
 
	/*
    below are the 3d object pointthat is world point for box drawing
    */
	imagePoints3D.push_back(Point3d(0,0,-10.0)); 
    imagePoints3D.push_back(Point3d(10.0,0,0));  
	imagePoints3D.push_back(Point3d(0,10.0,0));
    imagePoints3D.push_back(Point3d(12, 0,-12.0));
    imagePoints3D.push_back(Point3d(12, 8,-12.0));
	imagePoints3D.push_back(Point3d(20, 8,-12.0));
	imagePoints3D.push_back(Point3d(20, 0,-12.0));

	while(1)
	{
		capture >> frame;

		
        if(frame.empty()!=1)
        {	
            found=findChessboardCorners(frame, patSize,  corner_pts);//This will detect pattern
        }  

        if(found )

        {
            cvtColor(frame,gray,cv::COLOR_RGB2GRAY);
            cornerSubPix(gray, corner_pts, Size(11, 11), Size(-1, -1),
            TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 30, 0.1));
        
            //Gives you rvecs, tvecs
            cv::solvePnP(objectPoints, corner_pts, camera_matrix, dist_coeffs, rvecs, tvecs);
            
            //point2d from imagePoints3D world point to drow them on 2d image.
            cv::projectPoints(imagePoints3D, rvecs, tvecs, camera_matrix, dist_coeffs, imagePoints);
            
            std::cout << "Rotation vector : " << rvecs << std::endl;
            std::cout << "Translation vector : " << tvecs << std::endl;

            // Lets draw cubes and 3D axis points.

            //z-axis
            cv::line(frame,corner_pts[0], imagePoints[0], cv::Scalar(0,0,255), 3);
            //y-axis
            cv::line(frame,corner_pts[0], imagePoints[1], cv::Scalar(255,0,0), 3);
            //x-axis
            cv::line(frame,corner_pts[0], imagePoints[2], cv::Scalar(0,255,0), 3);
        
        
            putText(frame, x, Point(imagePoints[1].x-10,imagePoints[1].y-10),FONT_HERSHEY_COMPLEX_SMALL,1, cv::Scalar(255,0,0),2 );
            putText(frame, y, Point(imagePoints[2].x-10,imagePoints[2].y-10),FONT_HERSHEY_COMPLEX_SMALL,1, cv::Scalar(0,255,0),2 );
            putText(frame, z, Point(imagePoints[0].x-10,imagePoints[0].y-10),FONT_HERSHEY_COMPLEX_SMALL,1, cv::Scalar(0,0,255),2 );
            circle(frame,imagePoints[0], 3, cv::Scalar(0,0,255), 4, 8,0);
            circle(frame,imagePoints[1], 3, cv::Scalar(255,0,0), 4, 8,0);
            circle(frame,imagePoints[2], 3, cv::Scalar(0,255,0), 4, 8,0);
        
            /*
            drawing Cube now.
            we can chnage change corner_pts point and imagePoints point. to draw cube on different corners on checkerboard 
            */

            cv::line(frame,corner_pts[3],  imagePoints[3], cv::Scalar(0,0,255),6);
            cv::line(frame,corner_pts[21], imagePoints[4],cv::Scalar(0,0,255), 6);
            cv::line(frame,corner_pts[23], imagePoints[5],cv::Scalar(0,0,255), 6);
            cv::line(frame,corner_pts[5],  imagePoints[6], cv::Scalar(0,0,255),6);
                                
            cv::line(frame,corner_pts[3],corner_pts[5], cv::Scalar(0,255,0),  6);
            cv::line(frame,corner_pts[5],corner_pts[23],cv::Scalar(0,255,0) ,6);
            cv::line(frame,corner_pts[23],corner_pts[21],cv::Scalar(0,255,0),  6);
            cv::line(frame,corner_pts[21],corner_pts[3],cv::Scalar(0,255,0),   6);
            
            cv::line(frame,imagePoints[3], imagePoints[4], cv::Scalar(255,0,0), 6);
            cv::line(frame,imagePoints[4], imagePoints[5], cv::Scalar(255,0,0), 6);
            cv::line(frame,imagePoints[5], imagePoints[6], cv::Scalar(255,0,0), 6); 
            cv::line(frame,imagePoints[3], imagePoints[6], cv::Scalar(255,0,0), 6);

        }

        // Display image.
        cv::imshow("Output", frame);
        cv::waitKey(1);
	
    }

}