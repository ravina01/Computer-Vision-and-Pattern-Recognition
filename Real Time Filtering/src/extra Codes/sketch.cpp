#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

int main(){

    //Read input image
    Mat image = imread("/home/ravina/sample/girl.jpg");

    //check if images exists
    if(image.empty()){
       // std::cout<<"can not find image"<<endl;
        return 0;
    }

    //create variable to store output image
    Mat output;

    //convert to gray scale
    Mat grayImage;
    cvtColor(image, grayImage, COLOR_BGR2GRAY);

    //invert the gray image
    Mat grayImageInv = 255 - grayImage;

    //apply gaussian blur
    GaussianBlur(grayImageInv, grayImageInv, Size(21, 21), 0);

    //blend using color dodge
    divide(grayImage, 255-grayImageInv, output, 256.0);

    //create windows to display images
    namedWindow("image", WINDOW_AUTOSIZE);
    namedWindow("pencilsketch", WINDOW_AUTOSIZE);

    //display images
    imshow("image", image);
    imshow("pencilsketch", output);

    //press esc to exit the program
    waitKey(0);

    //close all the opened windows
    destroyAllWindows();

    return 0;
}