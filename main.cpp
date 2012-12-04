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

void find_marker(string file);

int main(int argc, const char * argv[])
{
    string folder = "/Users/tamen/Documents/Archive/Skole/SDU/7Semester/VIS/Projects/Project 2/vis1_pro2_sequences/sequence3/";
    
    for (int i = 1; i <= 30; i++) {
        stringstream out;
        out << folder << i << ".png";
        find_marker(out.str());
    }
    waitKey();
    std::cout << "Done!" << endl;
    return 0;
}

void find_marker(string file) {
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