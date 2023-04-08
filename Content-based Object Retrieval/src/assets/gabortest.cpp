#include <opencv2/opencv.hpp>
#include <cmath>
#include <iostream>
#include <dirent.h>
#include "calculation.h"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    cv::Mat inImg = imread("/home/ravina/Desktop/OpenCV/CBIR/sample/gabor_demo_02.png", cv::IMREAD_COLOR);
    cv::Mat floatImg, floatImg1;
    inImg.copyTo(floatImg);
    inImg.copyTo(floatImg1);
    inImg.convertTo(floatImg,CV_32F);
    inImg.convertTo(floatImg1,CV_32F);
    cv::Mat dst,dst1;
    inImg.copyTo(dst);
    inImg.copyTo(dst1);

    int kernel_size = 31;
    double sig = 1, th = 0, lm = 1.0, gm = 0.02, ps = 0;
    // cv::Mat kernel = cv::getGaborKernel(cv::Size(kernel_size,kernel_size), sig, th, lm, gm, ps);
    // cv::Mat kernel1 = cv::getGaborKernel(cv::Size(kernel_size,kernel_size), sig, (M_PI/2), lm, gm, ps);
    // cv::filter2D(floatImg, dst, CV_32F, kernel);
    // cv::filter2D(floatImg1, dst1, CV_32F, kernel1);
   

    // Mat viz;
    // Mat viz1, viz3;
    // dst.convertTo(viz,CV_8UC3);     // move to proper[0..255] range to show it
    // dst1.convertTo(viz1,CV_8UC3);
    // imshow("1", viz);
    // imshow("2", viz1);

    // cv::Mat res;
    // // = Mat::zeros( dst.size(), dst.type() );
    // dst.copyTo(res);
    // //cout<<"type "<<dst.type();
    // for(int y = 1; y < dst.rows - 1; y++){
    //     for(int x = 1; x < dst.cols - 1; x++){
    //         for(int c =0; c<3; c++){
    //             res.at<Vec3f>(y,x)[c] = max(dst.at<Vec3f>(y,x)[c], dst1.at<Vec3f>(y,x)[c]);
    //         }                        
    //     }
    // }
   
    // res.convertTo(viz3,CV_8UC3,1.0/255.0);
    // imshow("res",viz3);
   
    double th1 = M_PI/16;
    cv::Mat res;
    inImg.convertTo(res,CV_32F);
    //imshow("res", res);
    // cout<<"res = "<<res.type();
    for(int i = 1; i< 32; i++){
        cv::Mat kernel = cv::getGaborKernel(cv::Size(kernel_size,kernel_size), sig, (i*th1), lm, gm, ps);
        cv::filter2D(floatImg, dst, CV_32F, kernel);
        //cerr << dst(Rect(30,30,10,10)) << endl;
        Mat viz;
        
        cout<<"res = "<<dst.type();
        for(int y = 1; y < dst.rows-1; y++){
            for(int x = 1; x < dst.cols-1; x++){
                for(int c =0; c<3; c++){
                    res.at<Vec3f>(y,x)[c] = min(res.at<Vec3f>(y,x)[c], dst.at<Vec3f>(y,x)[c]);
                }                        
            }
        }
        dst.convertTo(viz,CV_8UC3);
        // imshow("k",kernel);
        imshow("d",viz);
        waitKey();
        
    }
    Mat viz1;
    res.convertTo(viz1,CV_8UC3);

    // imshow("k",kernel);
    // cout<<kernel.size();
    // imshow("d",viz);
    //imshow("d1",viz1);
    imshow("res",viz1);

    waitKey();
    
    
}

