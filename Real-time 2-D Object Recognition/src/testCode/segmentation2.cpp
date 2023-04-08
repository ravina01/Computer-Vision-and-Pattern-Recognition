#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "filter.h"

using namespace cv;

int main(int argc, char** argv )
{
        Mat img;
        img = imread("/home/ravina/Desktop/OpenCV/ObjectDetection/sample/img2P3.png");
        //img = imread("/home/ravina/Desktop/OpenCV/ObjectDetection/Pics/originalT1H1B1.png");

        // it works on one object in an image at a time
        if (img.empty())
        {
            cout << "Could not read input image file: " << endl;
            return -1;
        }

        namedWindow("Image", 1);
        imshow("Image", img);


        Mat image_th;
        cv::Mat gray_mat(img.size(), CV_8U);
        cv::cvtColor(img, gray_mat, cv::COLOR_RGB2GRAY); 
        image_th = Mat::zeros(img.rows,img.cols, CV_8UC1);
        thresholdingGray(gray_mat, image_th);
        imshow("thersolded", image_th);

        grassfireTransform(image_th, image_th);
        shrinkingImage(image_th, image_th);
        grassfireTransform(image_th, image_th);
        shrinkingImage(image_th, image_th);
        growingOperations(image_th, image_th);
        growingOperations(image_th, image_th);
        grassfireTransform(image_th, image_th);
        shrinkingImage(image_th, image_th);
        growingOperations(image_th, image_th);

        

        imshow("clean up binary ", image_th);

        Mat labelImage(img.size(), CV_32S);
        Mat stats, centroids;
        int nLabels = connectedComponentsWithStats(image_th, labelImage, stats, centroids, 8, CV_32S);
        cout<<nLabels<<endl;
        std::vector<Vec3b> colors(nLabels);
        std::vector<int> labels_finals;
        colors[0] = Vec3b(0, 0, 0);//background


        for (int label = 1; label < nLabels; ++label){ //label  0 is the background
            if ((stats.at<int>(label, CC_STAT_AREA)) > 2000){
                labels_finals.push_back(label);
                //cout << "hola" << endl;
            }

            cout << "area del component: " << label << "-> " << stats.at<int>(label, CC_STAT_AREA) << endl;
            //colors[label] = Vec3b((rand() & 255), (rand() & 255), (rand() & 255));
            colors[label] = Vec3b(0, 255, 0);
        }


        Mat dst(img.size(), CV_8UC3);
        for (int r = 0; r < dst.rows; ++r){
            for (int c = 0; c < dst.cols; ++c){

                int label = labelImage.at<int>(r, c);
                //cout << "label:  " << label << endl; 
                Vec3b &pixel = dst.at<Vec3b>(r, c);//accesa al elemento 
                pixel = colors[label];
            }
        }
        imshow("dst", dst);

        Mat dst2(img.size(), CV_8UC3);

        for (int i = 0; i < labels_finals.size(); ++i){
            std::cout << "path i:  " << labels_finals[i] << ' ' << endl;
            compare(labelImage, labels_finals[i], dst2, CMP_EQ);
        }

        imshow("compare imagem ", dst2);

    waitKey(0);
    return 0;

    // cvtColor(img, img, cv::COLOR_RGB2GRAY);
    // Mat imBin;
    // threshold(img,imBin,180,255,THRESH_BINARY);
    // imshow("Result", imBin);
    // waitKey();
    // Mat stats, centroids, labelImage;
    // int nLabels = connectedComponentsWithStats(imBin, labelImage, stats, centroids, 8, CV_32S);
    // Mat mask(labelImage.size(), CV_8UC1, Scalar(0));
    // Mat surfSup=stats.col(4)<200;

    // for (int i = 1; i < nLabels; i++)
    // {
    //     if (surfSup.at<uchar>(i, 0))
    //     {
    //         mask = mask | (labelImage==i);
    //     }
    // }
    // Mat r(img.size(), CV_8UC1, Scalar(0));
    // img.copyTo(r,mask);
    // imshow("Result", r);
    

}