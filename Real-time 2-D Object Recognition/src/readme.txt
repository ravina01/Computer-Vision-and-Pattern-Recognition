NOTE : Time-travel Day taken : 2 days (taking it for thr first time)
1.Link to Screen Recording of all implemented filters : https://github.com/ravina01/2DObject-Detection/blob/main/2DObjectDetection.mp4

2. Wiki Report URL : https://wiki.khoury.northeastern.edu/display/~ravinalad/Project+3+%3A+2D+Object+Detection

3. Details of Operating system : Linux Ubuntu 20.4

4. OpenCV Version : 4.5.0

5. cmake version : 3.16.3

6. Instructions to execute files as follows:

    1. executables added to run vidDisplay.cpp and main.cpp, task7 in CMakeLists.txt file.
    2. But, to execute imgDisplay.cpp, need to change the CMakeLists.txt : Line 6 - add_executable (${PROJECT_NAME}  imgDisplay.cpp)
    3. Run : vidDisplay: It will display input of Task 1 to 3 : thresholding, segmentation and clean-up binary image. 
            Press : g : gray space thresholding,
                    h : HSV space thresholding,
                    c : clean-up binary,
                    s : segmentation of Image
    4. Run Task 4_5 : it will compute features of images and append them in csv files
    5. Run main.cpp : this file contains all task from : 1 to 6, and it is working on live video
    6. Run task7.cpp : this will compute feature vectors and classify them using k-means clustering

