#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include "filter.h"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    
    cv::Mat src,dst; 	
    src = cv::imread("/home/ravina/Desktop/OpenCV/Project4/sample/assets/checkerboard.png", cv::IMREAD_COLOR);

    if (src.empty()) 
    {
        cout << "Could not load the image" << endl;
        return -1;
    }

    String windowName = "Enter q to quit \t ";
    
    
   // imshow(windowName, src);
    harrisCornerDetector(src,dst);
    //shiTomasiCornerDetector(src,dst);
    imshow("Harris Detector", src);

    char key = cv::waitKey(0);
    while (1)
    {
        if(key == 'q'){
            break;
        }
    }
}