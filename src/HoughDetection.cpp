//
// Created by matejko on 12.3.2017.
//
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "HughLineDetection.h"

int main( int argc, char** argv)
{
    cv::Mat image;
    image = cv::imread("vizualne.jpg", CV_LOAD_IMAGE_GRAYSCALE);   // Read the file


    HughLineDetection rrr(4,0.1,12);

    rrr.getLines(image);
    //cv::imshow("rrr",image);
    //cv::waitKey();

    return 0;
}