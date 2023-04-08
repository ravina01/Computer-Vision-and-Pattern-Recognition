#include<iostream>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;
 
int main()
{
      Mat img, dst, temp;

      /// Load an image
      img = imread("/home/ravina/sample/girl.jpg", cv::IMREAD_COLOR);
      
      if( !img.data )
      { return -1; }    
 
      dst = img.clone();
      temp = img.clone();
 
        // along y - direction
        for(int c = 0; c < 3; c++){
            for(int y = 2; y < img.rows-2; y++){
                for(int x = 2; x < img.cols-2; x++){
                    temp.at<Vec3b>(y,x)[c] = 0.1*img.at<Vec3b>(y, x-2)[c] + 0.2*img.at<Vec3b>(y, x-1)[c] + 0.4*img.at<Vec3b>(y, x)[c] + 0.2*img.at<Vec3b>(y, x+1)[c] + 0.1*img.at<Vec3b>(y, x+2)[c];        
                    dst.at<Vec3b>(y,x)[c] = 0.1*temp.at<Vec3b>(y-2, x)[c] + 0.2*temp.at<Vec3b>(y-1, x)[c] + 0.4*temp.at<Vec3b>(y, x)[c] + 0.2*temp.at<Vec3b>(y+1, x)[c] + 0.1*temp.at<Vec3b>(y+2, x)[c];
                }
            }
        }
        
        namedWindow("final");
        imshow("final", dst);
 
        namedWindow("initial");
        imshow("initial", img);
 
      waitKey();
 
 
    return 0;
}