
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include "filter.h"

using namespace cv;
using namespace std;

Mat src; Mat src_gray;
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);


/// Function header
void thresh_callback(int, void* );

/** @function main */
int main( int argc, char** argv )
{

    cv::VideoCapture *capdev;

        // open the video device
        capdev = new cv::VideoCapture(0);
        if( !capdev->isOpened() ) {
                printf("Unable to open video device\n");
                return(-1);
        }

    cv::Mat src;
    for(;;) {
        
        *capdev >> src; 



        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;


        /// Detect edges using Threshold
            //Mat image_th;
            Mat image_th(src.size(), CV_8UC3);
            segmenetation(src,image_th);
            imshow("segmentated img", image_th);

        /// Find contours
        findContours( image_th, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );

        /// Approximate contours to polygons + get bounding rects and circles
        vector<vector<Point> > contours_poly( contours.size() );
        vector<Rect> boundRect( contours.size() );
        vector<Point2f>center( contours.size() );
        vector<float>radius( contours.size() );

        for( int i = 0; i < contours.size(); i++ )
            { approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
            boundRect[i] = boundingRect( Mat(contours_poly[i]) );
            minEnclosingCircle( contours_poly[i], center[i], radius[i] );
            }


        /// Draw polygonal contour, bonding rects or circles
        Mat drawing = Mat::zeros( image_th.size(), CV_8UC3 );
        for( int i = 0; i< contours.size(); i++ )
            {
            // use a random color:     Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
            Scalar color = Scalar( 0, 0, 240 );

            // draw contours:     
            drawContours( drawing, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
            // draw rectangle:    
            rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );

            // draw circle with dynamic radious:     circle( drawing, center[i], (int)radius[i], color, 2, 8, 0 );
                //circle( drawing, center[i], 2, color, 2, 8, 0 );

            /// print data sctructure to command line to be used in another program etc
            cout << "{id:" << i << ",x:" << center[i].x << ",y:" << center[i].y << "}," << flush;
            }

        /// print some clear lines to command line to seperate output from different runs
        cout << "\n \n" << flush;

        /// Show in a window
        //namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
        imshow( "Contours", drawing );
        

    waitKey(0);

    }
  return(0);
}


/** @function thresh_callback */
void thresh_callback(int, void* )
{
  
}