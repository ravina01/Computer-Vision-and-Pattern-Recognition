// int rgbtoHsv( cv::Mat &src, cv::Mat &dst ){
    
//         for(int y = 0; y < src.rows; y++){
//             for(int x = 0; x < src.cols; x++){
//                 double r = src.at<Vec3b>(y,x)[0]/255.0;
//                 double g = src.at<Vec3b>(y,x)[1]/255.0;
//                 double b = src.at<Vec3b>(y,x)[2]/255.0;

//                 double maxc = max(r, g,b);
//                 double minc = min(r,g,b);
//                 if(maxc == minc){
//                     dst.at<Vec3d>(y,x)[0] = 0;
//                     dst.at<Vec3d>(y,x)[1] = 0;
//                     dst.at<Vec3d>(y,x)[2] = maxc;

//                 }
                
//                 double s = (maxc - minc) / maxc;
//                 double rc = (maxc - r)/ (maxc - minc);
//                 double gc = (maxc - g)/ (maxc - minc);
//                 double bc = (maxc - b)/ (maxc - minc);

//                 if(r == maxc)
//                     dst.at<Vec3b>(y,x)[0] = 0.0+bc-gc;
//                 else if( g == maxc)
//                     dst.at<Vec3d>(y,x)[0] = 0.0+rc-bc;
//                 else
//                     dst.at<Vec3d>(y,x)[0] = 4.0+gc-rc;

//                 dst.at<Vec3d>(y,x)[0] = double (dst.at<Vec3d>(y,x)[0]/6.0) % double 1.0;
//                 dst.at<Vec3d>(y,x)[1] = s * 100;
//                 dst.at<Vec3d>(y,x)[2] = maxc * 100;

                
//             }
//         }
    


// }

int rgb_to_hsv(cv::Mat &src, cv::Mat &dst)

    {
        for(int y = 0; y < src.rows; y++){
            for(int x = 0; x < src.cols; x++){

                double r = src.at<Vec3b>(y,x)[0];
                double g = src.at<Vec3b>(y,x)[1];
                double b = src.at<Vec3b>(y,x)[2];

                r = r / 255.0;
                g = g / 255.0;
                b = b / 255.0;
                // h, s, v = hue, saturation, value
                double cmax = std::max(r, std::max(g, b)); // maximum of r, g, b
                double cmin = std::min(r, std::min(g, b)); // minimum of r, g, b
                double diff = cmax - cmin; // diff of cmax and cmin.
                double h = -1, s = -1;
                
                // if cmax and cmax are equal then h = 0
                if (cmax == cmin)
                    h = 0;
        
                // if cmax equal r then compute h
                else if (cmax == r)
                    h = double (60 * ((g - b) / diff) + 360) % double 360;
        
                // if cmax equal g then compute h
                else if (cmax == g)
                    h = double (60 * ((b - r) / diff) + 120) % double 360;
        
                // if cmax equal b then compute h
                else if (cmax == b)
                    h = double (60 * ((r - g) / diff) + 240) % double 360;
        
                // if cmax equal zero
                if (cmax == 0)
                    s = 0;
                else
                    s = (diff / cmax) * 100;
        
                // compute v
                double v = cmax * 100;
                //System.out.println("(" + h + " " + s + " " + v + ")");

                dst<Vec3b>(y,x)[0]= h;
                dst<Vec3b>(y,x)[1]= s;
                dst<Vec3b>(y,x)[2]= v;



            }
        }
 
       
        
 
        
 
    }
