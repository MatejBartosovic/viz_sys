//
// Created by matejko on 12.3.2017.
//

#ifndef VIZ_SYS_HUGHLINEDETECTION_H
#define VIZ_SYS_HUGHLINEDETECTION_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

class HughLineDetection {

public:

    struct Pixel{
    public:
        uint32_t x;
        uint32_t y;
        Pixel(uint32_t x,uint32_t y) : x(x), y(y){
        }
    };
    class Line{
    public:
        int angleId;
        uint32_t distance;
        Pixel pixel;
        Line(int angleId, uint32_t distance, Pixel pixelr):angleId(angleId), distance(distance),pixel(pixelr){
        }
    };

    class Data{
    public:
        Pixel pixel;
        std::vector<double> *angles;
        std::vector<Line> lines;

        inline Data(Pixel pixel, std::vector<double>* angles,double rho) : pixel(pixel), angles(angles){
            //calculate polar
            double r = sqrt(pow(pixel.x,2) + pow(pixel.y,2));
            double fi = atan2(pixel.x,pixel.y);
            #ifdef DEBUG
            printf(" r = %lf fi = %lf\n",r,fi);
            #endif
            //calculate distance for each angle
            for(int i = 0;i<angles->size();i++) {
                double delta = M_PI_2 + fi - angles->at(i);
                double d = cos(delta) * r;
                lines.push_back(Line(i,d,this->pixel));
                #ifdef DEBUG
                printf(" x = %d y = %d delta %lf angle = %lf distance = %lf\n",pixel.x, pixel.y,delta,angles->at(i),d);
                #endif
            }
        }
    };
    std::vector<double> angles;
protected:
    int rho;
    int threshold;
public:
    HughLineDetection(double rho, double theta, int threshold);
    void getLines(cv::Mat &image);
};
#endif //VIZ_SYS_HUGHLINEDETECTION_H
