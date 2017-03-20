//
// Created by matejko on 12.3.2017.
//
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "HughLineDetection.h"
#include <unistd.h>

HughLineDetection::HughLineDetection(double rho, double theta, int threshold): rho(rho), threshold(threshold){
    double angle = -M_PI_2 + theta;
    while(angle < M_PI_2){
        angles.push_back(tan(angle));
        if(DEBUG)
            printf("angle = %lf\n",angle);
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
                //data.push_back(Data(Pixel(j,i),&angles,rho));
            }
        }
    }
    printf("%d white pixels found\n",(int)data.size());

    std::vector<std::vector<std::vector<Line*>>> lines(angles.size());
    for (int i = 0; i <lines.size() ; i++) {
        lines[i].resize(1000); //TODO nieco s tymto sprav
    }

    for (int i = 0; i < data.size(); i++)
        for (int j = 0; j < data[i].lines.size(); j++)
            lines[data[i].lines[j].angleId][data[i].lines[j].distance].push_back(&data[i].lines[j]);



    std::vector<Line*>* longest;
    uint16_t length = 0;
    for (int i = 0; i < lines.size(); i++) {
        //printf("aaa %d %d\n",lines.size(),i);
        for (int j = 0; j < lines[i].size(); j++) {
            if(lines[i][j].size() > length){
                length = lines[i][j].size();
                printf("length = %d\n",length);
                longest = &lines[i][j];
            }
        }
    }


    printf("najdlhsia ma %d pixelov\n",length);
    printf("najdlhsia alfa %lf distance %d\n",angles[longest->at(0)->angleId], longest->at(0)->distance);

}

