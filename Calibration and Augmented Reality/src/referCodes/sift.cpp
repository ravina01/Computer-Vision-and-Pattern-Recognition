#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <iostream>
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

        // get some properties of the image
        cv::Size refS( (int) capdev->get(cv::CAP_PROP_FRAME_WIDTH ),
                       (int) capdev->get(cv::CAP_PROP_FRAME_HEIGHT));
        printf("Expected size: %d %d\n", refS.width, refS.height);

        cv::namedWindow("Video", 1);

        cv::Mat frame,GrayFrame;

        for(;;) {
            *capdev >> frame; 
            
            if( frame.empty() ) {
                printf("frame is empty\n");
                break;
            }

            cvtColor(frame, GrayFrame, cv::COLOR_RGB2GRAY);
            cv::Ptr<cv::SiftFeatureDetector> detector = cv::SiftFeatureDetector::create();
            std::vector<cv::KeyPoint> keypoints;
            //auto detector = cv::xfeatures2d::SiftFeatureDetector::create();

            detector->detect(GrayFrame, keypoints);
            cv::Mat output;
            cv::drawKeypoints(GrayFrame, keypoints, output);
            cv::imshow("Video", output); 

            char key = cv::waitKey(0);
            while (1)
            {
                if(key == 'q'){
                    break;
                }
            }       
        }

    return 0;
}