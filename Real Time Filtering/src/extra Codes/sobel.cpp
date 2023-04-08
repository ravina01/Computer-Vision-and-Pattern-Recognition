#include <iostream>
#include "opencv2/opencv.hpp"

int main() {


     else if (img_filter == "GRADIENT MAGNITUDE"){
                    
                    frame.copyTo(result);
                    frame.copyTo(temp_sx);
                    frame.copyTo(temp_sy);                  

                    sobelX3x3(frame, temp_sx);
                   //convertScaleAbs(temp_sx, sx);

                    sobelY3x3(frame, temp_sy);
                   //convertScaleAbs(temp_sy, sy);

                    magnitude(temp_sx, temp_sy, temp);
                    convertScaleAbs(temp, result);
                    
                }
                
    // int sobelX3x3(Mat image, int x, int y)
// {
//     for(int c=0; c < 3; c++){
//     return (image.at<Vec3s>(y-1, x-1)[c] +
//                 2*image.at<Vec3s>(y, x-1)[c] +
//                  image.at<Vec3s>(y+1, x-1)[c] -
//                   image.at<Vec3s>(y-1, x+1)[c] -
//                    2*image.at<Vec3s>(y, x+1)[c] -
//                     image.at<Vec3s>(y+1, x+1)[c])/4;
//     }
// }
 
 
// int sobelY3x3(Mat image, int x, int y)
// {
//     for(int c=0; c < 3; c++){
//         return (image.at<Vec3s>(y-1, x-1)[c] +
//                 2*image.at<Vec3s>(y-1, x)[c] +
//                  image.at<Vec3s>(y-1, x+1)[c] -
//                   image.at<Vec3s>(y+1, x-1)[c] -
//                    2*image.at<Vec3s>(y+1, x)[c] -
//                     image.at<Vec3s>(y+1, x+1)[c])/4;
//     }
    
// }

// int sx; Sobel Y
    // for(int y = 1; y < src.rows - 1; y++){
    //     for(int x = 1; x < src.cols - 1; x++){
    //         for(int c=0; c < 3; c++){
    //             dst.at<Vec3s>(y,x)[c] = (src.at<Vec3b>(y-1, x-1)[c] +
    //                 2*src.at<Vec3b>(y-1, x)[c] +
    //                 src.at<Vec3b>(y-1, x+1)[c] -
    //                 src.at<Vec3b>(y+1, x-1)[c] -
    //                 2*src.at<Vec3b>(y+1, x)[c] -
    //                     src.at<Vec3b>(y+1, x+1)[c])/4;                
    //         }
                        
    //     }
    // }
    // return 0;

// for(int y = 1; y < src.rows - 1; y++){ Sobel X
    //     for(int x = 1; x < src.cols - 1; x++){
    //         for(int c = 0; c <3; c++){
    //             dst.at<Vec3s>(y,x)[c] = (-src.at<Vec3b>(y-1, x-1)[c] -
    //             2*src.at<Vec3b>(y, x-1)[c] -
    //              src.at<Vec3b>(y+1, x-1)[c] +
    //               src.at<Vec3b>(y-1, x+1)[c] +
    //                2*src.at<Vec3b>(y, x+1)[c] +
    //                 src.at<Vec3b>(y+1, x+1)[c])/4;
    //         }
                
    //     }
    // }
    // return 0;

    int rotate(cv::Mat &src, cv::Mat &dst, double angle){

    int tempRow, tempCol;
    int newWidth = src.cols * sqrt(2);
    int newHeight = src.rows * sqrt(2);
    dst.create(newHeight, newWidth, src.type());

    double cosTheta = cos(angle);
    double sinTheta = sin(angle);

    for(int c = 0; c < 3; c++){
        for(int y = 0; y < src.rows; y++){
            for(int x = 0; x < src.cols; x++){

                tempRow =  ((y - newHeight/2) * cosTheta + (x - newWidth/2) * sinTheta + src.rows);
                tempCol =  (-(y - newHeight/2) * sinTheta + (x - newWidth/2) * cosTheta + src.cols);

                if( tempRow > 0 && tempCol >0 && tempRow <= src.rows && tempCol <= src.cols)
                    dst.at<Vec3b>(y,x)[c] = src.at<Vec3b>(tempRow,tempCol)[c];
                else
                    dst.at<Vec3b>(x,y)[c] = unsigned char(0,0,0);
            }
        }
    }
    return 0;

}

}