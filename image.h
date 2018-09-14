//
// Created by ilim0t on 2018/09/14.
//

#ifndef RENDERER_IMAGE_H
#define RENDERER_IMAGE_H
#include "vector.h"
#include <fstream>
#include <algorithm>

int tonemap(double x) {
    return std::pow(std::clamp(x, 0., 1.), 1/2.2) * 255;
}

struct Image {
    int width;
    int height;
    Vector* data;

    Image(int width, int height) : width(width), height(height), data(new Vector[width* height]) {}

    ~Image() {
        delete[] data;
    }

    void setPixel(int i, int j, const Vector& color) {
        data[width * i + j] = color;
    }

    Vector getPixel(int i, int j) const {
        return data[width * i + j];
    }

    void ppmOutput() const {
        std::ofstream ofs("../result.ppm");
        ofs << "P3\n" << width <<  " " << height << "\n255\n";
        for (int i=0; i < height; ++i) {
            for (int j=0; j < width; ++j) {
                Vector color = this->getPixel(j, i);
                ofs << tonemap(color.x) <<
                " " << tonemap(color.y) <<
                " " << tonemap(color.z) << std::endl;
            }
        }
    }

};

#endif //RENDERER_IMAGE_H
