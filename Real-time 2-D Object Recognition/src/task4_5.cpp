/*
Ravina Lad
Date : 6th March 2022
CS 5330 Computer Vision
Spring 2021

CPP file for calculating feature vectors for different objects
- Task 4 and 5 : compute features and store them in mydata.csv file
- feature vector includes : Humoments, Height:width ratio, percentage filled ratio
- Training of the image is done on Still images not on Live feed
- Go on changing the path of images and storing relative label for the same.
*/

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <cmath>
#include "filter.h"
#include "csv_util.h"

using namespace cv;
using namespace std;

Mat src; 

/** @function main */
int main( int argc, char** argv )
{
    // Load source image and convert it to gray
   // src = imread("/home/ravina/Desktop/OpenCV/ObjectDetection/sample/assets/scissor2.jpeg");
    src = cv::imread("/home/ravina/Desktop/OpenCV/ObjectDetection/sample/pics/img2P3.png", cv::IMREAD_COLOR);
    cv::resize(src, src, cv::Size(666, 492));
    imshow( "SRC", src );
    

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    Mat image_th(src.size(), CV_8UC3);
    segmenetation(src,image_th);
    imshow("segmentated img", image_th);

    /// Find contours
    findContours( image_th, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );
   
    vector<double[7]> huMoments(contours.size() );
    vector<vector<float> > finalFeature(contours.size());
    vector<RotatedRect> minRect( contours.size() );
    vector<Moments> mu(contours.size() );
    vector<Point2f> mc( contours.size() );
    vector<double> asepectRatio(contours.size());
    vector<double> percentageRatio(contours.size());
    vector<double> central_angle(contours.size()) ;
    vector<Point2f> centralAngleStart(contours.size());
    vector<Point2f> centralAngleEnd(contours.size());
    vector<float> contourAreas(contours.size());
    char filename[355] = "/home/ravina/Desktop/OpenCV/ObjectDetection/sample/knn.csv";

    for( size_t i = 0; i < contours.size(); i++ ){

      //Minimum Rectangle 
      minRect[i] = minAreaRect( contours[i] );

      //Moments calculation
      mu[i] = moments( contours[i] );

      //HuMomments calculation
      HuMoments(mu[i], (huMoments[i]));
      
      float area = contourArea(contours[i]);

      contourAreas[i] = (area);

      for ( int j = 0; j < 6; j++ )
      {
        huMoments[i][j] = log(abs(huMoments[i][j]));
                
      }

    }
    float maxArea = *max_element(contourAreas.begin(),contourAreas.end());

    //centroid Calculation
    for( int i = 0; i < contours.size(); i++ )
    {
        //add 1e-5 to avoid division by zero
        mc[i] = Point2f( static_cast<float>(mu[i].m10 / (mu[i].m00 + 1e-5)),
                         static_cast<float>(mu[i].m01 / (mu[i].m00 + 1e-5)) );
        cout << "mc[" << i << "]=" << mc[i] << endl;
    }

    Mat drawing = Mat::zeros( image_th.size(), CV_8UC3 );
    for( int i = 0; i < contours.size(); i++ )
    {
        central_angle[i] = 0.5*atan2(2*mu[i].mu11,(mu[i].mu20-mu[i].mu02));
        cout<<"Central angle"<<central_angle[i]<<endl;
        centralAngleStart[i] = Point2f ( mc[i].x + 200 * cos(central_angle[i]) , mc[i].y + 200 * sin(central_angle[i]) );
        centralAngleEnd[i] = Point2f ( mc[i].x - 200 * cos(central_angle[i]) , mc[i].y - 200 * sin(central_angle[i]) );
        
    }

    for( int i = 0; i< contours.size(); i++ )
    {
      
      if(maxArea == contourArea(contours[i])){

        // use a random color:     Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        Scalar color1 = Scalar( 0, 0, 240 );
        Scalar color2 = Scalar( 0, 230, 240 );
        Scalar color3 = Scalar( 200, 120, 120 );

        // draw contours:     
        drawContours( drawing, contours, i, color1, 1, 8, vector<Vec4i>(), 0, Point() );

        // draw rectangle:    
        //rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color2, 2, 8, 0 );
          Point2f rect_points[4];
          minRect[i].points( rect_points );
          for ( int j = 0; j < 4; j++ )
          {
              line( image_th, rect_points[j], rect_points[(j+1)%4], (255) );
              
          }
          
          double width =  sqrt((rect_points[0].x - rect_points[1].x) * (rect_points[0].x - rect_points[1].x) + (rect_points[0].y - rect_points[1].y) * (rect_points[0].y - rect_points[1].y));
          double height =  sqrt((rect_points[1].x - rect_points[2].x) * (rect_points[1].x - rect_points[2].x) + (rect_points[1].y - rect_points[2].y) * (rect_points[1].y - rect_points[2].y));

          //calculate width:height ratio
          asepectRatio[i] = width > height ? height/width : width/height;
          
          //calculate percent ratio
          percentageRatio[i] =  (contourArea(contours[i]) * 100 )/ (width * height);

          //draw circle at centroid
          circle( drawing, mc[i], 4, color1, -1 );

          //draw line at central axis
          line(drawing, centralAngleStart[i], centralAngleEnd[i], color3, 2 );

          finalFeature[i].push_back(asepectRatio[i]);
          finalFeature[i].push_back(percentageRatio[i]);
          for ( int j = 0; j < 6; j++ )
            {
              finalFeature[i].push_back(huMoments[i][j]);
                
            }
   
          char image_filename[255] = "scissor";
          append_image_data_csv(filename, image_filename, finalFeature[i], 0);

      }
        

    }

    for(int i=0; i<finalFeature.size(); i++){
      for ( int j = 0; j < finalFeature[i].size(); j++)
        {
           cout<<" "<<finalFeature[i][j];    
        }
    }
  
  /// print some clear lines to command line to seperate output from different runs
  cout << "\n \n" << flush;
  imshow( "Contours", drawing );
  imshow("Final", image_th);
  

  waitKey(0);
  return(0);
}
