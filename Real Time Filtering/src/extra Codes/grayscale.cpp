// OpenCV Grayscale Image Tutorial 
#include <opencv2/core/core.hpp> 
#include <opencv2/highgui/highgui.hpp> 
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
 
 cv::Mat src1,src2;

 src1 = imread("/home/ravina/sample/girl.jpg",cv::IMREAD_COLOR);
 src2 = Mat::zeros(src1.rows,src1.cols, cv::IMREAD_GRAYSCALE);

 if( !src1.data ) { printf("Error loading src1 \n"); return -1;}


int sum = 0;

for (int i=0; i<src1.cols ; i++){
for (int j=0 ; j<src1.rows ; j++)
 { 
Vec3b color1 = src1.at<Vec3b>(Point(i,j));
Scalar color2 = src2.at<uchar>(Point(i,j));
for(int c=0;c<3;c++){
    sum = sum + (0.33*src1.at<Vec3b>(Point(i,j))[c]); 
}
src2.at<uchar>(Point(i,j)) = sum;
      //src2.at<uchar>(Point(i,j)) = (0.33*src1.at<Vec3b>(Point(i,j))[0]+0.33*src1.at<Vec3b>(Point(i,j))[1]+0.33*src1.at<Vec3b>(Point(i,j))[2]);

   
  }
 }























    // int main()
    // {
    
    //     Mat src,sx, sy,dst;
    //     Mat3s temp;
    //     int gx, gy;
    //     short sum;
    
    //     // Load an image
    //     src = imread("/home/ravina/sample/girl.jpg", cv::IMREAD_COLOR);
    //     src.copyTo(dst);
    //     src.copyTo(temp);
    //     //dst = Mat::zeros(src.rows,src.cols, cv::IMREAD_GRAYSCALE); // grayscale
    //     if( !src.data )
    //     { return -1; }
    
    
    //         // for(int y = 0; y < src.rows; y++)
    //         //     for(int x = 0; x < src.cols; x++)
    //         //         for(int c =0; c < 3; c++)
    //         //             dst.at<Vec3s>(y,x)[c] = 0.0;
                    
    //         sx = src.clone();
    //         sy = src.clone();

    //         prewitX3(src,temp);
            
                    
            
    //                 convertScaleAbs(temp,dst);
    //         //flipImage(src,dst);
    //         // std::cout<<"rows"<<src.rows<<"c= "<<src.cols;
    //         // sketch(src,dst);
    //         //dst.create(src.cols, src.rows, src.type());
    //         //mirrorImg(src, dst);
    //        // transpose(src, dst);
    //        // sketch(src,dst);

    //         //imgSharpening(src, dst);
    //         //cartoon(src, dst, 10, 18);
    //         //blurQuantize(src, dst, 10);
    //         //negative(src, dst);

    //         // sobelY3x3(src, sy);
    //         // sobelX3x3(src, sx);
    //         //magnitude(sx, sy, dst);
    //         // for(int y = 1; y < src.rows - 1; y++){
    //         //     for(int x = 1; x < src.cols - 1; x++){
    //         //         gx = sobelX3x3(src, x, y);
    //         //         gy = sobelY3x3(src, x, y);
    //         //         sum = sqrt(gx*gx + gy*gy);
    //         //         // sum = sum > 255 ? 255:sum;
    //         //         // sum = sum < 0 ? 0 : sum;
    //         //         dst.at<Vec3s>(y,x) = sum;          
    //         //     }
    //         // }

            
    //         //blur5x5(src,dst);
    //        //greyscale(src,dst);
    
    //             namedWindow("final");
    //             imshow("final", dst);
    
    //         namedWindow("initial");
    //         imshow("initial", src);
    
    //     waitKey();
    
    
    //     return 0;
    // }






imshow("GRAYSCALE_IMAGE", src2); 


imshow("Original Image", src1);

 waitKey(0);
 return 0;
}