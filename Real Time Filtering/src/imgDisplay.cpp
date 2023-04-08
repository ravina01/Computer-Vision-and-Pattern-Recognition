#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include "curses.h"


using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    
    cv::Mat src; 	
    src = cv::imread("/home/ravina/sample/girl.jpg", cv::IMREAD_COLOR);

    if (src.empty()) 
    {
        cout << "Could not load the image" << endl;
        return -1;
    }

    String windowName = "Enter q to quit \t ";
    
    namedWindow("windowName");
    imshow(windowName, src);
    

    char key = cv::waitKey(0);
    while (1)
    {
        if(key == 'q'){
            break;
        }
    }
}