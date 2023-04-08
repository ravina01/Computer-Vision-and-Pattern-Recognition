/*
Sequence Number : 4
Ravina Lad
Date : 26th March 2022
CS 5330 Computer Vision
Spring 2021

This CPP file includes
- Task Extension 2 : Implemented : Augmented Reality television
                     this file detects aruco markers from 0 to 3 and overlap video on the detected aruco frames (dictionary : DICT_6X6_250)
                     functions like : Persppective Transorms and overlapping frames written in filter.cpp
                     These function find box coordinates and overlaps video frames on detected markers.
                     we can also play live videos on these markers and post markers on call and get the experience of watching television.
*/

#include "opencv2/opencv.hpp"
#include <opencv2/aruco.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include "opencv2/imgcodecs.hpp"
#include <iostream>
#include <stdio.h>
#include <vector>
#include "filter.h"

int main(int argc, char **argv)
{
    // Firstly, reading aruco video and the video for projection
    cv::VideoCapture inputVid_Cap, ProjVid_Cap;

    //This input video recoreded and used to display the projected video on markers
    inputVid_Cap.open("/home/ravina/Desktop/OpenCV/Project4/sample/assets/minuteV.mp4");
    ProjVid_Cap.open("/home/ravina/Desktop/OpenCV/Project4/sample/assets/ProjVid.mp4");

    // Creating video writer object
    cv::VideoWriter OutVid;
    OutVid.open("/home/ravina/Desktop/OpenCV/Project4/sample/assets/savedMinV.mp4", static_cast<int>(inputVid_Cap.get(cv::CAP_PROP_FOURCC)), inputVid_Cap.get(cv::CAP_PROP_FPS), cv::Size(int(inputVid_Cap.get(3)), int(inputVid_Cap.get(4))));

    std::vector<int> SkippedFrames;         // record of skipped frames will be kept here

    while (true)
    {
        // Checking if all videos are opened.
        if (!inputVid_Cap.isOpened())
        {
            std::cout << "\nNot able to read aruco video.\n";
            exit(1);
        }
        if (!ProjVid_Cap.isOpened())
        {
            std::cout << "\nNot able to read projection video.\n";
            exit(2);
        }
                    
        // Reading frames
        cv::Mat ArucoVid_Frame, ProjVid_Frame;
        inputVid_Cap >> ArucoVid_Frame;
        ProjVid_Cap >> ProjVid_Frame;

        // End the output video if retArucoVid is false (video finished)
        if (ArucoVid_Frame.empty())
        {
            OutVid.release();
            std::cout << "\nStimulation Completed.\n";
            exit(3);
        }

        // Restart the projection video if finished
        if (ProjVid_Frame.empty())
        {
            ProjVid_Cap.set(cv::CAP_PROP_POS_FRAMES, 0);
            ProjVid_Cap >> ProjVid_Frame;
        }


        // Detecting Aruco markers in the frame
        std::vector<cv::Point2f> BoxCoordinates;
        bool ret = arucoDetector(ArucoVid_Frame, BoxCoordinates);

        // If aruco markers are not found, skip this frameand read next frame
        if (!ret)
        {
            // Storing the timestamp
            SkippedFrames.push_back(inputVid_Cap.get(cv::CAP_PROP_POS_MSEC));
            continue;
        }
            
        cv::Mat OverlapedFrame;
        FramesOverlapping(ArucoVid_Frame, ProjVid_Frame, BoxCoordinates, OverlapedFrame);

        // Storing to output video
        OutVid.write(OverlapedFrame);

        // Displaying Output video
        cv::imshow("Output Video", OverlapedFrame);
        cv::waitKey(1);
    }

    // Releasing video objectsand destroying windows
    inputVid_Cap.release();
    ProjVid_Cap.release();
    OutVid.release();
    cv::destroyAllWindows();

    

	return 0;
}