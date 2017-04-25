//
// Created by matejko on 12.3.2017.
//
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "HughLineDetection.h"
#include <unistd.h>

HughLineDetection::HughLineDetection(double rho, double theta, int threshold): rho(rho), threshold(threshold){
    double angle = 0 ;
    while(angle < M_PI){
        angles.push_back(angle);
        angle+=theta;
    }
    angles.push_back(M_PI);
    printf("\n");
}

void HughLineDetection::getLines(cv::Mat &image) {

    /*
     * get white pixels and create data
     * */
    std::vector<Data> data;
    for(int i= 0; i<image.rows;i++){
        for(int j=0;j<image.cols;j++){
            if(image.at<uint8_t>(i,j) >= 240){
                //pozri header
                data.push_back(Data(Pixel(i,j),&angles,rho));
            }
        }
    }
    printf("%d white pixels found\n",(int)data.size());

    /*
     * resize output data
     * */
    std::vector<std::vector<std::vector<Line*>>> lines(angles.size());
    for (int i = 0; i <lines.size() ; i++) {
        lines[i].resize(10000); //TODO nieco s tymto sprav
    }


    /*
     * Find lines in data
     * */
    for (int i = 0; i < data.size(); i++)
        for (int j = 0; j < data[i].lines.size(); j++)
            if(data[i].lines[j].distance<10000) //TODO tiez nieco
                lines[data[i].lines[j].angleId][data[i].lines[j].distance].push_back(&data[i].lines[j]);
            else{}
                //printf("skipping\n");


    /*
     * Sort lines and chose appropriate lines
     * */
    std::vector<std::vector<Line*>*> longest;
    for (int i = 0; i < lines.size(); i++) {
        for (int j = 0; j < lines[i].size(); j++) {
            if(lines[i][j].size() > threshold){
                #ifdef DEBUG
                printf("length = %d\n",(int)lines[i][j].size());
                #endif
                longest.push_back(&(lines[i][j]));
            }
        }
    }

    /*
     * draw found lines
     * */
    cv::Mat resized;
    #ifdef RESIZE
    cv::resize(image,resized,cv::Size(image.cols * 5,image.rows * 5),0,0,3);
    cv::imshow("Original Image",resized);
    #else
    cv::imshow("Original Image",image);
    #endif
    cv::waitKey();

    cv::Mat colorImage;
    cv::cvtColor(image,colorImage,cv::COLOR_GRAY2BGR);
    cv::Mat lineImage = colorImage.clone();

    cv::Point3_<uchar> color(255,0,0);
    printf("velkost x = %d y = %d r =%lf\n",lineImage.rows,lineImage.cols,sqrt(pow(lineImage.rows,2)+ pow(lineImage.cols,2)));
    for (int i = 0; i < longest.size() ; i++) {
        cv::Mat currentLine = colorImage.clone();
        for (int j = 0; j < longest[i]->size(); j++) {
            currentLine.at<cv::Point3_<uchar> >(longest[i]->at(j)->pixel.x,longest[i]->at(j)->pixel.y) = color;
            lineImage.at<cv::Point3_<uchar> >(longest[i]->at(j)->pixel.x,longest[i]->at(j)->pixel.y) = color;
            #ifdef DEBUG
            //printf("x %d y %d\n",longest[i]->at(j)->pixel.x,longest[i]->at(j)->pixel.y);
            #endif
        }
        printf(" alfa %lf, alfa id =%d, distance %d, length %d\n",angles[longest[i]->at(0)->angleId], longest[i]->at(0)->angleId,longest[i]->at(0)->distance,(int)longest[i]->size());
        #ifdef RESIZE
        cv::resize(currentLine,resized,cv::Size(image.cols * 5,image.rows * 5),0,0,3);
        cv::imshow("Lines",resized);
        #else
        cv::imshow("Lines",currentLine);
        #endif
        uchar tmp = color.x;
        color.x = color.y;
        color.y = color.z;
        color.z = tmp;
        cv::waitKey();
    }
    #ifdef RESIZE
    cv::resize(lineImage,resized,cv::Size(image.cols * 5,image.rows * 5),0,0,3);
    cv::imshow("Lines",resized);
    #else
    cv::imshow("Lines",lineImage);
    #endif
    cv::waitKey();

}

