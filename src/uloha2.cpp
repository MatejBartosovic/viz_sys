//
// Created by matejko on 12.3.2017.
//
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "HoughLineDetection.h"

int main( int argc, char** argv)
{
    cv::Mat image;
    image = cv::imread("vizualne3.jpg", CV_LOAD_IMAGE_GRAYSCALE);   // Read the file


    HughLineDetection rrr(1,0.01,170);

    rrr.getLines(image);

    return 0;
}