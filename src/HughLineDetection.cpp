//
// Created by matejko on 12.3.2017.
//
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "HughLineDetection.h"
#include <unistd.h>

HughLineDetection::HughLineDetection(double rho, double theta, int threshold): rho(rho), threshold(threshold){
    double angle = 0 - theta;
    while(angle > -M_PI){
        angles.push_back(tan(angle));
        if(DEBUG)
            printf("angle = %lf tan = %lf\n",angle,tan(angle));
        angle-=theta;
    }
    printf("\n");
}

void HughLineDetection::getLines(cv::Mat &image) {
    std::vector<Data> data;
    for(int i= 0; i<image.rows;i++){
        for(int j=0;j<image.cols;j++){
            if(image.at<uint8_t>(i,j) >= 240){
                data.push_back(Data(Pixel(i,j),&angles,rho));
            }
        }
    }
    printf("%d white pixels found\n",(int)data.size());

    std::vector<std::vector<std::vector<Line*>>> lines(angles.size());
    for (int i = 0; i <lines.size() ; i++) {
        lines[i].resize(10000); //TODO nieco s tymto sprav
    }

    for (int i = 0; i < data.size(); i++)
        for (int j = 0; j < data[i].lines.size(); j++)
            if(data[i].lines[j].distance<10000) //TODO tiez nieco
                lines[data[i].lines[j].angleId][data[i].lines[j].distance].push_back(&data[i].lines[j]);
            else
                printf("skipping\n");


    std::vector<std::vector<Line*>*> longest;
    for (int i = 0; i < lines.size(); i++) {
        for (int j = 0; j < lines[i].size(); j++) {
            if(lines[i][j].size() > threshold){
                if(DEBUG)
                    printf("length = %d\n",(int)lines[i][j].size());
                longest.push_back(&(lines[i][j]));
            }
        }
    }
    cv::Mat resized;
    //cv::resize(image,resized,cv::Size(image.cols * 3,image.rows * 3),0,0,3);
    cv::imshow("rrr",image);
    cv::waitKey();

    for (int i = 0; i < longest.size() ; i++) {
        for (int j = 0; j < longest[i]->size(); j++) {
            image.at<uint8_t>(cv::Point(longest[i]->at(j)->pixel.y,longest[i]->at(j)->pixel.x)) = 128;
            if(DEBUG)
                printf("x %d y %d\n",longest[i]->at(j)->pixel.x,longest[i]->at(j)->pixel.y);
        }
        printf(" alfa %lf distance %d length %d\n",angles[longest[i]->at(0)->angleId], longest[i]->at(0)->distance,(int)longest[i]->size());
    }


    //cv::resize(image,resized,cv::Size(image.cols * 3,image.rows * 3),0,0,3);
    cv::imshow("eee",image);
    cv::waitKey();

}

