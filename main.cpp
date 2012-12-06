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

int main(int argc, const char * argv[])
{
    animate_one();
    waitKey();
    std::cout << "Done!" << endl;
    return 0;
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
    Mat org = imread(file, 1); // Full color, to show the found marker.
    Mat img = imread(file, 0); // Grayscale
    
    // Vector for found circles (Only one is found)
    vector<Vec3f> circles;
    
    // Blur the image and find the marker
    GaussianBlur(img, img, Size(7,7), 1.5, 1.5);
    HoughCircles(img, circles, CV_HOUGH_GRADIENT, 2, 1000, 200, 100, 45, 50);
    
    // Have we found the big circle?
    if (circles.size() > 0) {
        
        // Get the parameters for the big circle
        Point big_center(cvRound(circles[0][0]), cvRound(circles[0][1]));
        int big_radius = cvRound(circles[0][2]);
        
        // Split up the image in individual color planes.
        vector<Mat> planes;
        split(org, planes);
        
        // Black out the part of the image we do not need.
        for (int x = 0; x < img.cols; x++) {
            for (int y = 0; y < img.rows; y++) {
                double d = sqrt(pow((double)x - (double)big_center.x, 2.0) + pow((double)y - (double)big_center.y, 2.0));
                if (d > big_radius) {
                    // Outside the circle
                    planes[2].at<uchar>(y, x) = 0;
                }
            }
        }
        
        // Find the small circle, don't use blur this time.
        vector<Vec3f> circles2;
        HoughCircles(planes[2], circles2, CV_HOUGH_GRADIENT, 1, 1000, 100, 2, 15, 18);
        
        // Did we find the small circle?
        if (circles2.size() > 0) {
            Point small_center(cvRound(circles2[0][0]), cvRound(circles2[0][1]));
            circle( org, small_center, 3, Scalar(0,255,0), -1, 8, 0 );
            circle( org, small_center, cvRound(circles2[0][2]), Scalar(255,0,0), 1, 8, 0 );
            
            // Print out the distance between the centers of the two circles.
            double d = sqrt(pow((double)small_center.x - (double)big_center.x, 2.0) + pow((double)small_center.y - (double)big_center.y, 2.0));
            cout << "Distance: \t" << d << endl;
            
        }
        
        circle( org, big_center, 3, Scalar(0,255,0), -1, 8, 0 );
        circle( org, big_center, big_radius, Scalar(0,0,255), 1, 8, 0 );
    }
    
    // Show the current image with marker marked.
    imshow("circles", org);
    waitKey(10);
}