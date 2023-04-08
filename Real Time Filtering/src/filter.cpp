#include<iostream>
#include<cmath>
#include "filter.h"
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int greyscale(cv::Mat &src, cv::Mat &dst){

    for (int y=0; y<src.rows ; y++){
        for (int x=0 ; x<src.cols ; x++){  
            int sum = 0;
            for(int c=0; c< 3; c++){
                sum += src.at<Vec3b>(y,x)[c]/3;
            } 
            for(int c=0; c< 3; c++){
                dst.at<Vec3b>(y,x)[c] = sum;
                
            }
        }
    }
    return 0;
}
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

int sobelX3x3(cv::Mat &src, cv::Mat3s &dst){
    
    cv::Mat temp;
    src.copyTo(temp);

    for(int c = 0; c < 3; c++){
        for(int y = 1; y < src.rows-1; y++){
            for(int x = 0; x < src.cols; x++){
                temp.at<Vec3b>(y,x)[c] = (0.25*src.at<Vec3b>(y-1, x)[c] + 0.5*src.at<Vec3b>(y, x)[c] + 0.25*src.at<Vec3b>(y+1, x)[c]);
            }
        }
    }
    for(int c = 0; c < 3; c++){
        for(int y = 0; y < src.rows; y++){
            for(int x = 1; x < src.cols-1; x++){
                dst.at<Vec3s>(y,x)[c] = (-1*temp.at<Vec3b>(y, x-1)[c] + 0*temp.at<Vec3b>(y, x)[c] + temp.at<Vec3b>(y, x+1)[c]);
            }
        }
    }
    return 0;

}

int sobelY3x3(cv::Mat &src, cv::Mat3s &dst)
{
    cv::Mat3s temp;
    src.copyTo(temp);

    for(int c = 0; c < 3; c++){
        for(int y = 1; y < src.rows-1; y++){
            for(int x = 0; x < src.cols; x++){
                temp.at<Vec3s>(y,x)[c] = (src.at<Vec3b>(y-1, x)[c] + 0*src.at<Vec3b>(y, x)[c] - src.at<Vec3b>(y+1, x)[c]);
            }
        }
    }
    for(int c = 0; c < 3; c++){
        for(int y = 0; y < src.rows; y++){
            for(int x = 1; x < src.cols-1; x++){
                dst.at<Vec3s>(y,x)[c] = (0.25*temp.at<Vec3s>(y, x-1)[c] + 0.5*temp.at<Vec3s>(y, x)[c] + 0.25*temp.at<Vec3s>(y, x+1)[c]);
            }
        }
    }
    return 0;    
}

int magnitude( cv::Mat &sx, cv::Mat &sy, cv::Mat &dst ){

    unsigned char gx, gy;
    for(int y = 1; y < sx.rows - 1; y++){
            for(int x = 1; x < sx.cols - 1; x++){
                for(int c =0; c<3; c++){
                    // gx = sx.at<Vec3s>(y,x)[c];
                    // gy = sy.at<Vec3s>(y,x)[c];
                    dst.at<Vec3b>(y,x)[c] = sqrt((sx.at<Vec3b>(y,x)[c] * sx.at<Vec3b>(y,x)[c]) + (sy.at<Vec3b>(y,x)[c] * sy.at<Vec3b>(y,x)[c]));
                }                        
            }
        }
    return 0;
}

int blurQuantize( cv::Mat &src, cv::Mat &dst, int levels ){

    cv:: Mat temp;
    int b = 255/levels;
    temp = src.clone();
    blur5x5(src,temp);
    short xt, xf;
    for(int y = 0; y < temp.rows; y++){
        for(int x = 0; x < temp.cols; x++){
            for(int c = 0; c < 3; c++){
                xt = (temp.at<Vec3b>(y,x)[c]/b);
                xf = xt * b;
                dst.at<Vec3b>(y,x)[c] = xf; 
            }
        }
    }
    return 0;
}

int cartoon(cv::Mat &src, cv::Mat &dst, int levels, int magThreshold){

    cv:: Mat3s temp_sx, temp_sy;
    cv::Mat result, sx, sy;

    src.copyTo(result);
    src.copyTo(temp_sx);
    src.copyTo(temp_sy);                  

    sobelX3x3(src, temp_sx);
    convertScaleAbs(temp_sx, sx);

    sobelY3x3(src, temp_sy);
    convertScaleAbs(temp_sy, sy);

    magnitude(sx, sy, result);                   

    blurQuantize(src, dst, levels);

    for(int y = 0; y < result.rows; y++){
        for(int x = 0; x < result.cols; x++){
            for(int c = 0; c < 3; c++){
               dst.at<Vec3b>(y,x)[c] = result.at<Vec3b>(y,x)[c] > magThreshold ? 0:dst.at<Vec3b>(y,x)[c];
            }
        }
    }
    return 0;

}
int brightness(cv::Mat &src, cv::Mat &dst){

    for(int c = 0; c < 3; c++){
        for(int y = 1; y < src.rows-1; y++){
            for(int x = 0; x < src.cols; x++){
                dst.at<Vec3b>(y,x)[c] = saturate_cast<uchar>(2*(src.at<Vec3b>(y,x)[c]) + 1);
            }
        }
    }
}
int negative(cv::Mat &src, cv::Mat &dst){

    for(int y = 0; y < src.rows; y++){
        for(int x = 0; x < src.cols; x++){
                for(int c=0; c<3; c++){
                    dst.at<Vec3b>(y,x)[c] = 255 - src.at<Vec3b>(y,x)[c];
                }          
        }
    }
    return 0;
}


int average_blur3x3( cv::Mat &src, cv::Mat &dst ){

    cv::Mat temp;
    src.copyTo(temp);

    for(int c = 0; c < 3; c++){
        for(int y = 1; y < src.rows-1; y++){
            for(int x = 0; x < src.cols; x++){
                temp.at<Vec3b>(y,x)[c] = (src.at<Vec3b>(y-1, x)[c] + src.at<Vec3b>(y, x)[c] + src.at<Vec3b>(y+1, x)[c])/3;
            }
        }
    }
    for(int c = 0; c < 3; c++){
        for(int y = 0; y < src.rows; y++){
            for(int x = 1; x < src.cols-1; x++){
                dst.at<Vec3b>(y,x)[c] = (temp.at<Vec3b>(y, x-1)[c] + temp.at<Vec3b>(y, x)[c] + temp.at<Vec3b>(y, x+1)[c])/3;
            }
        }
    }
    return 0;
}

int imgSharpening(cv::Mat &src, cv::Mat3s &dst){

    for(int c = 0; c < 3; c++){
        for(int y = 1; y < src.rows-1 ; y++){
            for(int x = 1; x < src.cols-1; x++){
                dst.at<Vec3s>(y,x)[c] = - src.at<Vec3b>(y-1, x)[c] - src.at<Vec3b>(y, x-1)[c] + 
                5*src.at<Vec3b>(y, x)[c] - src.at<Vec3b>(y, x+1)[c] - src.at<Vec3b>(y+1, x)[c];
            }
        }
    }
    return 0;
}

int sketch(cv::Mat &src, cv::Mat &dst){

    cv::Mat temp_gray, temp_negative, temp_blur;

    temp_gray = src.clone();
    temp_negative = src.clone();
    temp_blur = src.clone();

    greyscale(src, temp_gray);
    negative(temp_gray, temp_negative);
    blur5x5(temp_negative, temp_blur);
   
            for(int y = 0; y < src.rows; y++){

                for(int x = 0; x < src.cols; x++){

                    for(int c=0; c<3; c++){
                        temp_gray.at<Vec3b>(y,x)[c] = temp_gray.at<Vec3b>(y,x)[c] == 255 ? 254: temp_gray.at<Vec3b>(y,x)[c];
                        dst.at<Vec3b>(y,x)[c] = (temp_blur.at<Vec3b>(y,x)[c]*255)/(255 - temp_gray.at<Vec3b>(y,x)[c]);
                        dst.at<Vec3b>(y,x)[c] = dst.at<Vec3b>(y,x)[c] > 255 ? 255:dst.at<Vec3b>(y,x)[c];

                    }
                }

    }
    return 0;
}
int upsideDownImg(cv::Mat &src, cv::Mat &dst){

    for(int c = 0; c < 3; c++){
        for(int y = 0; y < src.rows/2; y++){
            for(int x = 0; x < src.cols; x++){
                
                int swap = src.rows - 1 - y;
                unsigned char temp = src.at<Vec3b>(y,x)[c];
                dst.at<Vec3b>(y,x)[c] = dst.at<Vec3b>(swap,x)[c];
                dst.at<Vec3b>(swap,x)[c] = temp;
            }
        }
    }

}

int reflectImage( cv::Mat &src, cv::Mat &dst ){
    
        for (int y=0 ; y<src.cols ; y++){
            for (int x=0 ; x<src.rows ; x++)
            {
                for(int c=0; c<3; c++){
                    dst.at<Vec3b>(Point(y,x))[c] = src.at<Vec3b>(Point((src.cols-1)-y,x))[c];
                }
            }
        } 
    return 0;
}

int prewitX3(cv::Mat &src, cv::Mat3s &dst){

    cv::Mat temp;
    src.copyTo(temp);

    for(int c = 0; c < 3; c++){
        for(int y = 1; y < src.rows-1; y++){
            for(int x = 0; x < src.cols; x++){
                temp.at<Vec3b>(y,x)[c] = (0.33*src.at<Vec3b>(y-1, x)[c] + 0.33*src.at<Vec3b>(y, x)[c] + 0.33*src.at<Vec3b>(y+1, x)[c]);
            }
        }
    }
    for(int c = 0; c < 3; c++){
        for(int y = 0; y < src.rows; y++){
            for(int x = 1; x < src.cols-1; x++){
                dst.at<Vec3s>(y,x)[c] = (temp.at<Vec3b>(y, x-1)[c] + 0*temp.at<Vec3b>(y, x)[c] - temp.at<Vec3b>(y, x+1)[c]);
            }
        }
    }
    return 0;
}

int prewitY3(cv::Mat &src, cv::Mat3s &dst){

    cv::Mat3s temp;
    src.copyTo(temp);

    for(int c = 0; c < 3; c++){
        for(int y = 1; y < src.rows-1; y++){
            for(int x = 0; x < src.cols; x++){
                temp.at<Vec3s>(y,x)[c] = (src.at<Vec3b>(y-1, x)[c] + 0*src.at<Vec3b>(y, x)[c] - src.at<Vec3b>(y+1, x)[c]);
            }
        }
    }
    for(int c = 0; c < 3; c++){
        for(int y = 0; y < src.rows; y++){
            for(int x = 1; x < src.cols-1; x++){
                dst.at<Vec3s>(y,x)[c] = (temp.at<Vec3s>(y, x-1)[c] + temp.at<Vec3s>(y, x)[c] + temp.at<Vec3s>(y, x+1)[c])/3;
            }
        }
    }
    return 0;
}

int prewitMagnitude( cv::Mat &px, cv::Mat &py, cv::Mat &dst ){

    for(int y = 1; y < px.rows - 1; y++){
            for(int x = 1; x < px.cols - 1; x++){
                for(int c =0; c<3; c++){
                    
                    dst.at<Vec3b>(y,x)[c] = sqrt((px.at<Vec3b>(y,x)[c] * px.at<Vec3b>(y,x)[c]) + (py.at<Vec3b>(y,x)[c] * py.at<Vec3b>(y,x)[c]));
                }                      
            }
        }
    return 0;
}


int highIntensityBlur5x5( cv::Mat &src, cv::Mat &dst ){

    cv::Mat temp;
    temp = src.clone();
    int coeff[] = {2, 4, 5, 4, 2};

    for(int c = 0; c < 3; c++){
        for(int y = 0; y < src.rows; y++){
            for(int x = 2; x < src.cols-2; x++){
                temp.at<Vec3b>(y,x)[c] = (2*src.at<Vec3b>(y, x-2)[c] + 4*src.at<Vec3b>(y, x-1)[c] + 5*src.at<Vec3b>(y, x)[c] + 4*src.at<Vec3b>(y, x+1)[c] + 2*src.at<Vec3b>(y, x+2)[c])/17;
            }
        }
    }
    for(int c = 0; c < 3; c++){
        for(int y = 2; y < src.rows-2; y++){
            for(int x = 0; x < src.cols; x++){
                dst.at<Vec3b>(y,x)[c] = (2*temp.at<Vec3b>(y-2, x)[c] + 4*temp.at<Vec3b>(y-1, x)[c] + 5*temp.at<Vec3b>(y, x)[c] + 4*temp.at<Vec3b>(y+1, x)[c] + 2*temp.at<Vec3b>(y+2, x)[c])/17;
            }
        }
    }
    return 0;
}


    // int main()
    // {
    
    //     Mat src,dst;
    //     // Mat3s temp;
    //     int gx, gy;
    //     short sum;

    //     cv::Mat frame;
    //     cv::Mat result;
    //     cv::Mat sx, px;
    //     cv::Mat sy, py;
    //     cv:: Mat3s temp, temp_sx, temp_sy, temp_px, temp_py;
    
    //     // Load an image
    //     //src = imread("/home/ravina/sample/girl.jpg", cv::IMREAD_COLOR);
    //     src = imread("/home/ravina/sample/tiles.jpg", cv::IMREAD_COLOR);
    //     src.copyTo(dst);
    //     src.copyTo(temp);
    //     //dst = Mat::zeros(src.rows,src.cols, cv::IMREAD_GRAYSCALE); // grayscale
    //     if( !src.data )
    //     { return -1; }
    //     src.copyTo(result);
    //                 src.copyTo(temp_sx);
    //                 src.copyTo(temp_sy);                  

    //                 sobelX3x3(src, temp_sx);
    //                 convertScaleAbs(temp_sx, sx);

    //                 sobelY3x3(src, temp_sy);
    //                 convertScaleAbs(temp_sy, sy);

    //                 magnitude(sx, sy, result);

    
    //         // for(int y = 0; y < src.rows; y++)
    //         //     for(int x = 0; x < src.cols; x++)
    //         //         for(int c =0; c < 3; c++)
    //         //             dst.at<Vec3s>(y,x)[c] = 0.0;
                    
    //         sx = src.clone();
    //         sy = src.clone();

    //         //imgSharpening(src,temp);
    //        // prewitX3(src,temp);
            
                    
            
    //         // convertScaleAbs(temp,dst);
    //         //flipImage(src,dst);
    //         // std::cout<<"rows"<<src.rows<<"c= "<<src.cols;
    //         // sketch(src,dst);
    //         //dst.create(src.cols, src.rows, src.type());
    //         //mirrorImg(src, dst);
    //        // transpose(src, dst);
    //        // sketch(src,dst);

    //         //imgSharpening(src, dst);
    //         cartoon(src, dst, 10, 18);
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
    //             imshow("final", result);
    
    //         namedWindow("initial");
    //         imshow("initial", src);
    
    //     waitKey();
    
    
    //     return 0;
    // }
