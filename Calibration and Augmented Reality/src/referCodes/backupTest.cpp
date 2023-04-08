#include <opencv2/opencv.hpp>
#include<opencv2/highgui/highgui.hpp>
#include "filter.h"
#include <iostream>

using namespace cv;
using namespace std;

int CHECKERBOARD[2]{6,9}; //Size(9,6)

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
        // vector to store the pixel coordinates of detected checker board corners 
        std::vector<cv::Point2f> corner_pts;
        bool success;
        string img_filter = "PREVIEW";
        
        for(;;) {
                *capdev >> frame; 
                
                if( frame.empty() ) {
                  printf("frame is empty\n");
                  break;
                }
                 
                cv::cvtColor(frame,gray,cv::COLOR_RGB2GRAY);
                success = cv::findChessboardCorners(gray, cv::Size(CHECKERBOARD[0], CHECKERBOARD[1]), corner_pts, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_FAST_CHECK | CALIB_CB_NORMALIZE_IMAGE);
                if(success)
                {
                    cv::TermCriteria criteria(cv::TermCriteria::EPS | cv::TermCriteria::MAX_ITER, 30, 0.001);
                    
                    // refining pixel coordinates for given 2d points.
                    cv::cornerSubPix(gray,corner_pts,cv::Size(11,11), cv::Size(-1,-1),criteria);
                    
                    // Displaying the detected corner points on the checker board
                    cv::drawChessboardCorners(frame, cv::Size(CHECKERBOARD[0], CHECKERBOARD[1]), corner_pts, success);

                    //printing out : corners points and cordinates
                    cout<<"Number of corners = "<<corner_pts.size();
                    for(int i= 0; i< corner_pts.size(); i++){
                        cout<<"Coordinates = "<<corner_pts[i].x<<" "<<corner_pts[i].y<<endl;
                    }
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
                    break;
                    case 'c':
                        {
                            img_filter = "SELECT CALIBRATION";                    
                            continue;
                        }
                    break;
                    
                    
                    case 'q':
                        delete capdev;
                        break;
                    default:
                        continue;
                }    
        }
        return(0);
}