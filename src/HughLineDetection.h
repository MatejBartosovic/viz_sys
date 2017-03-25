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
            //calculate distance for each angle
            for(int i = 0;i<angles->size();i++) {
                double citatel = pixel.y + pixel.x * angles->at(i);
                double x = citatel / (2 * angles->at(i));
                double y = citatel / 2;
                int distance = sqrt(pow(x, 2) + pow(y, 2)) / rho;
                #ifdef DEBUG
                printf("angle = %lf distance = %d\n",angles->at(i),distance);
                #endif
                lines.push_back(Line(i, distance, this->pixel));
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
