#include<iostream>
#include<cmath>
for(int y = 0; y < src.rows; y++){
        for(int x = 0; x < src.cols; x++){
            dst.at<Vec3s>(y,x) = (temp_blur.at<Vec3b>(y,x)*255)/(255 - 1*temp_gray.at<Vec3b>(y,x));
            dst.at<uchar>(y,x) = dst.at<uchar>(y,x) > 255 ? 255:dst.at<uchar>(y,x);
            
        }
    }
// tb = (temp_blur.at<uchar>(y,x)*255);
            // temp_gray.at<uchar>(y,x) = temp_gray.at<uchar>(y,x)==255 ? 254: temp_gray.at<uchar>(y,x);

            // tg = (255 - temp_gray.at<uchar>(y,x));
            // dst.at<uchar>(y,x) = tb/tg;

            // dst.at<uchar>(y,x) = dst.at<uchar>(y,x) > 255 ? 255:dst.at<uchar>(y,x);
    for(int y = 0; y < src.rows; y++){
        for(int x = 0; x < src.cols; x++){
            for(int c=0; c<3; c++){
                // dst.at<Vec3b>(y,x)[c] = (temp_blur.at<Vec3b>(y,x)[c]*255)/(255 - 1*temp_gray.at<Vec3b>(y,x)[c]);
                // dst.at<Vec3b>(y,x)[c] = dst.at<Vec3s>(y,x)[c] > 255 ? 255:dst.at<Vec3s>(y,x)[c];
            }
            
            
        }
    }
int cannyEdgeDetection(cv::Mat &src, cv::Mat &dst){

    //step 1 blur the image using gaussian blur
    cv::Mat tempBlur;
    src.copyTo(tempBlur);

    gaussian5x5(src, tempBlur);

    //gradient magnitude
    cv:: Mat3s temp_sx, temp_sy;
    cv::Mat resultMagnitude, sx, sy;

    src.copyTo(resultMagnitude);
    src.copyTo(temp_sx);
    src.copyTo(temp_sy);                  

    sobelX3x3(src, temp_sx);
    convertScaleAbs(temp_sx, sx);

    sobelY3x3(src, temp_sy);
    convertScaleAbs(temp_sy, sy);

    magnitude(sx, sy, resultMagnitude);

    //normalize
    normalize(resultMagnitude, resultMagnitude, 0, 1, NORM_MINMAX);

    //non-max supression
    int neighbor1X, neighbor1Y, neighbor2X, neighbor2Y;  
    float gradientAngle;

    for(int c = 0; c < 3; c++){
        for(int y = 0; y < tempBlur.rows; y++){
            for(int x = 0; x < tempBlur.cols; x++){
                gradientAngle = angle.at<float>(y, x);    
                if (abs(gradientAngle) > 180)        
                gradientAngle = abs(gradientAngle-180);      
                else        
                gradientAngle = abs(gradientAngle);       
                
                if (gradientAngle <= 22.5) {        
                neighbor1X = x-1; neighbor1Y = y;
                neighbor2X = x+1; neighbor2Y = y;      
                } else if (22.5 < gradientAngle <= 67.5) {        
                neighbor1X = x-1; neighbor1Y = y-1;        
                neighbor2X = x+1; neighbor2Y = y+1;      
                } else if (67.5 < gradientAngle <= 112.5) {        
                neighbor1X = x; neighbor1Y = y-1;        
                neighbor2X = x; neighbor2Y = y+1;      
                } else if (112.5 < gradientAngle <= 157.5) {        
                neighbor1X = x-1; neighbor1Y = y+1;        
                neighbor2X = x+1; neighbor2Y = y-1;      
                } else if (157.5 < gradientAngle <= 202.5) {        
                neighbor1X = x-1; neighbor1Y = y;        
                neighbor2X = x+1; neighbor2Y = y;      
                }   

                if ((0 <= neighbor1X < tempBlur.rows) && (0 <= neighbor1Y < tempBlur.cols)) {        
                    if (resultMagnitude.at<float>(y, x) < resultMagnitude.at<float>(neighbor1Y, neighbor1X)) {          
                        resultMagnitude.at<float>(y, x) = 0;          
                        continue;        
                    }      
                }      
                if ((0 <= neighbor2X < tempBlur.rows) && (0 <= neighbor2Y < tempBlur.cols)) {        
                    if (resultMagnitude.at<float>(y, x) < resultMagnitude.at<float>(neighbor2Y, neighbor2X)) {          
                        resultMagnitude.at<float>(y, x) = 0;          
                        continue;        
                    }      
                }
            }
        }
    }


}
