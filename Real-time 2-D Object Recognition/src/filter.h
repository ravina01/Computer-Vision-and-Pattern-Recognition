#ifndef __FILTER_H_
#define __FILTER_H_

#include<iostream>
#include<opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

int greyscale( cv::Mat &src, cv::Mat &dst );
int blur5x5( cv::Mat &src, cv::Mat &dst );

int thresholdingGray( cv::Mat &src, cv::Mat &dst );

int thresholdingHSV( cv::Mat &src, cv::Mat &dst );
int grassfireTransform( cv::Mat &src, cv::Mat &dst );
int shrinkingImage( cv::Mat &src, cv::Mat &dst , int size);
int growingImage( cv::Mat &src, cv::Mat &dst, int size);
int invertImg( cv::Mat &src, cv::Mat &dst );
int invertGrowingImg( cv::Mat &src, cv::Mat &dst );
int growingOperations( cv::Mat &src, cv::Mat &dst, int size);

int segmenetation(cv::Mat &src, cv::Mat &dst2);

int scaledEuclidean(vector<double> &tempCsvVector, vector<double> &finalFeatureVec, vector<double> stdDeviation, float &euclideanDist);
int chiSquare(vector<double> &tempCsvVector, vector<double> &finalFeatureVec, float &chiDist);
int manhattanDist(vector<double> &tempCsvVector, vector<double> &finalFeatureVec, float &manhattanDist);
int correlationDst(vector<double> &tempCsvVector, vector<double> &finalFeatureVec, float mean_csvVector, float mean_finalFeature,float &corrDist);

int scaledEuclideanF(vector<float> &tempCsvVector, vector<float> &finalFeatureVec, vector<double> stdDeviation, float &euclideanDist);

#endif