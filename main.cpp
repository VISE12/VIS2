//
//  main.cpp
//  Marker_Finder
//
//  Created by Nis Sarup on 26/11/12.
//  Copyright (c) 2012 Nis Sarup. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// Types
typedef float T;
typedef cv::Vec<T,3> VecT; // 3-vector
typedef cv::Mat_<VecT> ImgT; // 3-channel image
typedef cv::Mat_<T> MatT; // 1-channel container

void animate_one();
void find_marker_from_gray_image(string file);
void find_marker_in_HSV_image(string file);
void hsv_color_seperator_gui(string file);

int main(int argc, const char * argv[])
{
//    animate_one();
    hsv_color_seperator_gui("/Users/tamen/Documents/Archive/Skole/SDU/7Semester/VIS/Projects/Project 2/vis1_pro2_sequences/sequence3/8.png");
    std::cout << "Done!" << endl;
    return 0;
}

void hsv_color_seperator_gui(string file) {
    ImgT img = cv::imread(file);
    img /= 255;
    
    const int rows = img.rows;
    const int cols = img.cols;
    
    ImgT imghsv;
    cv::cvtColor(img, imghsv, CV_BGR2HSV);
    
    ImgT img1;
    
    int low = 0;
    int high = 360;
    int tollow = 0;
    int tolhigh = 100;
    
    
    namedWindow("Output", CV_WINDOW_NORMAL);
    createTrackbar("Low", "Output", &low, 360, NULL);
    createTrackbar("High", "Output", &high, 360, NULL);
    createTrackbar("TolLow", "Output", &tollow, 100, NULL);
    createTrackbar("TolHigh", "Output", &tolhigh, 100, NULL);
    
    while (true) {
        img1 = imghsv.clone();
        for(int i = 0; i < rows; ++i) {
            for(int j = 0; j < cols; ++j) {
                VecT& hsv = img1(i,j);
                
                const T& saturation = hsv[1];
                if (saturation < (double)(tollow / 100.0) || saturation > (double)(tolhigh / 100.0)) {
                    hsv = VecT(0,0,0);
                    continue;
                }
                
                const T& hue = hsv[0];
                if (hue < low || hue > high) {
                    hsv = VecT(0,0,0);
                }
            }
        }
        
        cv::cvtColor(img1, img1, CV_HSV2BGR);
        cv::imshow("Output", img1);
        
        cout << "High: " << getTrackbarPos("High", "Output") << endl;
        cout << "Low: " << getTrackbarPos("Low", "Output") << endl;
        cout << "High tol: " << (double)getTrackbarPos("TolHigh", "Output") / 100.0 << endl;
        cout << "Low tol: " << (double)getTrackbarPos("TolLow", "Output") / 100.0 << endl;
        
        int c = cvWaitKey(20);
        if((char)c==27)
            break;
    }

}

void find_marker_in_HSV_image(string file) {
    
    // Values found using hsv_color_seperator_gui()
    // For the big circle:
//        High: 198
//        Low: 157
//        High tol: 0.52
//        Low tol: 0.36
    // For the small circle:
//        High: 344
//        Low: 314
//        High tol: 0.66
//        Low tol: 0.46
}

void animate_one() {
    string folder = "/Users/tamen/Documents/Archive/Skole/SDU/7Semester/VIS/Projects/Project 2/vis1_pro2_sequences/sequence3/";
    
    for (int i = 1; i <= 30; i++) {
        stringstream out;
        out << folder << i << ".png";
        find_marker_from_gray_image(out.str());
    }
    waitKey();
}

void find_marker_from_gray_image(string file) {
    // Load image
    Mat org = imread(file, 1);
    Mat img = imread(file, 0);
    
    // Vector for found circles (Only one is found)
    vector<Vec3f> circles;
    
    // Blur the image and find the marker
    GaussianBlur(img, img, Size(7,7), 1.5, 1.5);
    HoughCircles( img, circles, CV_HOUGH_GRADIENT, 2, 1000, 200, 100, 45, 50);
    
    // Paint the marker
    Point center(cvRound(circles[0][0]), cvRound(circles[0][1]));
    int radius = cvRound(circles[0][2]);
    circle( org, center, 3, Scalar(0,255,0), -1, 8, 0 );
    circle( org, center, radius, Scalar(0,0,255), 3, 8, 0 );
    
    imshow( "circles", org);
    waitKey(10);
}