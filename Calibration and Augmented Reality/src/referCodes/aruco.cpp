// Import the aruco module in OpenCV
#include <opencv2/aruco.hpp>
#include <iostream>
#include "opencv2/core.hpp"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
    cv::Mat markerImage;
    // Load the predefined dictionary
    cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);

    // Generate the marker
    aruco::drawMarker(dictionary, 33, 200, markerImage, 1);
    imwrite("/home/ravina/Desktop/OpenCV/Project4/sample/aruco.png",markerImage);


    // Initialize the detector parameters using default values
    Ptr<cv::aruco::DetectorParameters> parameters = DetectorParameters::create();
                
    // Declare the vectors that would contain the detected marker corners and the rejected marker candidates
    vector<vector<Point2f>> markerCorners, rejectedCandidates;

    // The ids of the detected markers are stored in a vector
    vector<int> markerIds;
                
    // Detect the markers in the image
    detectMarkers(frame, dictionary, markerCorners, markerIds, parameters, rejectedCandidates);

    // Compute homography from source and destination points
    Mat h = cv::findHomography(pts_src, pts_dst);

    // Warped image
    Mat warpedImage;
                
    // Warp source image to destination based on homography
    warpPerspective(im_src, warpedImage, h, frame.size(), INTER_CUBIC);
            
    // Prepare a mask representing region to copy from the warped image into the original frame.
    Mat mask = Mat::zeros(frame.rows, frame.cols, CV_8UC1);
    fillConvexPoly(mask, pts_dst, Scalar(255, 255, 255));
                
    // Erode the mask to not copy the boundary effects from the warping
    Mat element = getStructuringElement( MORPH_RECT, Size(3,3) );
    erode(mask, mask, element);

    // Copy the masked warped image into the original frame in the mask region.
    Mat imOut = frame.clone();
    warpedImage.copyTo(imOut, mask);
}

