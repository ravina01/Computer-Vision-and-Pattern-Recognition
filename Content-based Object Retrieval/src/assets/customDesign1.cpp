#include <opencv2/opencv.hpp>
#include <cmath>
#include <iostream>
#include <dirent.h>
#include "calculation.h"

using namespace cv;
using namespace std;

//target_histogram
const int Hsize_3D = 8;
const int L = 255;
const float k1 = 0.01;
const float k2 = 0.03;

int ssim(float mean_target, float mean_input, float var_target, float var_input, float covar, float ssimDist){

    float D1 = (mean_target * mean_target + mean_input * mean_input + c1);
    float D2 = (var_target * var_target + var_input * var_input + c2);
    ssimDist = (2 * mean_target * mean_input) * ( 2 * covar + c1) / (D1 * D2);
    ssimDist = 1- ssimDist;
    return 0;
}
int varianceMat(vector<float> &normalized_target, float &var, float meanVal){

    var = 0.0;
    for(int i = 0; i < normalized_target.size(); i++){
        var += ((normalized_target[i] - meanVal) * (normalized_target[i] - meanVal) / normalized_target.size());
    }
    return 0;
}
int covariance(vector<float> &normalized_target, vector<float> &normalized_input, float mean_target, float mean_input, float &covar){
    
    covar =0.0;
    for(int i=0; i<normalized_target.size(); i++){
        covar += ( (normalized_target[i] - mean_target) * (normalized_input[i] - mean_input)/normalized_target.size() );
    }
    
    return 0;
}

int main(int argc, char** argv)
{
    char dirname[256];
    char buffer[256];
    FILE *fp;
    DIR *dirp;
    struct dirent *dp;
    int i;
  
    cv::Mat input_img;
    cv::Mat src;
   
    float c1 = (k1 * L) * (k1 * L);
    float c2 = (k2 * L) * (k2 * L);
    
    src = cv::imread("/home/ravina/Desktop/OpenCV/CBIR/sample/olympus/pic.0165.jpg", cv::IMREAD_COLOR);

    if (src.empty()) 
    {
        cout << "Could not load the image" << endl;
        return -1;
    }

    String windowName = "Target Image";
    imshow(windowName, src);
    
    int *hist_3d_target = new int [Hsize_3D * Hsize_3D * Hsize_3D];
    //initialize to all zeros
    for(int i=0; i < (Hsize_3D * Hsize_3D * Hsize_3D); i++)
        hist_3d_target[i] = 0;

    int divisor = 256/Hsize_3D;
    int size = Hsize_3D * Hsize_3D * Hsize_3D;
    cal3DHistogram(src, hist_3d_target,divisor, 0, src.rows, 0, src.cols);

    vector<float> normalized_target;
    normalizeHist(hist_3d_target, size, normalized_target);



    cv:: Mat mean_target, stddev_target, variance_target;
    cv::meanStdDev(normalized_target, mean_target, stddev_target);    
    variance_target = (stddev_target * stddev_target);
    cout<<(mean_target);
    cout<<variance_target;
    float meanVal;
    meanVal = 1.0/normalized_target.size();
    cout<<meanVal;
    float var;
    varianceMat(normalized_target, var, meanVal);
    cout<<"var ="<<var;
    
    

    // if( argc < 2) {
    //     printf("usage: %s <directory path>\n", argv[0]);
    //     exit(-1);
    // }
    // strcpy(dirname, argv[1]);
    // printf("Processing directory %s\n", dirname );
    
    // dirp = opendir( dirname );
    // if( dirp == NULL) {
    //     printf("Cannot open directory %s\n", dirname);
    //     exit(-1);
    // }
   
    std::vector<pair<float, string>> histSortVec;

    // while( (dp = readdir(dirp)) != NULL ) {
        
    //     if( strstr(dp->d_name, ".jpg") || strstr(dp->d_name, ".png") || strstr(dp->d_name, ".ppm") || strstr(dp->d_name, ".tif") ) {

    //             strcpy(buffer, dirname);
    //             strcat(buffer, "/");
    //             strcat(buffer, dp->d_name);
    //             //printf("full path name: %s\n", buffer);

    //             float intersectionDist;
    //             std::vector<int> input_feature;

    //             input_img = cv::imread(buffer);

    //             int *hist_3d_input = new int [Hsize_3D * Hsize_3D * Hsize_3D];

    //             //initialize to all zeros
    //             for(int i=0; i < (Hsize_3D * Hsize_3D * Hsize_3D); i++)
    //                 hist_3d_input[i] = 0;

    //             cal3DHistogram(input_img, hist_3d_input, divisor, 0, input_img.rows, 0, input_img.cols);
    //             vector<float> normalized_input;
    //             normalizeHist(hist_3d_input, size, normalized_input);

    //             //dstMetricHistIntersection(normalized_target, normalized_input, intersectionDist);
    //             delete hist_3d_input;
    //             histSortVec.push_back(make_pair(intersectionDist,buffer));
    //     }
    // }
    // delete hist_3d_target;
    // sort(histSortVec.begin(),histSortVec.end());

    // for( int i = 0; i < 4; i++ ) {
    //     cout<<histSortVec[i].first << " "<<histSortVec[i].second<<endl;
    // }        

    char key = cv::waitKey(0);
    while (1)
    {
        if(key == 'q'){
            break;
        }
    }

}


