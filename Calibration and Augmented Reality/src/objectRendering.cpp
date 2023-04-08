/*
Sequence Number : 5
Ravina Lad
Date : 26th March 2022
CS 5330 Computer Vision
Spring 2021

This CPP file includes
- Task Extension 3 : Implemented : Render .obj File and display on checkerboard
                     This render .obj file extracts its vertices, normals and face and draws triangle on each face
                     As a result, you see collection on such faces displayed on checkerboard

                    Objects like : violin case, cessna, airboat and teapot
                     
*/

#include <opencv2/opencv.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/core.hpp"
#include "ObjRender.h"
#include <iostream>

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

    string path = cv::samples::findFile("/home/ravina/Desktop/OpenCV/Project4/sample/objFiles/humanoid_tri.obj");
    cv::Mat camera_matrix,dist_coeffs;
    readCameraConfig(camera_matrix, dist_coeffs);

	vector<Point2f>corner_pts;
	Mat rvecs, tvecs;

	VideoCapture capture(0);
	Mat frame, gray;
	
	vector<Point3f>  objectPoints;
	bool found = false;
	vector<Point3d> imagePoints3D;

    //width of checkerboard 
	int width=9;
    //height of checkerboard 
	int height=6;
    //size of checkerboard
	Size patSize(width,height);
    //distance between 2 corners in mm
	float boxSize = 0.18;
	for (int j=0; j< patSize.height; j++)
	{
		for( int i=0; i < patSize.width; i++)
	    {
		    objectPoints.push_back(Point3f((float)i*boxSize,(float)j*boxSize,0));
        }
	}
    
    ObjRender obj1;
    obj1.renderFile(path);
    
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

            for(int i=0;i<obj1.faceVert.size();i++){
                vector<int> temp_face;

                for(int j=0;j<obj1.faceVert[i].size();j++){

                    //cout<<obj1.faceVert[i][j]<<" ";
                    temp_face.push_back(obj1.faceVert[i][j]);
                }

                vector<Point3f> imagePoints3f;
                for(int x=0;x<temp_face.size();x++){
                    int index = temp_face[x];
                    imagePoints3f.push_back(obj1.vertices[index]);
                }
                vector<Point2f> imagePoints;
                cv::projectPoints(imagePoints3f, rvecs, tvecs, camera_matrix, dist_coeffs, imagePoints);
                cv::line(frame,imagePoints[0], imagePoints[1], cv::Scalar(0,255,0), 3, LINE_AA);
                cv::line(frame,imagePoints[0], imagePoints[2], cv::Scalar(0,255,0), 3, LINE_AA);
                cv::line(frame,imagePoints[1], imagePoints[2], cv::Scalar(0,255,0), 3, LINE_AA); 
            }
            
        }

        // Display image.
        cv::imshow("Output", frame);
        cv::waitKey(1);
	
    }

}
