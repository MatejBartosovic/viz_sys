//
// Created by matejko on 12.3.2017.
//

#ifndef VIZ_SYS_HUGHLINEDETECTION_H
#define VIZ_SYS_HUGHLINEDETECTION_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

class HughLineDetection {

private:


    class Pixel{
    public:
        uint32_t x;
        uint32_t y;
        Pixel(uint32_t x,uint32_t y) : x(x), y(y){
        }

        /*Pixel(const HughLineDetection::Pixel& obj){
            this->x=obj.x;
            this->y=obj.y;
        }*/
    };

    class Data{
    public
        :
        Pixel pixel;
        std::vector<uint8_t> distance;
        std::vector<double> *angles;

        inline Data(Pixel pixel, std::vector<double>* angles,double rho) : pixel(pixel), angles(angles){
            //calculate distance for each angle
            for(int i = 0;i<angles->size();i++) {
                double citatel = pixel.y +pixel.x*angles->at(i);
                double x = citatel / (2*angles->at(i));
                double y = citatel / 2;
                distance.push_back((int)sqrt(pow(x,2)+pow(y,2))/rho);
            }
        }

        int isEqual(Data &other){
            for(uint32_t i=0;i<distance.size();i++)
                if(this->distance[i] == other.distance[i])
                    return i;
            return -1;
        }

        bool  isEqual(Data &other, int id){
            return (this->distance[id] == other.distance[id]);
        }
    };
    std::vector<double> angles;
protected:
    int rho;
    int threshold;
public:
    HughLineDetection(double rho, double theta, int threshold);
    void getLines(cv::Mat &image);
    struct Line{
        double angle;
        int distance;
        std::vector<int> dataId;
        Line (double angle,int distance) : angle(angle), distance(distance){}
    };
};
#endif //VIZ_SYS_HUGHLINEDETECTION_H
