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
        lines[i].resize(1000); //TODO nieco s tymto sprav
    }

    for (int i = 0; i < data.size(); i++)
        for (int j = 0; j < data[i].lines.size(); j++){
            lines[data[i].lines[j].angleId][data[i].lines[j].distance].push_back(&data[i].lines[j]);
            printf("array x =%d y= %d\n",lines[data[i].lines[j].angleId][data[i].lines[j].distance][0]->pixel.x,lines[data[i].lines[j].angleId][data[i].lines[j].distance][0]->pixel.y);
            printf("array distance =%d\n",lines[data[i].lines[j].angleId][data[i].lines[j].distance][0]->distance);
            printf("data pointer x = %d y = %d\n",data[i].lines[0].pixel.x,data[i].lines[0].pixel.y);
            printf("data object x = %d y = %d\n",data[i].pixel.x,data[i].pixel.y);
            //printf("lines x = %d y = %d\n",lines[data[i].lines[j].angleId][data[i].lines[j].distance][j]->pixel->x,lines[data[i].lines[j].angleId][data[i].lines[j].distance][j]->pixel->y);
        }



    std::vector<Line*> *longest;
    uint16_t length = 0;
    for (int i = 0; i < lines.size(); i++) {
        //printf("aaa %d %d\n",lines.size(),i);
        for (int j = 0; j < lines[i].size(); j++) {
            if(lines[i][j].size() > length){
                length = lines[i][j].size();
                printf("length = %d\n",length);
                printf("x %d y %d\n",lines[i][j][0]->pixel.x,lines[i][j][0]->pixel.y);
                longest = &(lines[i][j]);
            }
        }
    }
    cv::Mat resized;
    cv::resize(image,resized,cv::Size(image.cols * 3,image.rows * 3),0,0,3);
    cv::imshow("rrr",resized);
    cv::waitKey();
    cv::Vec3b red;
    red[0]=255;
    red[1]=0;
    red[2]=0;
    for (int i = 0; i < longest->size(); ++i) {
        printf("x %d y %d\n",longest->at(i)->pixel.x,longest->at(i)->pixel.y);
        image.at<uint8_t>(cv::Point(longest->at(i)->pixel.x,longest->at(i)->pixel.y)) = 128;
    }

    cv::resize(image,resized,cv::Size(image.cols * 3,image.rows * 3),0,0,3);
    cv::imshow("eee",resized);
    cv::waitKey();

    printf("najdlhsia ma %d pixelov\n",length);
    printf("najdlhsia alfa %lf distance %d\n",angles[longest->at(0)->angleId], longest->at(0)->distance);

}

