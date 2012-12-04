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

int main(int argc, const char * argv[])
{
    // Load image
    Mat img = imread("/Users/tamen/Documents/Archive/Skole/SDU/7Semester/VIS/Projects/Project 2/vis1_pro2_sequences/sequence3/8.png");
    vector<Mat> planes;
    split(img, planes);
    
    // Which channel to use:
    int channel = 1;
    
    vector<Vec3f> circles1;
    vector<Vec3f> circles2;
    
    GaussianBlur(planes[channel], planes[channel], Size(7,7), 1.5, 1.5);
    HoughCircles( planes[channel], circles1, CV_HOUGH_GRADIENT, 2, 1000, 200, 100, 45, 50);
    
    // Black out the part of the image we do not need:
    for (int x = 0; x < img.cols; x++) {
        for (int y = 0; y < img.rows; y++) {
            double d = sqrt(pow((double)x - (double)circles1[0][0], 2.0) + pow((double)y - (double)circles1[0][1], 2.0));
            if (d > cvRound(circles1[0][2])) {
                planes[channel].at<uchar>(y, x) = 0;
            }
        }
    }
    
    split(img, planes);
    GaussianBlur(planes[channel], planes[channel], Size(9,9), 2.0, 2.0);
    HoughCircles( planes[channel], circles2, CV_HOUGH_GRADIENT, 2, 5, 200, 100, 0, 0);

//    Point center(cvRound(circles1[0][0]), cvRound(circles1[0][1]));
//    int radius = cvRound(circles1[0][2]);
//    circle( img, center, 3, Scalar(0,255,0), -1, 8, 0 );
//    circle( img, center, radius, Scalar(0,0,255), 3, 8, 0 );
    
    /// Draw the circles detected
    for( size_t i = 0; i < circles2.size(); i++ )
    {
        Point center(cvRound(circles2[i][0]), cvRound(circles2[i][1]));
        int radius = cvRound(circles2[i][2]);
        // circle center
        circle( img, center, 3, Scalar(0,255,0), -1, 8, 0 );
        // circle outline
        circle( img, center, radius, Scalar(0,0,255), 3, 8, 0 );
    }
    
    
    imshow( "circles", planes[channel] );
    
    cout << "Circles: " << circles2.size() << endl;
    
    // Pause
    cv::waitKey();
    
    std::cout << "Done!" << endl;
    return 0;
}

