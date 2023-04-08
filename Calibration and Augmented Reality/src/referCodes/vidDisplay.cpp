#include <opencv2/opencv.hpp>
#include<opencv2/highgui/highgui.hpp>
#include "filter.h"
#include <iostream>

using namespace cv;
using namespace std;


int main(int argc, char *argv[]) {
        cv::VideoCapture *capdev;

        // open the video device
        capdev = new cv::VideoCapture(0);
        if( !capdev->isOpened() ) {
                printf("Unable to open video device\n");
                return(-1);
        }

        // get some properties of the image
        cv::Size refS( (int) capdev->get(cv::CAP_PROP_FRAME_WIDTH ),
                       (int) capdev->get(cv::CAP_PROP_FRAME_HEIGHT));
        printf("Expected size: %d %d\n", refS.width, refS.height);

        cv::namedWindow("Video", 1);

        cv::Mat frame;
        cv::Mat result;
        cv::Mat sx, px;
        cv::Mat sy, py;
        cv:: Mat3s temp, temp_sx, temp_sy, temp_px, temp_py;
        
        string img_filter = "PREVIEW";
        
        for(;;) {
                *capdev >> frame; 
                
                if( frame.empty() ) {
                  printf("frame is empty\n");
                  break;
                }
                if (img_filter == "PREVIEW")
                    result = frame;
                else if (img_filter == "GRAY")
                    cvtColor(frame, result, cv::COLOR_RGB2GRAY);
                else if (img_filter == "ALTERNATIVEGRAY"){
                    frame.copyTo(result);
                    //result = Mat::zeros(frame.rows,frame.cols, CV_8UC1);
                    greyscale(frame, result);
                } 
                else if (img_filter == "BLUR"){
                    frame.copyTo(result);
                    blur5x5(frame, result);
                }
                else if (img_filter == "SOBEL3X"){
                    frame.copyTo(temp);
                    
                    sobelX3x3(frame, temp);
                    convertScaleAbs(temp,result);
                }
                else if (img_filter == "SOBEL3Y"){
                    frame.copyTo(temp);
                    
                    sobelY3x3(frame, temp);
                    convertScaleAbs(temp,result);
                }
                else if (img_filter == "GRADIENT MAGNITUDE"){
                    
                    frame.copyTo(result);
                    frame.copyTo(temp_sx);
                    frame.copyTo(temp_sy);                  

                    sobelX3x3(frame, temp_sx);
                    convertScaleAbs(temp_sx, sx);

                    sobelY3x3(frame, temp_sy);
                    convertScaleAbs(temp_sy, sy);

                    magnitude(sx, sy, result);
                    
                }
                else if (img_filter == "BLURQUANTIZE"){
                    frame.copyTo(result);
                    blurQuantize(frame, result, 15);
                }
                else if (img_filter == "CARTOONIZATION"){
                    frame.copyTo(result);
                    cartoon(frame, result, 15, 19);
                }
                else if(img_filter == "BRIGHTNESS"){
                    frame.copyTo(result);
                    brightness(frame, result);
                }
                else if(img_filter == "NEGATIVE"){
                    frame.copyTo(result);
                    negative(frame, result);
                }
                else if(img_filter == "AVERAGE BLUR"){
                    frame.copyTo(result);
                    average_blur3x3(frame, result);
                }
                else if(img_filter == "IMAGE SHARPENING"){
                    frame.copyTo(temp);
                    imgSharpening(frame, temp);
                    convertScaleAbs(temp, result);
                }
                else if(img_filter == "SKETCH"){
                    frame.copyTo(result);
                    sketch(frame, result);
                }
                else if(img_filter == "UPSIDEDOWNIMAGE"){
                    frame.copyTo(result);
                    upsideDownImg(frame, result);
                }
                else if(img_filter == "FLIPIMAGE"){
                    frame.copyTo(result);
                    reflectImage(frame, result);
                }
                else if(img_filter == "PREWIT3X"){
                    frame.copyTo(temp);
                    prewitX3(frame, temp);
                    convertScaleAbs(temp,result);
                }
                else if(img_filter == "PREWIT3Y"){
                    frame.copyTo(temp);
                    prewitY3(frame, temp);
                    convertScaleAbs(temp,result);
                }
                else if(img_filter == "PREWITMAGNITUDE"){
                    frame.copyTo(result);
                    frame.copyTo(temp_px);
                    frame.copyTo(temp_py);                  

                    sobelX3x3(frame, temp_px);
                    convertScaleAbs(temp_px, px);

                    sobelY3x3(frame, temp_py);
                    convertScaleAbs(temp_py, py);

                    prewitMagnitude(px, py, result);
                }
                

                cv::imshow("Video", result);
                char key = cv::waitKey(10);

                switch (key)
                {
                    case 's':
                        {
                            bool isSuccess = imwrite("/home/ravina/sample/MyImage.jpg", frame);
                            if (isSuccess == false)
                            {
                                cout << "Failed to save the image" << endl;
                                return -1;
                            }
                        }
                    break;
                    case 'g':
                    {                        
                        img_filter = "GRAY";                    
                        continue;
                    }                    
                    case 'h':
                    {
                        img_filter = "ALTERNATIVEGRAY";                    
                        continue;
                    }                    
                    case 'b':
                    {
                        img_filter = "BLUR";
                        continue;                  
                    }                    
                    case 'x':
                    {
                        img_filter = "SOBEL3X";                    
                        continue;
                    }
                    case 'y':
                    {
                        img_filter = "SOBEL3Y";                    
                        continue;
                    }
                    case 'm':
                    {
                        img_filter = "GRADIENT MAGNITUDE";                    
                        continue;
                    }
                    case 'i':
                    {
                        img_filter = "BLURQUANTIZE";                    
                        continue;
                    }
                    case 'c':
                    {
                        img_filter = "CARTOONIZATION";                    
                        continue;
                    }
                    case 'r':
                    {
                        img_filter = "BRIGHTNESS"; 
                        continue;
                    }
                    case 'n':
                    {
                        img_filter = "NEGATIVE"; 
                        continue;
                    }
                    case 'a':
                    {
                        img_filter = "AVERAGE BLUR"; 
                        continue;
                    }
                    case 'e':
                    {
                        img_filter = "IMAGE SHARPENING"; 
                        continue;
                    }
                    case 'k':
                    {
                        img_filter = "SKETCH"; 
                        continue;
                    }
                    case 'u':
                    {
                        img_filter = "UPSIDEDOWNIMAGE"; 
                        continue;
                    }
                    case 'f':
                    {
                        img_filter = "FLIPIMAGE"; 
                        continue;
                    }
                    case 'w':
                    {
                        img_filter = "PREWIT3X"; 
                        continue;
                    }
                    case 't':
                    {
                        img_filter = "PREWIT3Y"; 
                        continue;
                    }
                    case 'p':
                    {
                        img_filter = "PREWITMAGNITUDE"; 
                        continue;
                    }
                    case 'q':
                        delete capdev;
                        break;
                    default:
                        continue;
                }    
        }
        return(0);
}