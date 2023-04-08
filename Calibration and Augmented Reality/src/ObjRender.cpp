/*
Ravina Lad
Date : 26th March 2022
CS 5330 Computer Vision
Spring 2021

This CPP file includes
- Part of Task Extension 3 : Implemented : Render .obj File and display on checkerboard
                    Here, we are parsing the file and extracting normals, faces and vertices of .obj file
                     
*/
#include <opencv2/opencv.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/core.hpp"
#include "ObjRender.h"
#include <iostream>

using namespace std;

ObjRender::ObjRender() {}

void ObjRender::renderFile(std::string fileName){

    std::string initial;
    std::ifstream inFile(fileName);
    std::vector<std::string> tokens, indices;

    while(std::getline(inFile, initial)){

        tokens = ObjRender::split(initial, ' ');
        if(tokens.size() >0){
            std::vector<int> faceIndex;
            if(!tokens[0].compare("v")){
                vertices.push_back(cv::Point3f(std::stof(tokens[1]) ,std::stof(tokens[3]), (-1*std::stof(tokens[2])) ));
            }
            else if(!tokens[0].compare("vn")){
                normals.push_back( cv::Point3f( std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3])) );
            }
            else if(!tokens[0].compare("f")){
                for(int i=1;i<tokens.size();i++){
                    indices = ObjRender::split(tokens[i],'/');
                    faceIndex.push_back(std::stoi(indices[0]));
                }

                faceVert.push_back(faceIndex);
            }
        }

    }
    /*
    for(int i=0;i<vertices.size();i++){
        cout<<"V = "<<vertices[i]<<endl;
    }

    for(int i=0;i<faceVert.size();i++){
        for(int j=0; j<faceVert[i].size();j++){
            cout<<"F = "<<faceVert[i][j]<<endl;
        }
        
    }*/
}

std::vector<std::string> ObjRender::split(std::string &str, char delim){

    std::vector<string> tokens;
    std::string part;

    std::stringstream str_stream(str);
    while(std::getline(str_stream,part,delim)){
        tokens.push_back(part);
    }
    return tokens;
}
ObjRender::~ObjRender(){
    vertices.clear();
}
