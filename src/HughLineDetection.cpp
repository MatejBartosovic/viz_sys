//
// Created by matejko on 12.3.2017.
//
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "HughLineDetection.h"

HughLineDetection::HughLineDetection(double rho, double theta, int threshold): rho(rho), threshold(threshold){
    double angle = theta;
    while(angle < M_PI_2){
        angles.push_back(tan(angle));
        if(DEBUG)
            printf("angle = %lf",angle);
        angle+=theta;
    }
    printf("\n");
}

void HughLineDetection::getLines(cv::Mat &image) {
    std::vector<Data> data;
    for(int i= 0; i<image.rows;i++){
        for(int j=0;j<image.cols;j++){
            if(image.at<uint8_t>(i,j) >= 240){
                data.push_back(Data(Pixel(j,image.rows-i-1),&angles,rho));
            }
        }
    }
    printf("%d white pixels found\n",(int)data.size());

    std::vector<bool> procesed(data.size(),false);
    std::vector<Line> lines;

    for(int i = 0;i<data.size();i++){
        if(procesed[i])
            continue;
        for(int j=i+1;j<data.size();j++){
            if(procesed[j])
                continue;
            int id = data[i].isEqual(data[j]);
            if(DEBUG)
                printf("id = %d\n",id);
            if(id>=0){
                Line line(angles[id],data[i].distance[id]);
                line.dataId.push_back(i);
                line.dataId.push_back(j);
                for (int k = j+1; k <data.size() ; k++) {
                    //if(procesed[k])//mozon tobude treba dat prec
                      //  continue;
                    if(DEBUG)
                        printf("k = %d",k);
                    if(data[i].isEqual(data[k],id)){
                        line.dataId.push_back(k);
                        if(DEBUG)
                            printf("je\n");
                    }
                }
                if(line.dataId.size()>threshold){
                    if(DEBUG)
                        printf("get line size = %d\n",(int)line.dataId.size());
                    for (int k = 0; k < line.dataId.size(); k++) {
                        procesed[line.dataId[k]] = true;
                        if(DEBUG)
                            printf("mam bod %d\n",line.dataId[k]);
                    }
                    lines.push_back(line);
                }
            }
        }
    }

    printf("%d lines detected\n",(int)lines.size());
    for (int i = 0; i < lines.size(); i++) {
        printf("\nalfa = %lf distance = %d size =%d\n",atan(lines[i].angle),lines[i].distance,(int)lines[i].dataId.size());
        printf("pixels:\n");
        /*for (int j = 0; j < lines[i].dataId.size(); j++) {
            printf("x = %d y =%d\n",data[lines[i].dataId[j]].pixel.x,data[lines[i].dataId[j]].pixel.y);
        }*/
    }
}

