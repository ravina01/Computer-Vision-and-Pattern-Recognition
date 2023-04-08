/*
Ravina Lad
Date : 6th March 2022
CS 5330 Computer Vision
Spring 2021

CPP file is collection of all functions used throgh out the project
- different distance metrics : squared euclidean, chi-square
- segmentation code, morphological operations : growing and shrnking from scratch
- grassfire Tranform Code
*/
#include<iostream>
#include<cmath>
#include "filter.h"
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

//function to convert image to gray scale
int greyscale(cv::Mat &src, cv::Mat &dst){

    for (int y=0; y<src.rows ; y++){
        for (int x=0 ; x<src.cols ; x++){  
            int sum = 0;
            for(int c=0; c< 3; c++){
                sum += src.at<Vec3b>(y,x)[c]/3;
            } 
            
            dst.at<uchar>(y,x) = sum;   
                
            
        }
    }
    return 0;
}

// blur the image to reduce noise
int blur5x5( cv::Mat &src, cv::Mat &dst ){

    cv::Mat temp;
    temp = src.clone();

    for(int c = 0; c < 3; c++){
        for(int y = 0; y < src.rows; y++){
            for(int x = 2; x < src.cols-2; x++){
                temp.at<Vec3b>(y,x)[c] = 0.1*src.at<Vec3b>(y, x-2)[c] + 0.2*src.at<Vec3b>(y, x-1)[c] + 0.4*src.at<Vec3b>(y, x)[c] + 0.2*src.at<Vec3b>(y, x+1)[c] + 0.1*src.at<Vec3b>(y, x+2)[c];
            }
        }
    }
    for(int c = 0; c < 3; c++){
        for(int y = 2; y < src.rows-2; y++){
            for(int x = 0; x < src.cols; x++){
                dst.at<Vec3b>(y,x)[c] = 0.1*temp.at<Vec3b>(y-2, x)[c] + 0.2*temp.at<Vec3b>(y-1, x)[c] + 0.4*temp.at<Vec3b>(y, x)[c] + 0.2*temp.at<Vec3b>(y+1, x)[c] + 0.1*temp.at<Vec3b>(y+2, x)[c];
            }
        }
    }
    return 0;
}

//thresholding gray will thresholding gray scale image
int thresholdingGray( cv::Mat &src, cv::Mat &dst ){

    for(int y = 0; y < src.rows; y++){
        for(int x = 0; x < src.cols; x++){
            dst.at<uchar>(y,x) = src.at<uchar>(y,x) > 78 ? 0 : 255;
        }
    }
    return 0;
}

//thresholding HSV space image
int thresholdingHSV( cv::Mat &src, cv::Mat &dst ){

    for(int c = 0; c < 3; c++){
        for(int y = 0; y < src.rows; y++){
            for(int x = 0; x < src.cols; x++){
                dst.at<uchar>(y,x) = src.at<Vec3b>(y,x)[1] > 75 ? 255 : 0;
            }
        }
    }
    convertScaleAbs(dst,dst);
   
    return 0;
}

//Grass Transform Function to calculate manhattan distance
int grassfireTransform( cv::Mat &src, cv::Mat &dst ){

    for(int y = 0; y < src.rows; y++){
        for(int x = 0; x < src.cols; x++){
            if( src.at<uchar>(y,x)  != 0)
            {
                unsigned char north = (y == 0) ? 0 : dst.at<uchar>(y-1,x);
                unsigned char west = (x == 0) ? 0 : dst.at<uchar>(y,x-1);
                dst.at<uchar>(y,x) = saturate_cast<uchar>(1 + min(north, west));
            }
            else{
                dst.at<uchar>(y,x) = 0;
            }
        }
    }

    for(int y = src.rows-1; y < -1; y--){
        for(int x = src.cols-1; x < -1; x--){
            if( (dst.at<uchar>(y,x)) != 0)
            {
                unsigned char south = (y == src.rows-1) ? 0 : dst.at<uchar>(y+1,x);
                unsigned char east = (x == src.cols-1) ? 0 : dst.at<uchar>(y,x+1);
                dst.at<uchar>(y,x) = min(dst.at<uchar>(y,x) , saturate_cast<uchar>(1 + min(south, east)));
            }
            else{
                dst.at<uchar>(y,x) = 0;
            }
        }
    }

    return 0;
}

//Pass value to shrink image, It will convert those many pixels to background
int shrinkingImage( cv::Mat &src, cv::Mat &dst, int size){

    for(int y = 0; y < src.rows; y++){
        for(int x = 0; x < src.cols; x++){
            dst.at<uchar>(y,x) = src.at<uchar>(y,x) <= size ? 0 : src.at<uchar>(y,x);
            dst.at<uchar>(y,x) = src.at<uchar>(y,x) > size ? 255 : src.at<uchar>(y,x);
        }
    }
    return 0;
}

//Pass value to grow image, It will convert those many pixels to foreground
int growingImage( cv::Mat &src, cv::Mat &dst, int size ){

    for(int y = 0; y < src.rows; y++){
        for(int x = 0; x < src.cols; x++){
            dst.at<uchar>(y,x) = src.at<uchar>(y,x) <= size ? 0 : src.at<uchar>(y,x);
            dst.at<uchar>(y,x) = src.at<uchar>(y,x) > size ? 255 : src.at<uchar>(y,x);
        }
    }
    return 0;
}

//Function that is part of growng operation
int invertImg( cv::Mat &src, cv::Mat &dst ){

    for(int y = 0; y < src.rows; y++){
        for(int x = 0; x < src.cols; x++){
            dst.at<uchar>(y,x) = src.at<uchar>(y,x) == 0 ? 1 : 0;
        }
    }
    return 0;
}

//Function that is part of growng operation
int invertGrowingImg( cv::Mat &src, cv::Mat &dst ){

    for(int y = 0; y < src.rows; y++){
        for(int x = 0; x < src.cols; x++){
            dst.at<uchar>(y,x) = src.at<uchar>(y,x) == 0 ? 255 : 0;
        }
    }
    return 0;
}

//Function - performs multiple operation to grow an image
int growingOperations( cv::Mat &src, cv::Mat &dst, int size ){

    invertImg(src, dst);
    grassfireTransform(dst, dst);
    growingImage(dst, dst, size);
    invertGrowingImg(dst, dst);
    return 0;
}

// Function call to segment binary image
int segmenetation(cv::Mat &src, cv::Mat &dst2){

        Mat image_th;
        cv::Mat gray_mat(src.size(), CV_8U);

        image_th = Mat::zeros(src.rows,src.cols, CV_8UC1);

        cv::cvtColor(src, gray_mat, cv::COLOR_RGB2GRAY);
        //greyscale(src,gray_mat);
        thresholdingGray(gray_mat, image_th);
        
        //uncomment below code to : change thresholding to HSV
        // Mat result;
        // src.copyTo(result);
        // cvtColor(src, result, cv::COLOR_RGB2HSV);
        // thresholdingHSV(result, image_th);
        
        //imshow("thresolded Img", image_th);

        grassfireTransform(image_th, image_th);
        shrinkingImage(image_th, image_th, 2);
        
        growingOperations(image_th, image_th,2);
        grassfireTransform(image_th, image_th);
        shrinkingImage(image_th, image_th, 1);
        growingOperations(image_th, image_th,1);

        //imshow("clean binary", image_th);
        
        Mat labelImage(src.size(), CV_32S);
        Mat stats, centroids;
        int nLabels = connectedComponentsWithStats(image_th, labelImage, stats, centroids, 8, CV_32S);
        std::vector<Vec3b> colors(nLabels);
        std::vector<int> labels_finals;
        colors[0] = Vec3b(0, 0, 0);//background


        for (int label = 1; label < nLabels; ++label){ //label  0 is the background
            if ((stats.at<int>(label, CC_STAT_AREA)) > 1000){
                labels_finals.push_back(label);
                //cout << "hola" << endl;
            }
            colors[label] = Vec3b(0, 255, 0);
        }


        Mat dst(src.size(), CV_8UC3);
        for (int r = 0; r < dst.rows; ++r){
            for (int c = 0; c < dst.cols; ++c){

                int label = labelImage.at<int>(r, c);
                Vec3b &pixel = dst.at<Vec3b>(r, c);
                pixel = colors[label];
            }
        }

        //Mat dst2(src.size(), CV_8UC3);

        for (int i = 0; i < labels_finals.size(); ++i){
            compare(labelImage, labels_finals[i], dst2, CMP_EQ);
        }

        imshow("Segmen", dst2);

    return 0;

}

//Scaled euclidean distance to calculate distance of : various units like : Humoments, aspect and percentage ratio
int scaledEuclidean(vector<double> &tempCsvVector, vector<double> &finalFeatureVec, vector<double> stdDeviation, float &euclideanDist){

    euclideanDist = 0.0;
    for(int i = 0; i < finalFeatureVec.size(); i++){
        euclideanDist += (tempCsvVector[i] - finalFeatureVec[i]) * (tempCsvVector[i] - finalFeatureVec[i])/(stdDeviation[i] * stdDeviation[i]);
    }
    return 0;
}


//Chi-square distance mteric to calculate distances
int chiSquare(vector<double> &tempCsvVector, vector<double> &finalFeatureVec, float &chiDist){

    float dist1, dist2 = 0.0;
    for(int i = 0; i < finalFeatureVec.size(); i++){
        dist1 += (tempCsvVector[i] - finalFeatureVec[i]) * (tempCsvVector[i] - finalFeatureVec[i]);
        dist2 += (finalFeatureVec[i]);
    }
    chiDist = dist1/dist2;
    //chiDist = 1 - chiDist;
    return 0;
}

//Manhattan / L1 Norm distance mteric to calculate distances
int manhattanDist(vector<double> &tempCsvVector, vector<double> &finalFeatureVec, float &manhattanDist){

    for(int i = 0; i < finalFeatureVec.size(); i++){
        manhattanDist += abs(tempCsvVector[i] - finalFeatureVec[i]);
    }

    return 0;
}

//Function to calculate Correlation as distance Metric
int correlationDst(vector<double> &tempCsvVector, vector<double> &finalFeatureVec, float mean_csvVector, float mean_finalFeature,float &corrDist){

    float dist1, dist2, dist3 = 0.0;
    for(int i = 0; i < tempCsvVector.size(); i++){
        dist1 += (tempCsvVector[i]- mean_csvVector) * (finalFeatureVec[i] - mean_finalFeature);
        dist2 += (tempCsvVector[i]- mean_csvVector) * (tempCsvVector[i]- mean_csvVector);
        dist3 +=  (finalFeatureVec[i]- mean_finalFeature) * (finalFeatureVec[i]- mean_finalFeature);

    }
    corrDist = dist1 / sqrt(dist2 * dist3);
    corrDist = 1 - corrDist;
    return 0;
}

int scaledEuclideanF(vector<float> &tempCsvVector, vector<float> &finalFeatureVec, vector<double> stdDeviation, float &euclideanDist){

    euclideanDist = 0.0;
    for(int i = 0; i < finalFeatureVec.size(); i++){
        euclideanDist += (tempCsvVector[i] - finalFeatureVec[i]) * (tempCsvVector[i] - finalFeatureVec[i])/(stdDeviation[i] * stdDeviation[i]);
    }
    return 0;
}



// int greyscale(cv::Mat &src, cv::Mat &dst){

//     for (int y=0; y<src.rows ; y++){
//         for (int x=0 ; x<src.cols ; x++){  
//             //int sum = 0;
//             int minVal = min(src.at<Vec3b>(y,x)[0], src.at<Vec3b>(y,x)[1], src.at<Vec3b>(y,x)[2]);
//             float sum = (src.at<Vec3b>(y,x)[0], src.at<Vec3b>(y,x)[1], src.at<Vec3b>(y,x)[2])/3;
//             // for(int c=0; c< 3; c++){
//             //     sum += src.at<Vec3b>(y,x)[c]/3;
//             // }
//             sum = (sum > 130) ? sum*0.75 : (255- ((255-sum)*0.75) );
//             float saturation = (float)(1 - 3*minVal)/sum;
//             saturation = (1.25 - saturation)*sum;
//             dst.at<uchar>(y,x) = saturation;
                
            
//         }
//     }
//     return 0;
// }