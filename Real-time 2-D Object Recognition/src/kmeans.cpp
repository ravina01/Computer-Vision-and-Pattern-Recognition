/*
Ravina Lad
Date : 6th March 2022
CS 5330 Computer Vision
Spring 2021

CPP file for Implementing a different classifier that is k-means clustering 
- K = 3
- Done training and prediction in same file
*/

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <cmath>
#include "filter.h"
#include "csv_util.h"

using namespace cv;
using namespace std;

const int K = 3; 
/** @function main */
int kmeans( vector<vector<float> > finalFeature)
{

        char filename[355] = "/home/ravina/Desktop/OpenCV/ObjectDetection/sample/knn.csv";

        vector<char *> objectNames;
        vector<vector<float> > ObjectFeatureVectors;
        read_image_data_csv(filename, objectNames, ObjectFeatureVectors);
        vector<double> stdDeviation;
        vector<int> labels(ObjectFeatureVectors.size());


        for(int i = 0; i< ObjectFeatureVectors[0].size(); i++){
              vector<float> tempstd;
              for(int j = 0; j< ObjectFeatureVectors.size(); j++){
                  tempstd.push_back(ObjectFeatureVectors[j][i]);        
              }
              Mat mean, stddev;
              cv::meanStdDev(tempstd, mean, stddev);
              stdDeviation.push_back(stddev.at<double>(0));
        }

        vector<vector<float> >centroid(K);
        for(int i = 0; i < 3; i++){
            centroid[i] = ObjectFeatureVectors[i];      
        }
        vector<vector<float> >previous_centroid(K);

        // Train k-means clustering model

        for(int iterator=0; iterator<ObjectFeatureVectors.size(); iterator++){

            for(int i = 0; i < ObjectFeatureVectors.size(); i++){
                  vector<float> euclideanVec(K);
                  for(int c=0; c<centroid.size(); c++){
                    scaledEuclideanF(ObjectFeatureVectors[i], centroid[c], stdDeviation, euclideanVec[c]);
                    //cout<<euclideanDist;
                  }
                  for(int c1=0; c1<centroid.size();c1++){
                      if(*min_element(euclideanVec.begin(),euclideanVec.end()) == euclideanVec[c1]){
                          labels[i] = c1;
                      }
                  }
              }

              //loop over
              previous_centroid = centroid;
              int count1=1,count2=1,count3=1;
              for(int i = 0; i < ObjectFeatureVectors.size(); i++){
                for(int j=0; j<ObjectFeatureVectors[i].size();j++){              
                    
                    if(labels[i]==0){
                      centroid[0][j] +=ObjectFeatureVectors[i][j];
                    }
                    if(labels[i]==1){
                      centroid[1][j] +=ObjectFeatureVectors[i][j];
                    }
                    if(labels[i]==2){
                      centroid[2][j] +=ObjectFeatureVectors[i][j];
                    }
                }
                if(labels[i]==0){
                      count1++;
                    }
                    if(labels[i]==1){
                      count2++;
                    }
                    if(labels[i]==2){
                      count3++;
                    }
              }

              for(int i = 0; i < ObjectFeatureVectors[0].size(); i++){
                      centroid[0][i] /=count1;
                      centroid[1][i] /=count2;
                      centroid[2][i] /=count3;
              }
            
            if(centroid == previous_centroid){
              break;
            }

        }

        
                // for(int j=0; j<centroid[0].size();j++){              
                //     cout<<" c1 = "<<centroid[0][j]<<endl;
                //     cout<<" c2 = "<<centroid[1][j]<<endl;
                //     cout<<" c3 = "<<centroid[2][j]<<endl;
                // }
        
        
        //predict K-means results
        
        vector<float> euclideanVec(K);
          for(int c=0; c<centroid.size(); c++){
              scaledEuclideanF(finalFeature[0], centroid[c], stdDeviation, euclideanVec[c]);
          }

        for(int c1=0; c1<centroid.size();c1++){

            if(*min_element(euclideanVec.begin(),euclideanVec.end()) == euclideanVec[c1]){

              if(c1==0){
                cout<<"This is K-means result object is hairbelt";
              }
              if(c1==1){
                cout<<"This is K-means result object is scissor";
              }
              if(c1==2){
                cout<<"This is K-means result object is knife";
              }
          }
        }
                     
                           
                  
        

            

        // for(int l=0; l<labels.size();l++){
        //   cout<<labels[l]<<endl;
        // }
        

    return 0;
}

  


