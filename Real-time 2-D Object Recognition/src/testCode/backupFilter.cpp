#include<iostream>
#include<cmath>
#include "filter.h"
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

// int greyscale(cv::Mat &src, cv::Mat &dst){

//     for (int y=0; y<src.rows ; y++){
//         for (int x=0 ; x<src.cols ; x++){  
//             int sum = 0;
//             for(int c=0; c< 3; c++){
//                 sum += src.at<Vec3b>(y,x)[c]/3;
//             } 
            
//             dst.at<uchar>(y,x) = sum;   
                
            
//         }
//     }
//     return 0;
// }

int greyscale(cv::Mat &src, cv::Mat &dst){

    for (int y=0; y<src.rows ; y++){
        for (int x=0 ; x<src.cols ; x++){  
            int sum = 0;
            int minVal = min(src.at<Vec3b>(y,x)[0], src.at<Vec3b>(y,x)[1], src.at<Vec3b>(y,x)[2]);
            for(int c=0; c< 3; c++){
                sum += src.at<Vec3b>(y,x)[c]/3;
            } 
            sum = sum > 130 ? sum*0.75 : (255- ((255-sum)*0.75) ) ;
            float saturation = (1 - 3*minVal)/sum;
            saturation = (1.25 - saturation) * sum;
            dst.at<uchar>(y,x) = saturation;   
                
            
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

int thresholdingGray( cv::Mat &src, cv::Mat &dst ){

    for(int y = 0; y < src.rows; y++){
        for(int x = 0; x < src.cols; x++){
            dst.at<uchar>(y,x) = src.at<uchar>(y,x) > 78 ? 0 : 255;
        }
    }
    return 0;
}

int thresholdingHSV( cv::Mat &src, cv::Mat &dst ){

    for(int c = 0; c < 3; c++){
        for(int y = 0; y < src.rows; y++){
            for(int x = 0; x < src.cols; x++){
                dst.at<uchar>(y,x) = src.at<Vec3b>(y,x)[1] > 75 ? 255 : 0;
            }
        }
    }
    convertScaleAbs(dst,dst);
   
    return 0;
}
int grassfireTransform( cv::Mat &src, cv::Mat &dst ){

    for(int y = 0; y < src.rows; y++){
        for(int x = 0; x < src.cols; x++){
            if( src.at<uchar>(y,x)  != 0)
            {
                unsigned char north = (y == 0) ? 0 : dst.at<uchar>(y-1,x);
                unsigned char west = (x == 0) ? 0 : dst.at<uchar>(y,x-1);
                dst.at<uchar>(y,x) = saturate_cast<uchar>(1 + min(north, west));
            }
            else{
                dst.at<uchar>(y,x) = 0;
            }
        }
    }

    for(int y = src.rows-1; y < -1; y--){
        for(int x = src.cols-1; x < -1; x--){
            if( (dst.at<uchar>(y,x)) != 0)
            {
                unsigned char south = (y == src.rows-1) ? 0 : dst.at<uchar>(y+1,x);
                unsigned char east = (x == src.cols-1) ? 0 : dst.at<uchar>(y,x+1);
                dst.at<uchar>(y,x) = min(dst.at<uchar>(y,x) , saturate_cast<uchar>(1 + min(south, east)));
            }
            else{
                dst.at<uchar>(y,x) = 0;
            }
        }
    }

    return 0;
}
int shrinkingImage( cv::Mat &src, cv::Mat &dst ){

    for(int y = 0; y < src.rows; y++){
        for(int x = 0; x < src.cols; x++){
            dst.at<uchar>(y,x) = src.at<uchar>(y,x) == 1 ? 0 : src.at<uchar>(y,x);
            dst.at<uchar>(y,x) = src.at<uchar>(y,x) > 1 ? 255 : src.at<uchar>(y,x);
        }
    }
    return 0;
}

int growingImage( cv::Mat &src, cv::Mat &dst ){

    for(int y = 0; y < src.rows; y++){
        for(int x = 0; x < src.cols; x++){
            dst.at<uchar>(y,x) = src.at<uchar>(y,x) == 1 ? 0 : src.at<uchar>(y,x);
            dst.at<uchar>(y,x) = src.at<uchar>(y,x) > 1 ? 255 : src.at<uchar>(y,x);
        }
    }
    return 0;
}

int invertImg( cv::Mat &src, cv::Mat &dst ){

    for(int y = 0; y < src.rows; y++){
        for(int x = 0; x < src.cols; x++){
            dst.at<uchar>(y,x) = src.at<uchar>(y,x) == 0 ? 1 : 0;
        }
    }
    return 0;
}
int invertGrowingImg( cv::Mat &src, cv::Mat &dst ){

    for(int y = 0; y < src.rows; y++){
        for(int x = 0; x < src.cols; x++){
            dst.at<uchar>(y,x) = src.at<uchar>(y,x) == 0 ? 255 : 0;
        }
    }
    return 0;
}

int growingOperations( cv::Mat &src, cv::Mat &dst ){

    invertImg(src, dst);
    grassfireTransform(dst, dst);
    growingImage(dst, dst);
    invertGrowingImg(dst, dst);
    return 0;
}


int segmenetation(cv::Mat &src, cv::Mat &dst2){

        Mat image_th;
        cv::Mat gray_mat(src.size(), CV_8U);

        image_th = Mat::zeros(src.rows,src.cols, CV_8UC1);

        cv::cvtColor(src, gray_mat, cv::COLOR_RGB2GRAY);
        //greyscale(src,gray_mat);
        thresholdingGray(gray_mat, image_th);
        
        // Mat result;
        // src.copyTo(result);
        // cvtColor(src, result, cv::COLOR_RGB2HSV);
        // thresholdingHSV(result, image_th);
        
        imshow("thresolded Img", image_th);

        grassfireTransform(image_th, image_th);
        shrinkingImage(image_th, image_th);
        grassfireTransform(image_th, image_th);
        shrinkingImage(image_th, image_th);
        
        growingOperations(image_th, image_th);
        growingOperations(image_th, image_th);
        grassfireTransform(image_th, image_th);
        shrinkingImage(image_th, image_th);
        //growingOperations(image_th, image_th);
        

        
        imshow("clean binary", image_th);
       // imshow("clean up binary ", image_th);

        Mat labelImage(src.size(), CV_32S);
        Mat stats, centroids;
        int nLabels = connectedComponentsWithStats(image_th, labelImage, stats, centroids, 8, CV_32S);
        std::vector<Vec3b> colors(nLabels);
        std::vector<int> labels_finals;
        colors[0] = Vec3b(0, 0, 0);//background


        for (int label = 1; label < nLabels; ++label){ //label  0 is the background
            if ((stats.at<int>(label, CC_STAT_AREA)) > 1000){
                labels_finals.push_back(label);
                //cout << "hola" << endl;
            }

            cout << "area del component: " << label << "-> " << stats.at<int>(label, CC_STAT_AREA) << endl;
            //colors[label] = Vec3b((rand() & 255), (rand() & 255), (rand() & 255));
            colors[label] = Vec3b(0, 255, 0);
        }


        Mat dst(src.size(), CV_8UC3);
        for (int r = 0; r < dst.rows; ++r){
            for (int c = 0; c < dst.cols; ++c){

                int label = labelImage.at<int>(r, c);
                //cout << "label:  " << label << endl; 
                Vec3b &pixel = dst.at<Vec3b>(r, c);//accesa al elemento 
                pixel = colors[label];
            }
        }

       // Mat dst2(src.size(), CV_8UC3);

        for (int i = 0; i < labels_finals.size(); ++i){
            std::cout << "path i:  " << labels_finals[i] << ' ' << endl;
            compare(labelImage, labels_finals[i], dst2, CMP_EQ);
        }

        imshow("compare imagem ", dst);


    return 0;

}

int greyscale(cv::Mat &src, cv::Mat &dst){

    for (int y=0; y<src.rows ; y++){
        for (int x=0 ; x<src.cols ; x++){  
            //int sum = 0;
            int minVal = min(src.at<Vec3b>(y,x)[0], src.at<Vec3b>(y,x)[1], src.at<Vec3b>(y,x)[2]);
            float sum = (src.at<Vec3b>(y,x)[0], src.at<Vec3b>(y,x)[1], src.at<Vec3b>(y,x)[2])/3;
            // for(int c=0; c< 3; c++){
            //     sum += src.at<Vec3b>(y,x)[c]/3;
            // }
            sum = (sum > 130) ? sum*0.75 : (255- ((255-sum)*0.75) );
            float saturation = (float)(1 - 3*minVal)/sum;
            saturation = (1.25 - saturation)*sum;
            dst.at<uchar>(y,x) = saturation;
                
            
        }
    }
    return 0;
}