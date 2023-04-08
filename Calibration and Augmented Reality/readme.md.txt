NOTE : Time-travel Day taken : 2 days (taking it for the second time)
1.Link to Screen Recording of all implemented filters : 
        1. https://youtu.be/HO7w6VCAFcQ
        2. https://www.youtube.com/watch?v=1DOw8BqG23k


2. Wiki Report URL : https://wiki.khoury.northeastern.edu/x/ftd5Bg

3. Details of Operating system : Linux Ubuntu 20.4

4. OpenCV Version : 4.5.0

5. cmake version : 3.16.3

6. Instructions to execute files as follows:

    1. executables added to run cameraCalib.cpp, main.cpp, boardDetect, augmentedRealityTV.cpp, and objectRendering.cpp in CMakeLists.txt file.
    2. But, other Utility files such as: filter.cpp and ObjRender.cpp already added in CMakeLists.txt file.
    3. Run : 
            cameraCalib.cpp: It will display input of Task 1 to 3 : Deetect extract chessboard corners, select calibration images and Calibrate Camera 
            Press : (Task 1, 2, 3 cases)
                    c : select calibration corner points and save them all in vectors
                    m : Calibrate the camera using selected calibration images and write the camera matrix and distortion coefficients in yaml file
                    (Task 7 case)
                    h : detect harrris orners
                    (Extension Task=4 cases)
                    t : detect shiTomasi corners
                    f : implement SIFT (feature detector and descriptor)
    4. Run main.cpp : This file executes Task : 4, 5, 6 and draw virual cube as the final output
    5. Run boardDetect.cpp : This is an extension task=1 and this it detectcs multiple objects.
    6. Run augmentedRealityTV.cpp : This is an extesion task=2, It detects four aruco markers with Ids ranging from 0 to 3 and 
                                    overlaps video frame on top of it.

        Video Links of Input Video and Projected videos are:
        1. input Video : https://github.com/ravina01/AR-Videos/blob/main/minuteV.mp4
        2. Projected Video : https://github.com/ravina01/AR-Videos/blob/main/ProjVid.mp4
    7. Run objectRendering.cpp : This is an extesion task=3, it renders .obj file and display the same on checkerboard.

    NOTE : I have to remove q key detection to terminate the program as video frames processing was too slow. 
    So please terminate the program with ctrl+c

    Thank you !