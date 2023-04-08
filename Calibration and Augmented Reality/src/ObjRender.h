#ifndef OBJRENDER
#define OBJRENDER

#include<vector>
#include<iostream>
#include<fstream>
#include<sstream>
#include<opencv2/core.hpp>

class ObjRender{

    public:
        ObjRender();
        ~ObjRender();

        void renderFile(std::string fileName);

        std::vector<cv::Point3f> vertices;
        std::vector<cv::Point3f> normals;
        std::vector<std::vector<int>> faceVert;
        std::vector<int> faceNorm;

    private:

        std::vector<std::string> split(std::string &str, char delim);

};
#endif