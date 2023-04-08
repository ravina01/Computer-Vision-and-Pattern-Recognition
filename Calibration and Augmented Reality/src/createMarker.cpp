/*
Ravina Lad
Date : 26th March 2022
CS 5330 Computer Vision
Spring 2021

This CPP file includes
- Task Extension 2 : This file is part of extension 2 that is Augmented Reality television,
                     this file generates and store first 4 aurco markers from dictionary : DICT_6X6_250
                     later these markers are used to project any video or frames on markers.
*/
#include <opencv2/highgui.hpp>
#include <opencv2/aruco.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char *argv[]) {

    int markerId = 0;
    int borderBits = 1;
    int markerSize = 400;
    bool showImage = true;

    Ptr<aruco::Dictionary> dictionary = aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME(cv::aruco::DICT_6X6_250));
 
    Mat markerImg;
    aruco::drawMarker(dictionary, markerId, markerSize, markerImg, borderBits);

    if(showImage) {
        imshow("marker", markerImg);
        waitKey(0);
    }

    imwrite("/home/ravina/Desktop/OpenCV/Project4/sample/MyImage.jpg", markerImg);

    return 0;
}
